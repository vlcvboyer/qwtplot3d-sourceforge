#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt_plot3d.h"

using namespace Qwt3D;
	
/*!
  Create a QwtPlot3D widget
*/
QwtPlot3D::QwtPlot3D( QWidget* parent, const char* name, MESHTYPE mt )
    : QGLWidget( parent, name )
{
	actualGridData_ = new GridData();
	actualCellData_ = new CellData();
	meshtype_ = mt;
		
	xRot_ = yRot_ = zRot_ = 0.0;		// default object rotation

	xShift_ = yShift_ = zShift_ = xVPShift_ = yVPShift_ = 0.0;
	xScale_ = yScale_ = zScale_ = 1.0;
	zoom_ = 8;
	resolution_ = 1;
	ortho_ = true;
	plotstyle_ = FILLEDMESH;
	floorstyle_ = NOFLOOR;
	isolines_ = 10;
	datanormals_ = false;

	lastMouseMovePosition_ = QPoint(0,0);
	mpressed_ = false;

	setPolygonOffset(0.5);
	setMeshColor(RGBA(0.0,0.0,0.0));
	setBackgroundColor(RGBA(1.0,1.0,1.0,1.0));

	objectList_ = std::vector<GLuint>(4);
	for (unsigned k=0; k!=objectList_.size(); ++k)
	{
		objectList_[k] = 0;
	}

	dataColor_ = new StandardColor(this, 100);
	title_.setFont("Courier", 16, QFont::Bold);
	title_.setString("");

	setCaptionPosition(0.95);
	
	assignMouse(Qt::LeftButton, 
							Qt::LeftButton | Qt::ShiftButton,
							Qt::LeftButton, 
							Qt::LeftButton | Qt::AltButton, 
							Qt::LeftButton | Qt::AltButton, 
							Qt::LeftButton | Qt::AltButton | Qt::ShiftButton,
							Qt::LeftButton | Qt::AltButton | Qt::ControlButton,
							Qt::LeftButton | Qt::ControlButton, 
							Qt::LeftButton | Qt::ControlButton);
}

/*!
  Release allocated resources
*/

QwtPlot3D::~QwtPlot3D()
{
	SaveGlDeleteLists( objectList_[0], objectList_.size() );
	dataColor_->destroy();
	delete actualGridData_;
	delete actualCellData_;
}


/*!
  Set up the OpenGL rendering state
*/
void 
QwtPlot3D::initializeGL()
{
  glEnable( GL_BLEND );
  glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Set up the lights


  glDisable(GL_LIGHTING);

	
	GLfloat whiteDir[4] = {2.0, 2.0, 2.0, 1.0};
  GLfloat reflection[4] = {0.0, 0.0, 0.0, 0.0};
  GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightPos[4] = {300.0, 300.0, 300.0, 1.0};

  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
  glMaterialfv(GL_FRONT, GL_SPECULAR, reflection);
  glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);		// enable diffuse
  glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos); 
	
}

