#include <float.h>

#include "qwt_plot3d.h"
#include "types.h"
#include "functiongenerator.h"

Function::Function(QwtPlot3D* pw)
{
	if (!pw)
	{
		fprintf(stderr,"NativeReader: no valid QwtPlot3D Widget");
		return;
	}
	plotwidget_ = pw;
	setMesh(0,0);
	setDomain(0,0,0,0);
	setMinZ(-DBL_MAX);
	setMaxZ(DBL_MAX);
}

Function::~Function()
{ 
}

void
Function::setMesh(unsigned int columns,unsigned int rows)
{
	xmesh_ = columns;
	ymesh_ = rows;
}

void
Function::	setDomain(double minx, double maxx, double miny, double maxy)
{
	minx_ = minx;
	maxx_ = maxx;
	miny_ = miny;
	maxy_ = maxy;
}

void
Function::	setMinZ(double val)
{
	minz_ = val;
}

void
Function::	setMaxZ(double val)
{
	maxz_ = val;
}

bool 
Function::create()
{
	if ((xmesh_<=2) || (ymesh_<=2) || !plotwidget_)
		return false;
	
	/* allocate some space for the mesh */
 	GLdouble** data         = new GLdouble* [xmesh_] ;

	unsigned i,j;
	for ( i = 0; i < xmesh_; i++) 
	{
		data[i]         = new GLdouble [ymesh_];
	}
	
	/* get the data */

	double dx = (maxx_ - minx_) / (xmesh_ - 1);
	double dy = (maxy_ - miny_) / (ymesh_ - 1);
	
	for (i = 0; i < xmesh_; ++i) 
	{
		for (j = 0; j < ymesh_; ++j) 
		{
			data[i][j] = operator()(minx_ + i*dx, miny_ + j*dy);
			
			if (data[i][j] > maxz_)
				data[i][j] = maxz_;
			else if (data[i][j] < minz_)
				data[i][j] = minz_;
		}
	}

	plotwidget_->createInternalRepresentation(data, xmesh_, ymesh_, minx_, maxx_, miny_, maxy_);

	for ( i = 0; i < xmesh_; i++) 
	{
		delete [] data[i];
	}
	return true;
}
