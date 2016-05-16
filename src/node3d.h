/*****************************************************
*** Header file for declaration of class clNode3D
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef NODE3D_H
#define NODE3D_H

#include "list.h"
#include "matrix.h"
#include "node.h"

class clNode3D: public clNode
{
private:

public:
  clNode3D(void);
  clNode3D(const int, const dMatrix&);
  clNode3D(const int, const double, const double, const double);
  clNode3D(const clNode3D&);
  virtual ~clNode3D(void);

  using     clNode::SetCoordinates; // Prevent name hiding
  dMatrix   SetCoordinates(const double, const double, const double);

  double    x(void) const;
  double    y(void) const;
  double    z(void) const;

  clNode3D& Assign(const clNode3D&);
  clNode3D& operator=(const clNode3D&);
};

#endif

