#include <qapplication.h>
#include <qsplitter.h>
#include <qtimer.h>

#include "autoswitch.h"

using namespace Qwt3D;

//--------------------------------------------------------------------
//              autoswitch.cpp
//
//      Demonstrates autoswitching axes with a cutted saddle as data
//--------------------------------------------------------------------

Plot::Plot(QWidget* pw, int updateinterval)
:SurfacePlot(pw)
{
	setRotation(30,0,15);
	setScale(1,1,1);
	setShift(0.1,0,0);
	setZoom(0.8);
	coordinates()->setNumberFont("Courier",8);

	for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
	{
		coordinates()->axes[i].setMajors(7);
		coordinates()->axes[i].setMinors(4);
	}
	
	
	coordinates()->axes[Qwt3D::X1].setLabelString("x");
	coordinates()->axes[Qwt3D::Y1].setLabelString("y");
	coordinates()->axes[Qwt3D::Z1].setLabelString("z"); 
	coordinates()->axes[Qwt3D::X2].setLabelString("x");
	coordinates()->axes[Qwt3D::Y2].setLabelString("y");
	coordinates()->axes[Qwt3D::Z2].setLabelString("z"); 
	coordinates()->axes[Qwt3D::X3].setLabelString("x");
	coordinates()->axes[Qwt3D::Y3].setLabelString("y");
	coordinates()->axes[Qwt3D::Z3].setLabelString("z"); 
	coordinates()->axes[Qwt3D::X4].setLabelString("x");
	coordinates()->axes[Qwt3D::Y4].setLabelString("y");
	coordinates()->axes[Qwt3D::Z4].setLabelString("z"); 


  QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(rotate()) );

	timer->start(updateinterval);

}

void Plot::rotate()
{
	int prec = 3;
		
	setRotation(
			(int(prec*xRotation() + 2) % (360*prec))/double(prec),
			(int(prec*yRotation() + 2) % (360*prec))/double(prec),
			(int(prec*zRotation() + 2) % (360*prec))/double(prec)
			);
}


using namespace Qwt3D;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
		QSplitter* spl = new QSplitter(QSplitter::Horizontal);

		Plot* plot1 = new Plot(spl,30);
		plot1->setFloorStyle(FLOORISO);
		plot1->setCoordinateStyle(BOX);
		Saddle saddle(plot1);
		saddle.create();
		plot1->setTitle("Autoswitching axes");
		plot1->setBackgroundColor(RGBA(1,1, 157./255));
		plot1->makeCurrent();
		plot1->updateData();
		plot1->updateGL();


		Plot* plot2 = new Plot(spl,80);
		plot2->setZoom(0.8);
		Hat hat(plot2);
		hat.create();
		plot2->setPlotStyle(HIDDENLINE);
		plot2->setFloorStyle(FLOORDATA);
		plot2->setCoordinateStyle(FRAME);
		plot2->setBackgroundColor(RGBA(1,1, 157./255));
		plot2->makeCurrent();
		plot2->updateData();
		plot2->updateGL();


		a.setMainWidget(spl);
		spl->resize(800,400);
    spl->show();
    return a.exec(); 
}
