/********************************************************
*** Body file for definition of class clBoxCreator
***
*** J.L. Klaufus - Cranfield 1999
**/

#include "box.h"
#include "exception.h"
#include "geometry.h"
#include "matrix.h"
#include "patch.h"
#include "surface.h"

/*
** ---------------
** Private Methods
** ---------------
*/

void clBoxCreator::CreateBox(clGeometry &obj, clGeometry &box)
{
  double xMin = obj.GetMinX();
  double xMax = obj.GetMaxX();
  double yMin = obj.GetMinY();
  double yMax = obj.GetMaxY();
  double zMin = obj.GetMinZ();
  double zMax = obj.GetMaxZ();

  double xDelta = xMax - xMin;
  double yDelta = yMax - yMin;
  double zDelta = zMax - zMin;

  double xDeltaBox = ENLARGE_FACTOR*xDelta;
  double yDeltaBox = ENLARGE_FACTOR*yDelta;
  double zDeltaBox = ENLARGE_FACTOR*zDelta;

  double xCentre = (xMin + xMax)/2.0;
  double yCentre = (yMin + yMax)/2.0;
  double zCentre = (zMin + zMax)/2.0;

  double xMinBox = xCentre - xDeltaBox/2.0;
  double xMaxBox = xCentre + xDeltaBox/2.0;
  double yMinBox = yCentre - yDeltaBox/2.0;
  double yMaxBox = yCentre + yDeltaBox/2.0;
  double zMinBox = zCentre - zDeltaBox/2.0;
  double zMaxBox = zCentre + zDeltaBox/2.0;

  dMatrix   GX(4,4), GY(4,4), GZ(4,4);

  // Surface at xMinBox
  GX.SetElement(1,1,xMinBox); GX.SetElement(1,2, xMinBox); GX.SetElement(1,3,xMinBox); GX.SetElement(1,4,xMinBox);
  GX.SetElement(2,1,xMinBox); GX.SetElement(2,2, xMinBox); GX.SetElement(2,3,xMinBox); GX.SetElement(2,4,xMinBox);
  GX.SetElement(3,1,xMinBox); GX.SetElement(3,2, xMinBox); GX.SetElement(3,3,xMinBox); GX.SetElement(3,4,xMinBox);
  GX.SetElement(4,1,xMinBox); GX.SetElement(4,2, xMinBox); GX.SetElement(4,3,xMinBox); GX.SetElement(4,4,xMinBox);

  GY.SetElement(1,1,yMinBox); GY.SetElement(1,2, yMinBox); GY.SetElement(1,3,yMaxBox); GY.SetElement(1,4,yMaxBox);
  GY.SetElement(2,1,yMinBox); GY.SetElement(2,2, yMinBox); GY.SetElement(2,3,yMaxBox); GY.SetElement(2,4,yMaxBox);
  GY.SetElement(3,1,yMinBox); GY.SetElement(3,2, yMinBox); GY.SetElement(3,3,yMaxBox); GY.SetElement(3,4,yMaxBox);
  GY.SetElement(4,1,yMinBox); GY.SetElement(4,2, yMinBox); GY.SetElement(4,3,yMaxBox); GY.SetElement(4,4,yMaxBox);

  GZ.SetElement(1,1,zMaxBox); GZ.SetElement(1,2, zMaxBox); GZ.SetElement(1,3,zMaxBox); GZ.SetElement(1,4,zMaxBox);
  GZ.SetElement(2,1,zMaxBox); GZ.SetElement(2,2, zMaxBox); GZ.SetElement(2,3,zMaxBox); GZ.SetElement(2,4,zMaxBox);
  GZ.SetElement(3,1,zMinBox); GZ.SetElement(3,2, zMinBox); GZ.SetElement(3,3,zMinBox); GZ.SetElement(3,4,zMinBox);
  GZ.SetElement(4,1,zMinBox); GZ.SetElement(4,2, zMinBox); GZ.SetElement(4,3,zMinBox); GZ.SetElement(4,4,zMinBox);

  clPatch *patch1 = new clPatch;
  patch1->SetNumberID(1);
  patch1->SetBlendType('B');
  patch1->SetGX(GX);
  patch1->SetGY(GY);
  patch1->SetGZ(GZ);

  clSurface *surface1 = new clSurface;
  surface1->AddPatch(patch1);

  // Surface at xMaxBox
  GX.SetElement(1,1,xMaxBox); GX.SetElement(1,2, xMaxBox); GX.SetElement(1,3,xMaxBox); GX.SetElement(1,4,xMaxBox);
  GX.SetElement(2,1,xMaxBox); GX.SetElement(2,2, xMaxBox); GX.SetElement(2,3,xMaxBox); GX.SetElement(2,4,xMaxBox);
  GX.SetElement(3,1,xMaxBox); GX.SetElement(3,2, xMaxBox); GX.SetElement(3,3,xMaxBox); GX.SetElement(3,4,xMaxBox);
  GX.SetElement(4,1,xMaxBox); GX.SetElement(4,2, xMaxBox); GX.SetElement(4,3,xMaxBox); GX.SetElement(4,4,xMaxBox);

  GY.SetElement(1,1,yMinBox); GY.SetElement(1,2, yMinBox); GY.SetElement(1,3,yMaxBox); GY.SetElement(1,4,yMaxBox);
  GY.SetElement(2,1,yMinBox); GY.SetElement(2,2, yMinBox); GY.SetElement(2,3,yMaxBox); GY.SetElement(2,4,yMaxBox);
  GY.SetElement(3,1,yMinBox); GY.SetElement(3,2, yMinBox); GY.SetElement(3,3,yMaxBox); GY.SetElement(3,4,yMaxBox);
  GY.SetElement(4,1,yMinBox); GY.SetElement(4,2, yMinBox); GY.SetElement(4,3,yMaxBox); GY.SetElement(4,4,yMaxBox);

  GZ.SetElement(1,1,zMaxBox); GZ.SetElement(1,2, zMaxBox); GZ.SetElement(1,3,zMaxBox); GZ.SetElement(1,4,zMaxBox);
  GZ.SetElement(2,1,zMaxBox); GZ.SetElement(2,2, zMaxBox); GZ.SetElement(2,3,zMaxBox); GZ.SetElement(2,4,zMaxBox);
  GZ.SetElement(3,1,zMinBox); GZ.SetElement(3,2, zMinBox); GZ.SetElement(3,3,zMinBox); GZ.SetElement(3,4,zMinBox);
  GZ.SetElement(4,1,zMinBox); GZ.SetElement(4,2, zMinBox); GZ.SetElement(4,3,zMinBox); GZ.SetElement(4,4,zMinBox);

  clPatch *patch2 = new clPatch;
  patch2->SetNumberID(2);
  patch2->SetBlendType('B');
  patch2->SetGX(GX);
  patch2->SetGY(GY);
  patch2->SetGZ(GZ);

  clSurface *surface2 = new clSurface;
  surface2->AddPatch(patch2);

  // Surface at yMinBox
  GX.SetElement(1,1,xMinBox); GX.SetElement(1,2, xMinBox); GX.SetElement(1,3,xMaxBox); GX.SetElement(1,4,xMaxBox);
  GX.SetElement(2,1,xMinBox); GX.SetElement(2,2, xMinBox); GX.SetElement(2,3,xMaxBox); GX.SetElement(2,4,xMaxBox);
  GX.SetElement(3,1,xMinBox); GX.SetElement(3,2, xMinBox); GX.SetElement(3,3,xMaxBox); GX.SetElement(3,4,xMaxBox);
  GX.SetElement(4,1,xMinBox); GX.SetElement(4,2, xMinBox); GX.SetElement(4,3,xMaxBox); GX.SetElement(4,4,xMaxBox);

  GY.SetElement(1,1,yMinBox); GY.SetElement(1,2, yMinBox); GY.SetElement(1,3,yMinBox); GY.SetElement(1,4,yMinBox);
  GY.SetElement(2,1,yMinBox); GY.SetElement(2,2, yMinBox); GY.SetElement(2,3,yMinBox); GY.SetElement(2,4,yMinBox);
  GY.SetElement(3,1,yMinBox); GY.SetElement(3,2, yMinBox); GY.SetElement(3,3,yMinBox); GY.SetElement(3,4,yMinBox);
  GY.SetElement(4,1,yMinBox); GY.SetElement(4,2, yMinBox); GY.SetElement(4,3,yMinBox); GY.SetElement(4,4,yMinBox);

  GZ.SetElement(1,1,zMaxBox); GZ.SetElement(1,2, zMaxBox); GZ.SetElement(1,3,zMaxBox); GZ.SetElement(1,4,zMaxBox);
  GZ.SetElement(2,1,zMaxBox); GZ.SetElement(2,2, zMaxBox); GZ.SetElement(2,3,zMaxBox); GZ.SetElement(2,4,zMaxBox);
  GZ.SetElement(3,1,zMinBox); GZ.SetElement(3,2, zMinBox); GZ.SetElement(3,3,zMinBox); GZ.SetElement(3,4,zMinBox);
  GZ.SetElement(4,1,zMinBox); GZ.SetElement(4,2, zMinBox); GZ.SetElement(4,3,zMinBox); GZ.SetElement(4,4,zMinBox);

  clPatch *patch3 = new clPatch;
  patch3->SetNumberID(3);
  patch3->SetBlendType('B');
  patch3->SetGX(GX);
  patch3->SetGY(GY);
  patch3->SetGZ(GZ);

  clSurface *surface3 = new clSurface;
  surface3->AddPatch(patch3);

  // Surface at yMaxBox
  GX.SetElement(1,1,xMinBox); GX.SetElement(1,2, xMinBox); GX.SetElement(1,3,xMaxBox); GX.SetElement(1,4,xMaxBox);
  GX.SetElement(2,1,xMinBox); GX.SetElement(2,2, xMinBox); GX.SetElement(2,3,xMaxBox); GX.SetElement(2,4,xMaxBox);
  GX.SetElement(3,1,xMinBox); GX.SetElement(3,2, xMinBox); GX.SetElement(3,3,xMaxBox); GX.SetElement(3,4,xMaxBox);
  GX.SetElement(4,1,xMinBox); GX.SetElement(4,2, xMinBox); GX.SetElement(4,3,xMaxBox); GX.SetElement(4,4,xMaxBox);

  GY.SetElement(1,1,yMaxBox); GY.SetElement(1,2, yMaxBox); GY.SetElement(1,3,yMaxBox); GY.SetElement(1,4,yMaxBox);
  GY.SetElement(2,1,yMaxBox); GY.SetElement(2,2, yMaxBox); GY.SetElement(2,3,yMaxBox); GY.SetElement(2,4,yMaxBox);
  GY.SetElement(3,1,yMaxBox); GY.SetElement(3,2, yMaxBox); GY.SetElement(3,3,yMaxBox); GY.SetElement(3,4,yMaxBox);
  GY.SetElement(4,1,yMaxBox); GY.SetElement(4,2, yMaxBox); GY.SetElement(4,3,yMaxBox); GY.SetElement(4,4,yMaxBox);

  GZ.SetElement(1,1,zMaxBox); GZ.SetElement(1,2, zMaxBox); GZ.SetElement(1,3,zMaxBox); GZ.SetElement(1,4,zMaxBox);
  GZ.SetElement(2,1,zMaxBox); GZ.SetElement(2,2, zMaxBox); GZ.SetElement(2,3,zMaxBox); GZ.SetElement(2,4,zMaxBox);
  GZ.SetElement(3,1,zMinBox); GZ.SetElement(3,2, zMinBox); GZ.SetElement(3,3,zMinBox); GZ.SetElement(3,4,zMinBox);
  GZ.SetElement(4,1,zMinBox); GZ.SetElement(4,2, zMinBox); GZ.SetElement(4,3,zMinBox); GZ.SetElement(4,4,zMinBox);

  clPatch *patch4 = new clPatch;
  patch4->SetNumberID(4);
  patch4->SetBlendType('B');
  patch4->SetGX(GX);
  patch4->SetGY(GY);
  patch4->SetGZ(GZ);

  clSurface *surface4 = new clSurface;
  surface4->AddPatch(patch4);

  // Surface at zMinBox
  GX.SetElement(1,1,xMinBox); GX.SetElement(1,2, xMinBox); GX.SetElement(1,3,xMaxBox); GX.SetElement(1,4,xMaxBox);
  GX.SetElement(2,1,xMinBox); GX.SetElement(2,2, xMinBox); GX.SetElement(2,3,xMaxBox); GX.SetElement(2,4,xMaxBox);
  GX.SetElement(3,1,xMinBox); GX.SetElement(3,2, xMinBox); GX.SetElement(3,3,xMaxBox); GX.SetElement(3,4,xMaxBox);
  GX.SetElement(4,1,xMinBox); GX.SetElement(4,2, xMinBox); GX.SetElement(4,3,xMaxBox); GX.SetElement(4,4,xMaxBox);

  GY.SetElement(1,1,yMaxBox); GY.SetElement(1,2, yMaxBox); GY.SetElement(1,3,yMaxBox); GY.SetElement(1,4,yMaxBox);
  GY.SetElement(2,1,yMaxBox); GY.SetElement(2,2, yMaxBox); GY.SetElement(2,3,yMaxBox); GY.SetElement(2,4,yMaxBox);
  GY.SetElement(3,1,yMinBox); GY.SetElement(3,2, yMinBox); GY.SetElement(3,3,yMinBox); GY.SetElement(3,4,yMinBox);
  GY.SetElement(4,1,yMinBox); GY.SetElement(4,2, yMinBox); GY.SetElement(4,3,yMinBox); GY.SetElement(4,4,yMinBox);

  GZ.SetElement(1,1,zMinBox); GZ.SetElement(1,2, zMinBox); GZ.SetElement(1,3,zMinBox); GZ.SetElement(1,4,zMinBox);
  GZ.SetElement(2,1,zMinBox); GZ.SetElement(2,2, zMinBox); GZ.SetElement(2,3,zMinBox); GZ.SetElement(2,4,zMinBox);
  GZ.SetElement(3,1,zMinBox); GZ.SetElement(3,2, zMinBox); GZ.SetElement(3,3,zMinBox); GZ.SetElement(3,4,zMinBox);
  GZ.SetElement(4,1,zMinBox); GZ.SetElement(4,2, zMinBox); GZ.SetElement(4,3,zMinBox); GZ.SetElement(4,4,zMinBox);

  clPatch *patch5 = new clPatch;
  patch5->SetNumberID(5);
  patch5->SetBlendType('B');
  patch5->SetGX(GX);
  patch5->SetGY(GY);
  patch5->SetGZ(GZ);

  clSurface *surface5 = new clSurface;
  surface5->AddPatch(patch5);

  // Surface at zMaxBox
  GX.SetElement(1,1,xMinBox); GX.SetElement(1,2, xMinBox); GX.SetElement(1,3,xMaxBox); GX.SetElement(1,4,xMaxBox);
  GX.SetElement(2,1,xMinBox); GX.SetElement(2,2, xMinBox); GX.SetElement(2,3,xMaxBox); GX.SetElement(2,4,xMaxBox);
  GX.SetElement(3,1,xMinBox); GX.SetElement(3,2, xMinBox); GX.SetElement(3,3,xMaxBox); GX.SetElement(3,4,xMaxBox);
  GX.SetElement(4,1,xMinBox); GX.SetElement(4,2, xMinBox); GX.SetElement(4,3,xMaxBox); GX.SetElement(4,4,xMaxBox);

  GY.SetElement(1,1,yMaxBox); GY.SetElement(1,2, yMaxBox); GY.SetElement(1,3,yMaxBox); GY.SetElement(1,4,yMaxBox);
  GY.SetElement(2,1,yMaxBox); GY.SetElement(2,2, yMaxBox); GY.SetElement(2,3,yMaxBox); GY.SetElement(2,4,yMaxBox);
  GY.SetElement(3,1,yMinBox); GY.SetElement(3,2, yMinBox); GY.SetElement(3,3,yMinBox); GY.SetElement(3,4,yMinBox);
  GY.SetElement(4,1,yMinBox); GY.SetElement(4,2, yMinBox); GY.SetElement(4,3,yMinBox); GY.SetElement(4,4,yMinBox);

  GZ.SetElement(1,1,zMaxBox); GZ.SetElement(1,2, zMaxBox); GZ.SetElement(1,3,zMaxBox); GZ.SetElement(1,4,zMaxBox);
  GZ.SetElement(2,1,zMaxBox); GZ.SetElement(2,2, zMaxBox); GZ.SetElement(2,3,zMaxBox); GZ.SetElement(2,4,zMaxBox);
  GZ.SetElement(3,1,zMaxBox); GZ.SetElement(3,2, zMaxBox); GZ.SetElement(3,3,zMaxBox); GZ.SetElement(3,4,zMaxBox);
  GZ.SetElement(4,1,zMaxBox); GZ.SetElement(4,2, zMaxBox); GZ.SetElement(4,3,zMaxBox); GZ.SetElement(4,4,zMaxBox);

  clPatch *patch6 = new clPatch;
  patch6->SetNumberID(6);
  patch6->SetBlendType('B');
  patch6->SetGX(GX);
  patch6->SetGY(GY);
  patch6->SetGZ(GZ);

  clSurface *surface6 = new clSurface;
  surface6->AddPatch(patch6);

  // Add surfaces to geometry
  box.AddSurface(surface1);
  box.AddSurface(surface2);
  box.AddSurface(surface3);
  box.AddSurface(surface4);
  box.AddSurface(surface5);
  box.AddSurface(surface6);
}

void clBoxCreator::CreateCType(const clGeometry &obj, clGeometry &box)
{
  throw clException("clBoxCreator", "CreateCType", "Sorry; not implemented yet.");
}

void clBoxCreator::CreateOType(const clGeometry &obj, clGeometry &box)
{
  throw clException("clBoxCreator", "CreateOType", "Sorry; not implemented yet.");
}

/*
** --------------
** Public Methods
** --------------
*/

/*
** Constructor and destructor
*/
clBoxCreator::clBoxCreator(clGeometry &obj, clGeometry &box, const enBoxType bType = btBox, const enLimitingAxis lAxis = laNone, const enHeading head = hdNone)
{
  boxType      = bType;
  limitingAxis = lAxis;
  heading      = head;

  switch (boxType)
  {
    case btBox:
      CreateBox(obj, box);
      break;
    case btCType:
      CreateCType(obj, box);
      break;
    case btOType:
      CreateOType(obj, box);
      break;
    default:
      throw clException("clBoxCreator", "clBoxCreator", "Undefined bounding box type.");
      break;
  }
}

clBoxCreator::~clBoxCreator(void)
{
  //cerr << "DESTROYING BOXCREATOR" << endl;
}

