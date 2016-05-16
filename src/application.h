/***********************************************
*** Declaration of class clApplication
***
*** Header File
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "ac3dobject.h"
#include "geometry.h"
#include "object2d.h"
#include "object3d.h"
#include "surfacegrid.h"
#include "unstructured3d.h"

using namespace std;

/*
** Declaration of the application
*/
class clApplication
{
private:
	vector<clGeometry*>    geometryList;
	vector<clSurfaceGrid*> surfaceGridList;
	clObject3D*            object3d;
	clUnstructured3D*      unstructured3d;

  bool TestUnstructured2D(void);
  bool TestUnstructured3D(void);

  bool CreateBoundingBox(void);
  bool Geometry_SurfaceGrid(int, int);
  bool SurfaceGrid_Unstructured3D(void); // From SurfaceGrids
  bool Object3D_Unstructured3D(void);    // From Object3Ds

  bool InsertNode(clObject2D*, clUnstructured2D*);
  bool InsertVertexList(vector< vector<double> >&, clUnstructured3D*, bool);

  bool EnsureGeometryConformity(clUnstructured2D*);
  bool EnsureGeometryConformity(clUnstructured3D*);
  bool RemoveCavityElements(clObject2D*, clUnstructured2D*);
  bool RemoveCavityElements(clObject3D*, clUnstructured3D*);
  bool RefineGrid(clObject2D*, clUnstructured2D*);
  bool RefineGrid(clObject3D*, clUnstructured3D*);

public:
  clApplication(void);
  ~clApplication(void);

  bool Run(int, char**);
};

#endif

