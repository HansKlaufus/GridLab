/*************************************************
*** Body file for definition of class clElement3D
***
*** J.L. Klaufus - Cranfield 1999
**/

//#include <stdio.h>
//#include <stdlib.h>

#include <algorithm>
#include <vector>
#include <sstream>

#include "element3d.h"
#include "exception.h"
#include "matrix.h"
#include "node3d.h"
#include "unstructured3d.h"

clElement3D::clElement3D(void) : clElement()
{
  try
  {
    SetObjectType(otElement3D);
    parentGrid = NULL;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement3D::clElement3D");
    throw EX;
  }
}

clElement3D::clElement3D(clUnstructured3D *volumeGrid, const int elementID, const int nNodes, const int nFaces, const int nFaceNodes) : clElement(elementID, nNodes, nFaces, nFaceNodes)
{
  try
  {
    SetObjectType(otElement3D);
    parentGrid = volumeGrid;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement3D::clElement3D");
    throw EX;
  }
}

clElement3D::clElement3D(clUnstructured3D *volumeGrid, const int nNodes, const int nFaces, const int nFaceNodes)
{
  try
  {
    SetObjectType(otElement3D);
    parentGrid = volumeGrid;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement3D::clElement3D");
    throw EX;
  }
}

clElement3D::clElement3D(const clElement3D &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement3D::clElement3D");
    throw EX;
  }
}

clElement3D::~clElement3D(void)
{
  //cerr << "DESTROYING ELEMENT: " << GetElementID() << endl;
}

void clElement3D::SetParentGrid(clUnstructured3D* volumeGrid)
{
  parentGrid = volumeGrid;
}

inline clUnstructured3D* clElement3D::GetParentGrid(void) const
{
  return(parentGrid);
}

int clElement3D::IsNodeInElement(const clNode3D *theNode) const
{
  //
  // IN :    *theNode    = the node to be investigated
  // RETURN: ID of current tetrahedron in case it covers theNode,
  //         or ID of one of the neighbours.
  //
  int    tetraID = GetElementID();
  double det     = (double)(-1);
  bool   onFace  = false;

  try
  {
    // Translate node indices in face system to node indices in parentgrid system
    int faceNodes[GetNumberFaceNodes()];

    //
    // Search all faces as long as the face vertices form a left-handed axes system.
    // (det<0 --> left-handed axes system --> theNode on the inside of tetrahedron).
    //
    int f=0;
    while(det<=PRECISION && f<GetNumberFaces()) // TODO: similar for element2D
    {
      // Get the node indices in tetrahedron system
      GetFaceNodes(f, faceNodes);

      // Get NodeIDs
      int idNode0 = GetNodeID(faceNodes[0]);
      int idNode1 = GetNodeID(faceNodes[1]);
      int idNode2 = GetNodeID(faceNodes[GetNumberFaceNodes()-1]);

      // Get node indices in volumeGrid system
      int inNode0 = parentGrid->GetNodeIndex(idNode0);
      int inNode1 = parentGrid->GetNodeIndex(idNode1);
      int inNode2 = parentGrid->GetNodeIndex(idNode2);

      // Get Node Pointers
      clNode3D *node0 = parentGrid->GetNodePointer(inNode0);
      clNode3D *node1 = parentGrid->GetNodePointer(inNode1);
      clNode3D *node2 = parentGrid->GetNodePointer(inNode2);

      // Set up vectors
      double x1 = node1->x() - node0->x();
      double x2 = node1->y() - node0->y();
      double x3 = node1->z() - node0->z();

      double y1 = node2->x() - node0->x();
      double y2 = node2->y() - node0->y();
      double y3 = node2->z() - node0->z();

      double z1 = theNode->x() - node0->x();
      double z2 = theNode->y() - node0->y();
      double z3 = theNode->z() - node0->z();

      double det1 = x1*(y2*z3-y3*z2);
      double det2 = x2*(y1*z3-y3*z1);
      double det3 = x3*(y1*z2-y2*z1);
      det = det1 - det2 + det3;

      if(fabs(det) < PRECISION) onFace = true; // TODO: Similar in element2d

      f++;
    }

//    if(det>0.0f)
    if(det > PRECISION) // TODO: Similar in element2d
    {
      // Continue with neighbour tetrahedron
      f--;
      tetraID=GetNeighbourID(f);
    }
    else if(onFace)
    {
      char message[MESLEN];
      sprintf(message, "Node positioned on face of element.\n    ELEMENTID = %d", GetElementID());
//      throw clExceptionTree("clElement3D", "IsNodeInElement", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement3D::IsNodeInElement");
    throw EX;
  }

  return(tetraID);
}

clElement3D& clElement3D::Assign(const clElement3D &source)
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

clElement3D& clElement3D::operator=(const clElement3D &source)
{
  return(Assign(source));
}

bool clElement3D::IsEqual(const clElement3D &source) const
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

bool clElement3D::operator==(const clElement3D &source) const
{
  return(IsEqual(source));
}

bool clElement3D::operator!=(const clElement3D &source) const
{
  return(!IsEqual(source));
}
