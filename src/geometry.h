/******************************************************
*** Header file for declaration of class clGeometry
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>

#include "list.h"
#include "surface.h"

using namespace std;

class clGeometry
{
private:
  string     name;
  int        numberID;
  int        initialised;

  vector<clSurface*> surfaceList;

  double     xMin, xMax;
  double     yMin, yMax;
  double     zMin, zMax;

  int        SetNumberID(const int);
  void       CalculateExtremes(void);
  void       Initialise(void);

public:
  clGeometry(void);
  clGeometry(const int, const string = "noName");
  clGeometry(const clGeometry&);
  ~clGeometry(void);

  void        SetName(const string);
  string      GetName(void);

  int         GetNumberID(void) const;
  int         GetNumberSurfaces(void) const;

  int         GetSurfaceIndex(const int) const;

  int         AddSurface(clSurface*);
  int         DeleteSurfaceByID(const int);
  int         DeleteSurfaceByIndex(const int);
  clSurface   GetSurface(const int) const;
  clSurface*  GetSurfacePointer(const int) const;

  double      GetMinX(void);
  double      GetMaxX(void);
  double      GetMinY(void);
  double      GetMaxY(void);
  double      GetMinZ(void);
  double      GetMaxZ(void);

  clGeometry& Assign(const clGeometry&);
  clGeometry& operator=(const clGeometry&);
};

#endif
