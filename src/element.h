/*****************************************************
*** Header file for declaration of class clElement
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>

#include "globals.h"

using namespace std;

class clElement
{
private:
  vector<int>   nodeIDArray;
  vector<int>   neighbourIDArray;

  enObjectType  objectType;
  int           elementID;
  bool          isInitialised;
  bool          isHaloCell;
  bool          isBoundaryCell;

  int           numberNodes;
  int           numberFaces;
  int           numberFaceNodes;

  double        area;
  double        volume;
  double        minimumAngle;

protected:
  virtual void  SetObjectType(enObjectType type) {objectType = type;};
//  int           SetElementID(const int);

  int           SetNumberNodes(const int);
  int           SetNumberFaces(const int);
  int           SetNumberFaceNodes(const int);
  double        SetArea(const double);
  double        SetVolume(const double);
  double        SetMinimumAngle(const double);

public:
  clElement(void);
  clElement(const int, const int, const int);
  clElement(const int, const int, const int, const int);
  clElement(const clElement&);
  virtual ~clElement(void);

  enObjectType      GetObjectType(void) {return (objectType);};
  int               SetElementID(const int);
  int               GetElementID(void) const;

  int               GetNumberNodes(void) const;
  int               GetNumberFaces(void) const;
  int               GetNumberFaceNodes(void) const;
  virtual void      GetFaceNodes(const int, int*) const;

  int               GetNodeID(const int) const;
  int               GetNeighbourID(const int) const;

  int               SetNodeID(const int, const int);
  int               SetNeighbourID(const int, const int);

  void              DeleteNodeRef(const int);
  void              DeleteNeighbourRef(const int);

  int               GetNodeIndex(const int) const;
  int               GetFaceIndex(const int) const;
  int               FindFaceIndex(const int) const;

  bool              IsHaloCell(void) const;
  void              SetHaloCell(void);
  void              ResetHaloCell(void);

  bool              IsBoundaryCell(void) const;
  void              SetBoundaryCell(void);
  void              ResetBoundaryCell(void);

  bool              IsInitialised(void) const;
  void              SetInitialised(void);
  void              ResetInitialised(void);

  bool              IsElementNode(const int) const;
  bool              IsInferiorElement(void) const;

  virtual double    GetArea(void);
  virtual double    GetVolume(void);
  virtual double    GetMinimumAngle(void);

  string            GetElementDescription(void) const;

  clElement&        Assign(const clElement&);
  clElement&        operator=(const clElement&);
  bool              IsEqual(const clElement&) const;
  bool              operator==(const clElement&) const;
  bool              operator!=(const clElement&) const;
};

#endif
