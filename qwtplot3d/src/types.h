#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4786 )
#endif

#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#ifdef _DEBUG
	#include <fstream.h>
#endif

#include <string>

#ifdef _WIN32
	#include <windows.h>
#endif

#ifndef WHEEL_DELTA
	#define WHEEL_DELTA 120
#endif

#include "helper.h"
#include "openglhelper.h"


namespace Qwt3D
{

enum PLOTSTYLE
{
	NOPLOT,
	WIREFRAME,
	HIDDENLINE,
	FILLED,
	FILLEDMESH
};

enum COORDSTYLE
{
	NOCOORD,
	BOX,
	FRAME		
};

enum FLOORSTYLE
{
	NOFLOOR,
	FLOORMESH,
	FLOORISO,
	FLOORDATA,
};

//! The 12 axes
enum AXIS
{
	X1 = 0,
	X2 = 3,
	X3 = 4,
	X4 = 5,
	Y1 = 1,
	Y2 = 8,
	Y3 = 7,
	Y4 = 6,
	Z1 = 2,
	Z2 = 9,
	Z3 = 11,
	Z4 = 10
};

//! Tuple <tt>[x,y]</tt>
struct Tuple
{
	Tuple() : x(0), y(0) {}
	Tuple(double X, double Y) : x(X), y(Y) {}
	double x,y;
};

//! Triple <tt>[x,y,z]</tt>
struct Triple
{
	explicit Triple(double xv = 0,double yv = 0,double zv = 0)
		: x(xv), y(yv), z(zv)
	{
	}
	
	double x,y,z;

	Triple& operator+=(Triple t)
	{
		x += t.x;
		y += t.y;
		z += t.z;

		return *this;
	}
	
	Triple& operator-=(Triple t)
	{
		x -= t.x;
		y -= t.y;
		z -= t.z;

		return *this;
	}
	Triple& operator*=(double d)
	{
		x *= d;
		y *= d;
		z *= d;

		return *this;
	}
	Triple& operator/=(double d)
	{
		x /= d;
		y /= d;
		z /= d;

		return *this;
	}
	Triple& operator*=(Triple t) // scale
	{
		x *= t.x;
		y *= t.y;
		z *= t.z;

		return *this;
	}

	bool operator!=(Triple t) const
	{
		return !isPracticallyZero(x,t.x) || !isPracticallyZero(y,t.y) || !isPracticallyZero(z,t.z);
	}
	
	bool operator==(Triple t) const
	{
		return !operator!=(t);
	}

	double length() const
	{
		double l2 = x*x + y*y + z*z;
		return (isPracticallyZero(l2)) ? 0 :sqrt(l2);
	}
	
	void normalize()
	{
		double l = length();
		if (l)
			*this /= l;
	}
};

inline const Triple operator+(const Triple& t, const Triple& t2)
{
	return Triple(t) += t2;
}
inline const Triple operator-(const Triple& t, const Triple& t2)
{
	return Triple(t) -= t2;
}
inline const Triple operator*(double d, const Triple& t)
{
	return Triple(t) *= d;
}
inline const Triple operator*(const Triple& t, double d)
{
	return Triple(t) *= d;
}
inline const Triple operator/(double d, const Triple& t)
{
	return Triple(t) /= d;
}
inline const Triple operator/(const Triple& t, double d)
{
	return Triple(t) /= d;
}
inline const Triple operator*(const Triple& t, const Triple& t2)
{
	return Triple(t) *= t2;
}

struct ParallelEpiped
{
	ParallelEpiped()
	{
	}

	ParallelEpiped(Triple minv, Triple maxv)
	: minVertex(minv), maxVertex(maxv)
	{
	}
	
	Triple minVertex;
	Triple maxVertex;
};


typedef double *Vertex;
typedef std::vector<Vertex> DataRow;
typedef std::vector<DataRow> DataMatrix;

//! Implements a matrix of z-Values with limit access functions 
class GridData
{
public:
	
	GridData();
	GridData(unsigned int columns, unsigned int rows);//!< see setSize()
	~GridData();

	int columns() const;
	int rows() const;

	void clear(); //!< destroy content
	bool empty() const { return vertices.empty();}
	void setSize(unsigned int columns, unsigned int rows); //!< destroys content and set new size, elements are uninitialized
	
