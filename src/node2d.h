/*****************************************************
*** Header file for declaration of class clNode2D
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef NODE2D_H
#define NODE2D_H

#include "list.h"
#include "matrix.h"
#include "node.h"

class clNode2D : public clNode
{
private:

public:
  clNode2D(void);
  clNode2D(const int, const double, const double);
  clNode2D(const int, const dMatrix&);
  clNode2D(const clNode2D&);
  virtual ~clNode2D(void);

  using     clNode::SetCoordinates; // Prevent name hiding
  dMatrix   SetCoordinates(const double, const double);

  double    x(void) const;
  double    y(void) const;

  clNode2D& Assign(const clNode2D&);
  clNode2D& operator=(const clNode2D&);
};

#endif

