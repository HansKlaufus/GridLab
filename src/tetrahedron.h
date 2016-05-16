/******************************************************
*** Header file for declaration of class clTetrahedron
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "element3d.h"

class clUnstructured3D;

class clTetrahedron : public clElement3D
{
private:
  clNode3D         *circumCentre;
  clNode3D         *centroid;
  double            radiusCircumsphere;

  void              CalculateArea(void);
  void              CalculateVolume(void);
  void              CalculateCircumCentre(void);
  void              CalculateCentroid(void);
  void              CalculateRadiusCircumsphere(void);

public:
  clTetrahedron(void);
  clTetrahedron(clUnstructured3D*);
  clTetrahedron(clUnstructured3D*, const int);
  clTetrahedron(const clTetrahedron&);
  virtual ~clTetrahedron(void);

  int               GetFaceNode(const int, const int) const;
  void              GetFaceNodes(const int, int*) const;

  bool              InSphereCriterion(const clNode3D*);
  bool              IsValidElement(void);

  double            GetArea(void);
  double            GetVolume(void);
  clNode3D          GetCircumCentre(void);
  clNode3D          GetCentroid(void);
  double            GetRadiusCircumsphere(void);
};

#endif
