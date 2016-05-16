/*****************************************************
*** Header file for declaration of class clElement3D
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef ELEMENT3D_H
#define ELEMENT3D_H

#include "element.h"
#include "node3d.h"

class clUnstructured3D; // Forward declaration to prevent #include loops

class clElement3D : public clElement
{
private:

protected:
  clUnstructured3D *parentGrid;

public:
  clElement3D(void);
  clElement3D(clUnstructured3D*, const int, const int, const int);
  clElement3D(clUnstructured3D*, const int, const int, const int, const int);
  clElement3D(const clElement3D&);
  virtual ~clElement3D(void);

  void              SetParentGrid(clUnstructured3D*);
  clUnstructured3D* GetParentGrid(void) const;

  int               IsNodeInElement(const clNode3D*) const;

  clElement3D&      Assign(const clElement3D&);
  clElement3D&      operator=(const clElement3D&);
  bool              IsEqual(const clElement3D&) const;
  bool              operator==(const clElement3D&) const;
  bool              operator!=(const clElement3D&) const;
};

#endif
