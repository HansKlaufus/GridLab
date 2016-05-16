/******************************************************
*** Header file for declaration of class clTriangle
***
*** J.L. Klaufus - Haarlem 2009
**/

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "element2d.h"

//TODO: Make clTriangle a template class that also can deal with clNode3D

class clTriangle : public clElement2D
{
private:
  clNode2D         *circumCentre;
  clNode2D         *centroid;
  double            radiusCircumcircle;

  void              CalculateArea(void);
  void              CalculateVolume(void);
  void              CalculateMinimumAngle(void);
  void              CalculateCircumCentre(void);
  void              CalculateCentroid(void);
  void              CalculateRadiusCircumsphere(void);

public:
  clTriangle(void);
  clTriangle(clUnstructured2D*);
  clTriangle(clUnstructured2D*, const int);
  clTriangle(const clTriangle&);
  ~clTriangle(void);

  int               GetFaceNode(const int, const int) const;
  void              GetFaceNodes(const int, int*) const;
  int               GetNeighbourNode(const int, const int) const;
  void              GetNeighbourNodes(const int, int*) const;

  bool              InCircleCriterion(const clNode2D*);
  bool              InSphereCriterion(const clNode2D *node) {return InCircleCriterion(node);};
  bool              IsValidElement(void);

  double            GetArea(void);
  double            GetVolume(void);
  double            GetMinimumAngle(void);
  clNode2D          GetCircumCentre(void);
  clNode2D          GetCentroid(void);
  double            GetRadiusCircumcircle(void);
};

#endif
