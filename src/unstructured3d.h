/*********************************************************
*** Header file for declaration of class clUnstructured3D
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef UNSTRUCTURED3D_H
#define UNSTRUCTURED3D_H

#include "node3d.h"
#include "tetrahedron.h"
#include "unstructured.h"

class clUnstructured3D : public tpUnstructured<clNode3D, clTetrahedron>
{
private:
  friend class          clTetrahedron;
  friend class          clNode3D;

  using                 tpUnstructured<clNode3D, clTetrahedron>::AddNode;
  int                   AddNode(const double*);
  int                   AddNode(const double, const double, const double);

  using                 tpUnstructured<clNode3D, clTetrahedron>::AddElement;
  int                   AddElement(const int*, const int*);
  int                   AddElement(const int, const int, const int, const int, const int, const int, const int, const int);

public:
  clUnstructured3D(void);
  clUnstructured3D(double, double, double, double, double, double);
  clUnstructured3D(const int);
  clUnstructured3D(const clUnstructured3D&);
  ~clUnstructured3D(void);

  clTetrahedron        *CreateElement(void);
  int                   CreateInitialGrid(void);
  int                   CreateInitialGrid(double, double, double, double, double, double);

  void                  WriteGnuplotGrid(void);
  void                  WriteGnuplotGrid(const string);
  void                  WriteGnuplotGrid(const char*);

  void                  WriteLegacyVTKGrid(void);
  void                  WriteLegacyVTKGrid(const string);
  void                  WriteLegacyVTKGrid(const char*);
};

#endif
