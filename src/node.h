/*****************************************************
*** Header file for declaration of class clNode
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef NODE_H
#define NODE_H

#include <vector>

#include "globals.h"
#include "matrix.h"

typedef tpMatrix<double> dMatrix;

class clNode
{
private:
  enObjectType       objectType;
  int                nodeID;
  bool               isHaloNode;
  bool               isBoundaryNode;
  std::vector<int>   elementIDList;

protected:
  dMatrix       coord;

  virtual void  SetObjectType(enObjectType type) {objectType = type;};
  int           SetNodeID(const int);

public:
  clNode(void);
  clNode(const int, const dMatrix&);
  clNode(const clNode&);
  virtual ~clNode(void);

  enObjectType  GetObjectType(void) {return (objectType);};
  int           GetNodeID(void) const;

  std::string   GetNodeDescription(void) const;

  bool          IsHaloNode(void) const;
  void          SetHaloNode(void);
  void          ResetHaloNode(void);

  bool          IsBoundaryNode(void) const;
  void          SetBoundaryNode(void);
  void          ResetBoundaryNode(void);

  dMatrix       GetCoordinates(void) const;
  dMatrix       SetCoordinates(const dMatrix&);

  int           GetNumberElements(void) const;
  int           GetElementID(const int) const;
  int           AddElementID(const int);
  void          DeleteElementRefByIndex(const int);
  void          DeleteElementRefByID(const int);

  bool          IsElementNode(const int) const;

  clNode&       Assign(const clNode&);
  clNode&       operator=(const clNode&);
  bool          IsEqual(const clNode&) const;
  bool          operator==(const clNode&) const;
  bool          operator!=(const clNode&) const;
};

#endif

