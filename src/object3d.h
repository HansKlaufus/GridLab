/******************************************************
*** Header file for declaration of class clObject3D
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <string>
#include <vector>

#include "ac3dobject.h"
#include "object.h"

using namespace std;

class clObject3D : public clObject
{
private:
  bool           PointInPolygon(int, double, double, double);
  bool           PointInPolyhedron(int, double, double, double);
  int            IsEncroachingPolygon(int, double, double, double);
  bool           IsEncroachingPolyhedron(int, double&, double&, double&, bool);
  void           SplitEdge(int, double&, double&, double&);

  void           ParseO3DFile(const char*);
  void           ParseSTLFile(const char*);

public:
  clObject3D(void);
  clObject3D(const int, const string = "noName");
  clObject3D(const clObject3D&);
  clObject3D(const clAc3dObject*);
  ~clObject3D(void);

  bool           PointInObject(double, double, double);
  bool           IsEncroachingObject(double&, double&, double&, bool);

  clObject3D&    Assign(const clObject3D&);
  clObject3D&    Assign(const clAc3dObject*);

  clObject3D&    operator=(const clObject3D&);
  clObject3D&    operator=(const clAc3dObject&);

  void           ReadO3DFile(const string);
  void           ReadO3DFile(const char*);

  void           ReadSTLFile(const string);
  void           ReadSTLFile(const char*);
};

#endif