	double maximum() const { return hull().maxVertex.z;} //!< \return minimal z value
	double minimum() const { return hull().minVertex.z;} //!< \return maximal z value 

	DataMatrix vertices;		//!< mesh vertices
	DataMatrix normals;		//!< mesh normals

	ParallelEpiped hull() const;
	void setHull(ParallelEpiped h) { hull_ = h; }

private:

	GridData(GridData const&);						//!< no copy
	GridData& operator=(GridData const&); //!< no copy

	ParallelEpiped hull_;
};


typedef std::vector<Triple> TripleVector;
//! Helds indexes in a TripleVector interpreted as node numbers for a convex polygon
typedef std::vector<unsigned> Cell;
//! Vector of convex polygons. You need a TripleVector as base for the node data
typedef std::vector<Cell> Tesselation;
//! returns the sum over the sizes of the single cells
unsigned tesselationSize(Qwt3D::Tesselation const& t);

//! Implements a graph-like cell structure with limit access functions 
class CellData
{
public:

	CellData() {}

	void clear(); //!< destroy content
	bool empty() const { return cells.empty();}
	
	double maximum() const { return hull().maxVertex.z;} //!< \return minimal z value
	double minimum() const { return hull().minVertex.z;} //!< \return maximal z value 

	Triple const& operator()(unsigned cellnumber, unsigned vertexnumber);
	
	//	ParallelEpiped hull() const;

	Tesselation cells;   //!< polygon/cell mesh 
	TripleVector    nodes;
	TripleVector    normals; //!< mesh normals

	ParallelEpiped hull() const;
	void setHull(ParallelEpiped h) { hull_ = h; }

private:

	CellData(CellData const&);						//!< no copy
	CellData& operator=(CellData const&); //!< no copy

	ParallelEpiped hull_;
};


// colors

//! Red-Green-Blue-Alpha value
struct RGBA
{
	RGBA() 
		{}
	explicit RGBA(double rr, double gg, double bb, double aa = 1)
		: r(rr), g(gg), b(bb), a(aa)
		{}
	double r,g,b,a;
};

typedef std::vector<RGBA> ColorVector;


QColor GL2Qt(GLdouble r, GLdouble g, GLdouble b); //!< RGB -> QColor
Qwt3D::RGBA Qt2GL(QColor col); //!< QColor -> RGBA


//! simplified glut routine (glProject): object coord --> windows coord 
inline Triple World2ViewPort(Triple obj,	bool* err = 0)
{
  Triple win;

  GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,	modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,	projMatrix);
	
	int res = gluProject(obj.x, obj.y, obj.z, modelMatrix, projMatrix, viewport, &win.x, &win.y, &win.z);
	if (err)
		*err = (res) ? false : true;
	return win;
}

//! simplified glut routine (glUnProject): windows coord --> object coord 
inline Triple ViewPort2World(Triple win, bool* err = 0)
{
  Triple obj;
	
	GLdouble modelMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,	modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,	projMatrix);
	
	int res = gluUnProject(win.x, win.y, win.z, modelMatrix, projMatrix, viewport, &obj.x, &obj.y, &obj.z);

	if (err)
		*err = (res) ? false : true;
	return obj;
}

inline void normalizedcross(GLdouble* u, GLdouble* v, GLdouble* n)
{
  GLdouble l;

  /* compute the cross product (u x v for right-handed [ccw]) */
  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0];

  /* normalize */
  l = (GLdouble)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
  if (l)
	{
		n[0] /= l;
		n[1] /= l;
		n[2] /= l;
	}
	else
	{
		n[0] = 0;
		n[1] = 0;
		n[2] = 0;
	}
}

inline Triple normalizedcross(Triple const& u, Triple const& v)
{
	Triple n;

  /* compute the cross product (u x v for right-handed [ccw]) */
  n.x = u.y * v.z - u.z * v.y;
  n.y = u.z * v.x - u.x * v.z;
  n.z = u.x * v.y - u.y * v.x;

  /* normalize */
  double l = n.length();
  if (l)
	{
		n /= l;
	}
	else
	{
		n = Triple(0,0,0);
	}
	
	return n;
}

inline double dotProduct(Triple const& u, Triple const& v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

void convexhull2d( std::vector<int>& idx, const std::vector<Qwt3D::Tuple>& src );

} // ns

#endif