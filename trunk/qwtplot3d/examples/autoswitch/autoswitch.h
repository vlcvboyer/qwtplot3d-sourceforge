#ifndef AUTOSWITCH_H__
#define AUTOSWITCH_H__

#include <qtimer.h>
#include <qwt_plot3d.h>
#include "../src/functiongenerator.h"

using namespace Qwt3D;

class Saddle : public Function
{
public:

	Saddle(Plot3D* pw)
	:Function(pw)
	{
		setDomain(-2.5,2.5,-2.5,2.5);
		setMaxZ(1.5);
		setMinZ(-1.5);
		setMesh(31,31);
	}

	double operator()(double x, double y)
	{
		return x*y;
	}
};

class Hat : public Function
{
public:

	Hat(Plot3D* pw)
	:Function(pw)
	{
		setDomain(-1.5,1.5,-1.5,1.5);
		setMesh(41,41);
	}

	double operator()(double x, double y)
	{
		return 1.0 / (x*x+y*y+0.3);
	}
};


class Plot : public Plot3D
{
	Q_OBJECT

public:
    Plot(QWidget* pw, int updateinterval);

public slots:
	void rotate();

};

#endif