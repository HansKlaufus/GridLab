/*********************************************************
*** Header file for declaration of class clUnstructured2D
***
*** J.L. Klaufus - Haarlem 2009
**/

#ifndef UNSTRUCTURED2D_H
#define UNSTRUCTURED2D_H

#include "node2d.h"
#include "triangle.h"
#include "unstructured.h"

class clUnstructured2D : public tpUnstructured<clNode2D, clTriangle>
{
private:
  friend class          clTriangle;
  friend class          clNode2D;

  using                 tpUnstructured<clNode2D, clTriangle>::AddNode;
  int                   AddNode(const double*);
  int                   AddNode(const double, const double);

  using                 tpUnstructured<clNode2D, clTriangle>::AddElement;
  int                   AddElement(const int, const int*, const int, const int*);

  void                  ComposeNeighbourNodeList(tpList<int>*, tpList<int>*);
  void                  BalanceNodes(tpList<int>*, tpList<int>*);

  void                  CorrectInCircleCriterion(void);

  bool                  CheckElementAreas(void);
  bool                  CheckInCircleCriterion(void);
  bool                  CheckTotalArea(void);

public:
  clUnstructured2D(void);
  clUnstructured2D(double, double, double, double);
  clUnstructured2D(const int);
  clUnstructured2D(const clUnstructured2D&);
  ~clUnstructured2D(void);

  clTriangle           *CreateElement(void);
  int                   CreateInitialGrid(void);
  int                   CreateInitialGrid(double, double, double, double);

  bool                  CheckGrid(void);
  void                  Smooth(void);

  void                  WriteLegacyVTKGrid(void);
  void                  WriteLegacyVTKGrid(const string);
  void                  WriteLegacyVTKGrid(const char*);
};

#endif
