#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

#include "qwt3d_plot.h"

using namespace std;
using namespace Qwt3D;

/*!
  Update OpenGL data representation
*/
void 
Plot3D::updateData()
{
	GLStateBewarer dt(GL_DEPTH_TEST, true);
	GLStateBewarer ls(GL_LINE_SMOOTH, true);

	calculateHull();
	updateFloorData();
	

	SaveGlDeleteLists(DisplayLists[DataObject], 1); // nur Daten
	
	if (plotStyle() == NOPLOT)
		return;

	DisplayLists[DataObject] = glGenLists(1);
	glNewList(DisplayLists[DataObject], GL_COMPILE);

	this->createData();
		
	glEndList();
}



/*!
  Update OpenGL representation of floor data
*/
void 
Plot3D::updateFloorData()
{
	SaveGlDeleteLists(DisplayLists[FloorObject], 1);
	
	if (floorStyle() == NOFLOOR)
		return;
		
	DisplayLists[FloorObject] = glGenLists(1);
	glNewList(DisplayLists[FloorObject], GL_COMPILE);

	this->createFloorData();

	glEndList();
}

