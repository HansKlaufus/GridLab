/******************************************************
*** Header file for declaration of class clObject2D
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef OBJECT2D_H
#define OBJECT2D_H

#include <string>
#include <vector>

#include "object.h"

using namespace std;

class clObject2D : public clObject
{
private:
  bool           PointInPolygon(int, double, double);
  int            IsEncroachingPolygon(int, double, double);
  void           SplitEdge(int, double&, double&);

  void           ParseO2DFile(const char*);

public:
  clObject2D(void);
  clObject2D(const int, const string = "noName");
  clObject2D(const clObject2D&);
  ~clObject2D(void);

  bool           PointInObject(double, double);
  bool           IsEncroachingObject(double&, double&, bool);

  clObject2D&    Assign(const clObject2D&);
  clObject2D&    operator=(const clObject2D&);

  void           ReadO2DFile(const string);
  void           ReadO2DFile(const char*);
};

#endif
