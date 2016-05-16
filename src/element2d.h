/*****************************************************
*** Header file for declaration of class clElement2D
***
*** J.L. Klaufus - Haarlem 2009
**/

#ifndef ELEMENT2D_H
#define ELEMENT2D_H

#include "element.h"
#include "node2d.h"

class clUnstructured2D; // Forward declaration to prevent #include loops

class clElement2D : public clElement
{
private:

protected:
  // NOTE: A reference to the parentGrid is required for access to the nodeList; required for IsNodeInElement, Area calculation, etc.
  clUnstructured2D *parentGrid;

public:
  clElement2D(void);
  clElement2D(clUnstructured2D*, const int, const int, const int);
  clElement2D(clUnstructured2D*, const int, const int, const int, const int);
  clElement2D(const clElement2D&);
  virtual ~clElement2D(void);

  void              SetParentGrid(clUnstructured2D*);
  clUnstructured2D* GetParentGrid(void) const;

  int               IsNodeInElement(const clNode2D*) const;

  clElement2D&      Assign(const clElement2D&);
  clElement2D&      operator=(const clElement2D&);
  bool              IsEqual(const clElement2D&) const;
  bool              operator==(const clElement2D&) const;
  bool              operator!=(const clElement2D&) const;
};

#endif
