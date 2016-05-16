/*************************************************
*** Body file for definition of class clElement2D
***
*** J.L. Klaufus - Cranfield 1999
**/

//#include <stdio.h>
//#include <stdlib.h>

#include <algorithm>
#include <vector>
#include <sstream>

#include "element2d.h"
#include "exception.h"
#include "matrix.h"
#include "node2d.h"
#include "unstructured2d.h"

clElement2D::clElement2D(void) : clElement()
{
  try
  {
    SetObjectType(otElement2D);
    parentGrid = NULL;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement2D::clElement2D");
    throw EX;
  }
}

clElement2D::clElement2D(clUnstructured2D *surfaceGrid, const int elementID, const int nNodes, const int nFaces, const int nFaceNodes) : clElement(elementID, nNodes, nFaces, nFaceNodes)
{
  try
  {
    SetObjectType(otElement2D);
    parentGrid = surfaceGrid;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement2D::clElement2D");
    throw EX;
  }
}

clElement2D::clElement2D(clUnstructured2D *surfaceGrid, const int nNodes, const int nFaces, const int nFaceNodes) : clElement(nNodes, nFaces, nFaceNodes)
{
  try
  {
    SetObjectType(otElement2D);
    parentGrid = surfaceGrid;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement2D::clElement2D");
    throw EX;
  }
}

clElement2D::clElement2D(const clElement2D &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement2D::clElement2D");
    throw EX;
  }
}

clElement2D::~clElement2D(void)
{
  //cerr << "DESTROYING ELEMENT: " << GetElementID() << endl;
}

void clElement2D::SetParentGrid(clUnstructured2D* grid)
{
  parentGrid = grid;
}

inline clUnstructured2D* clElement2D::GetParentGrid(void) const
{
  return(parentGrid);
}

int clElement2D::IsNodeInElement(const clNode2D *theNode) const
{
  //
  // IN :    *theNode    = the node to be investigated
  // RETURN: ID of current tetrahedron in case it covers theNode,
  //         or ID of one of the neighbours.
  //

  // TODO: The determinant principle is reverse of element3D
  int    elementID = GetElementID();
  double det       = 1.0f;
  bool   onFace    = false;

  try
  {
    // Translate node indices in face system to node indices in parentgrid system
    int faceNodes[GetNumberFaceNodes()];

    //
    // Search all faces as long as the face vertices form a left-handed axes system.
    // (det>0 --> left-handed axes system --> theNode on the inside of triangle).
    //
    int f=0;
    while(det>=0.0f && f<GetNumberFaces())
    {
      // Get the node indices in tetrahedron system
      GetFaceNodes(f, faceNodes);

      // Get NodeIDs
      int idNode0 = GetNodeID(faceNodes[0]);
      int idNode1 = GetNodeID(faceNodes[1]);

      // Get node indices in surfaceGrid system
      int inNode0 = parentGrid->GetNodeIndex(idNode0);
      int inNode1 = parentGrid->GetNodeIndex(idNode1);

      // Get Node Pointers
      clNode2D *node0 = parentGrid->GetNodePointer(inNode0);
      clNode2D *node1 = parentGrid->GetNodePointer(inNode1);

      // Set up vectors
      double x1 = node1->x() - node0->x();
      double x2 = node1->y() - node0->y();

      double y1 = theNode->x() - node0->x();
      double y2 = theNode->y() - node0->y();

      det = x1*y2 - x2*y1;

      if(fabs(det) < PRECISION) onFace = true;

      f++;
    }

    if(det<0.0f)
    {
      // Continue with neighbour element
      f--;
      elementID=GetNeighbourID(f);
    }
    else if(onFace)
    {
      char message[MESLEN];
      sprintf(message, "Node positioned on face of element.\n    ELEMENTID = %d", GetElementID());
//      throw clExceptionTree("clElement2D", "IsNodeInElement", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement2D::IsNodeInElement");
    throw EX;
  }

  return(elementID);
}

clElement2D& clElement2D::Assign(const clElement2D &source)
{
  SetParentGrid(source.GetParentGrid());
  SetElementID(source.GetElementID());
  SetNumberNodes(source.GetNumberNodes());
  SetNumberFaces(source.GetNumberFaces());
  SetNumberFaceNodes(source.GetNumberFaceNodes());

  source.IsInitialised() ? SetInitialised() : ResetInitialised();
  source.IsHaloCell() ? SetHaloCell() : ResetHaloCell();
  source.IsBoundaryCell() ? SetBoundaryCell() : ResetBoundaryCell();

  for(int n=0; n<source.GetNumberNodes(); n++)
  {
    SetNodeID(n, source.GetNodeID(n));
  }

  for(int f=0; f<source.GetNumberFaces(); f++)
  {
    SetNeighbourID(f, source.GetNeighbourID(f));
  }

  return(*this);
}

clElement2D& clElement2D::operator=(const clElement2D &source)
{
  return(Assign(source));
}

bool clElement2D::IsEqual(const clElement2D &source) const
{
  /*
  ** Does not evaluate 'elementID'
  */
  int ret = 0; //0 = not equal; 1 = equal

  for(int n=0; n<GetNumberNodes(); n++)
  {
    if(GetNodeID(n) != source.GetNodeID(n))
    {
      ret = false;
    }
  }

  for(int f=0; f<GetNumberFaces(); f++)
  {
    if(GetNeighbourID(f) != source.GetNeighbourID(f))
    {
      ret = false;
    }
  }

  return(ret);
}

bool clElement2D::operator==(const clElement2D &source) const
{
  return(IsEqual(source));
}

bool clElement2D::operator!=(const clElement2D &source) const
{
  return(!IsEqual(source));
}