/*!
  Paint the widgets content.
*/
void 
QwtPlot3D::paintGL()
{
	glClearColor(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
	
	glLoadIdentity();
  glColor3f(1.0, 1.0, 1.0);
	
  glRotatef( xRot_-90, 1.0, 0.0, 0.0 ); 
  glRotatef( yRot_, 0.0, 1.0, 0.0 ); 
  glRotatef( zRot_, 0.0, 0.0, 1.0 );

//	glTranslatef(xShift_, yShift_, zShift_);
	glEnable(GL_NORMALIZE);
	glScalef( zoom_ * xScale_, zoom_ * yScale_, zoom_ * zScale_ );
	glDisable(GL_NORMALIZE);
	
	
	Triple beg = coord.first();
	Triple end = coord.second();
	
	Triple center = beg + (end-beg) / 2;
	double radius = (center-beg).length();

	glTranslatef(xShift_-center.x, yShift_-center.y, zShift_-center.z);

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

	if (beg != end)
	{		
		if (ortho_)
			glOrtho( -radius, +radius, -radius, +radius, 0, 40 * radius);
		else
			glFrustum( -radius, +radius, -radius, +radius, 5 * radius, 400 * radius );
	}
	else
	{
		if (ortho_)
			glOrtho( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
		else
			glFrustum( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
	}
  
	glTranslatef( xVPShift_ * 2 * radius, yVPShift_ * 2 * radius, -7 * radius );

	for (unsigned i=0; i!= objectList_.size(); ++i)
	{
		if (i!=CoordObject)
			glCallList( objectList_[i] );
	}

  coord.draw();
		
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();  

	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	title_.setPosition(Triple(ViewPort2World(Triple((titlerelx_-vp[0])*vp[2],(titlerely_-vp[1])*vp[3],0))),titleanchor_);
	title_.draw();
}

/*!
  Set up the OpenGL view port
*/
void 
QwtPlot3D::resizeGL( int w, int h )
{
	glViewport( 0, 0, w, h );

	paintGL();
}

void 
QwtPlot3D::updateCoordinateSystem()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);

	SaveGlDeleteLists(objectList_[CoordObject], 1);
			
	objectList_[CoordObject] = glGenLists(1);
	glNewList(objectList_[CoordObject], GL_COMPILE);

	coord.draw();

	glEndList();
}

/*!
  Set data resolution (res == 1 original resolution) and updates widget
	If res < 1, the function does nothing
*/
void 
QwtPlot3D::setResolution( int res )
{
	if ((resolution_ == res) || res < 1)
		return;
	
	resolution_ = res;
	updateData();
	updateGL();

	emit resolutionChanged(res);
}

/**
	Create a coordinate system with generating corners beg and end 
*/
void
QwtPlot3D::createCoordinateSystem( Triple beg, Triple end )
{
	if (beg != coord.first() || end != coord.second())
		coord.init(beg, end);
}

/**
	Create a coordinate system from data
*/
void
QwtPlot3D::createCoordinateSystem()
{
	calculateHull();
	
	createCoordinateSystem(hull().minVertex, hull().maxVertex);
}

/*!
  Show a color legend <tt>(experimental)</tt>
*/
void 
QwtPlot3D::showColorLegend( bool show )
{
 	SaveGlDeleteLists(objectList_[LegendObject], 1);
	

	if (show)
	{
		objectList_[LegendObject] = glGenLists(1);
		glNewList(objectList_[LegendObject], GL_COMPILE);	

		legend_.draw();

		glEndList();
	}
	updateGL();
}


/**
	Saves the framebuffer to the file fileName using one of the image file formats supported by Qt 
*/
bool 
QwtPlot3D::saveContent(QString fileName, QString format)
{
	QImage im = grabFrameBuffer(true);
	return im.save(fileName,format);
}

void 
QwtPlot3D::setMeshColor(RGBA rgba)
{
	meshcolor_ = rgba;
}

void 
QwtPlot3D::setBackgroundColor(RGBA rgba)
{
	bgcolor_ = rgba;
}

/**
	Calculate the smallest x-y-z parallelepiped enclosing the data.
	It can be accessed by hull();
*/
void 
QwtPlot3D::calculateHull()
{
	if (meshtype() == GRID)
	{
		if (actualGridData_->empty())
			return;
		hull_ = actualGridData_->hull();
	}
	else
	{
		if (actualCellData_->empty())
			return;
		hull_ = actualCellData_->hull();
	}
}


/*!
	assign a new coloring object for the data.
*/
void 
QwtPlot3D::setDataColor( Color* col )
{
	Q_ASSERT(dataColor_);

	dataColor_->destroy();
	dataColor_ = col;
}

/*!
  <tt>(experimental)</tt>
*/
void 
QwtPlot3D::modifyStandardColorAlpha(double d)
{
	dataColor_->setAlpha(d);
}

/*!
  Set up ortogonal or perspective mode and updates widget
*/
void
QwtPlot3D::setOrtho( bool val )
{
	if (val == ortho_)
		return;
	ortho_ = val;
	updateGL();
	
	emit projectionChanged(val);
}

/*!
  <tt>(experimental)</tt>
*/
void 
QwtPlot3D::createColorLegend(ColorVector const& col, Triple a, Triple b, Triple c, Triple d)
{
		legend_.setPosition(a,b,c,d);
		legend_.colors = col;
}

/*!
  Set style of coordinate system
*/
void 
QwtPlot3D::setCoordinateStyle(COORDSTYLE st)
{
	coord.setStyle(st);
	updateCoordinateSystem();
	updateGL();
}

/*!
  Set plotting style
*/
void
QwtPlot3D::setPlotStyle( PLOTSTYLE val )
{
	if (val == plotstyle_)
		return;
	plotstyle_ = val;
	
	updateData();
	updateGL();
}

/*!
  Set style of floor data
*/
void
QwtPlot3D::setFloorStyle( FLOORSTYLE val )
{
	if (val == floorstyle_)
		return;
	
	floorstyle_ = val;
			
 	updateData(); 
	updateGL();
}

/*!
  Set number of isolines. The lines are equidistant between minimal and maximal Z value
*/
void 
QwtPlot3D::setIsolines(int steps)
{
	if (steps < 0)
		return;

	isolines_ = steps;
}

void 
QwtPlot3D::showNormals(bool b)
{
	datanormals_ = b;
}

/*!
  Set Polygon offset. The function affects the OpenGL rendering process. 
	Try different values for surfaces with polygons only and with mesh and polygons
	The value is clamped to [0..1]
*/
void
QwtPlot3D::setPolygonOffset( double val )
{
	Q_ASSERT(val>=0 && val<=1);

	if (val < 0 || val>1)
		return;
	
	polygonOffset_ = val;
}

/*!
Set relative caption position (0.5,0.5) means, the anchor point lies in the center of the screen
*/
void 
QwtPlot3D::setCaptionPosition(double rely, double relx, LabelPixmap::ANCHOR anchor)
{
	titlerely_= (rely<0 || rely>1) ? 0.5 : rely;
	titlerelx_= (relx<0 || relx>1) ? 0.5 : relx;

	titleanchor_ = anchor;
}

/**
Set caption font
*/
void 
QwtPlot3D::setCaptionFont(const QString& family, int pointSize, int weight, bool italic)
{ 
	title_.setFont(family, pointSize, weight, italic);
}
