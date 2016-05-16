/***************************************************
*** Body file for definition of class clTetrahedron
***
*** J.L. klaufus - Cranfield 1999
**/

#include <stdio.h>

#include <fstream>

#include "array.h"
#include "exceptiontree.h"
#include "list.h"
#include "matrix.h"
#include "node3d.h"
#include "tetrahedron.h"
#include "unstructured3d.h"

using namespace std;

clTetrahedron::clTetrahedron(void) : clElement3D(NULL, 0, 4, 4, 3)
{
  try
  {
    SetObjectType(otTetrahedron);

    SetArea(-1.0f);
    SetVolume(-1.0f);

    circumCentre       = NULL;
    centroid           = NULL;
    radiusCircumsphere = -1.0f;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::clTetrahedron");
    throw EX;
  }
}

clTetrahedron::clTetrahedron(clUnstructured3D *volumeGrid) : clElement3D(volumeGrid, 0, 4, 4, 3)
{
  try
  {
    SetObjectType(otTetrahedron);
    SetArea(-1.0f);
    SetVolume(-1.0f);

    circumCentre       = NULL;
    centroid           = NULL;
    radiusCircumsphere = -1.0f;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::clTetrahedron");
    throw EX;
  }
}

clTetrahedron::clTetrahedron(clUnstructured3D *volumeGrid, const int number) : clElement3D(volumeGrid, number, 4, 4, 3)
{
  try
  {
    SetObjectType(otTetrahedron);
    SetArea(-1.0f);
    SetVolume(-1.0f);

    circumCentre       = NULL;
    centroid           = NULL;
    radiusCircumsphere = -1.0f;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::clTetrahedron");
    throw EX;
  }
}

clTetrahedron::clTetrahedron(const clTetrahedron &source) : clElement3D(source)
{
  try
  {
    //TODO: Create Assign method for clTetrahedron
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::clTetrahedron");
    throw EX;
  }
}

clTetrahedron::~clTetrahedron(void)
{
  if(circumCentre) delete circumCentre;
  if(centroid) delete centroid;

  //cerr << "DESTROYING TETRAHEDRON: " << GetNumberID() << endl;
}

void clTetrahedron::GetFaceNodes(const int faceIndex, int *faceNodes) const
{
  /*
  ** Returns the indeces of the nodes in tetrahedron system, given the face index
  */

  try
  {
    if(faceIndex<0 || faceIndex>=GetNumberFaces())
    {
      char message[MESLEN];
      sprintf(message, "Invalid face entry.\n  FACE = %d\n", faceIndex);
      throw clException("clTetrahedron", "GetFaceNodes", message);
    }
    else
    {
      switch(faceIndex)
      {
        case 0:
          faceNodes[0]=1;
          faceNodes[1]=2;
          faceNodes[2]=3;
          break;
        case 1:
          faceNodes[0]=0;
          faceNodes[1]=3;
          faceNodes[2]=2;
          break;
        case 2:
          faceNodes[0]=0;
          faceNodes[1]=1;
          faceNodes[2]=3;
          break;
        case 3:
          faceNodes[0]=0;
          faceNodes[1]=2;
          faceNodes[2]=1;
          break;
        default:
          char message[MESLEN];
          sprintf(message, "Invalid face entry.\n  FACE = %d\n", faceIndex);
          throw clException("clElement3D", "GetFaceNodes", message);
          break;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::GetFaceNodes");
    throw EX;
  }
}

int clTetrahedron::GetFaceNode(const int faceIndex, const int nodeIndex) const
{
  /*
  ** Returns the index of the tetrahedron node, given the node index of a face index
  */

  int ret = -1;

  try
  {
    int faceNodes[3];
    GetFaceNodes(faceIndex, &faceNodes[0]);

    ret = faceNodes[nodeIndex];
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::GetFaceNode");
    throw EX;
  }

  return(ret);
}

bool clTetrahedron::InSphereCriterion(const clNode3D *theNode)
{
  //
  // IN    :  *theNode = the node to be tested for
  // RETURN:  1 if node falls within the circumsphere of current tetrahedron
  //          0 if node falls outside the circumsphere of current tetrahedron
  //

  bool  ret=false;

  try
  {
    clNode3D         centre = GetCircumCentre();
    double           radius = GetRadiusCircumsphere();
    tpMatrix<double> coord  = theNode->GetCoordinates();

    double r=sqrt((theNode->x() - centre.x())*(theNode->x() - centre.x()) +
                  (theNode->y() - centre.y())*(theNode->y() - centre.y()) +
                  (theNode->z() - centre.z())*(theNode->z() - centre.z()));

//    if(r<radius)
    if((radius - r) > PRECISION)
    {
//      cout << "ID= " << GetNumberID() << "; r= " << r << "; radius= " << radius << endl;
      ret=true;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::InSphereCriterion");
    throw EX;
  }

  return(ret);
}

void clTetrahedron::CalculateCircumCentre(void)
{
  //
  // IN    :
  // RETURN:
  //

  try
  {
    if(!IsHaloCell())
    {
      //THIS CALCULATION HAS COMPLETELY CHANGED!
      //WRONG ASSUMPTION OF MATRIX CALCULATION IN THESIS PAGE 35
      //SECOND MATRIX SHOULD NOT BE A MATRIX, BUT A VECTOR

      int numberNodes = GetNumberNodes();

      // Retrieve the nodes
      int       nodeIndices[numberNodes];
      clNode3D *nodes[numberNodes];
      for(int n=0; n<numberNodes; n++)
      {
        nodeIndices[n] = parentGrid->GetNodeIndex(GetNodeID(n));
        nodes[n]       = parentGrid->GetNodePointer(nodeIndices[n]);
      }

      double x1 = nodes[1]->x() - nodes[0]->x();
      double x2 = nodes[2]->x() - nodes[0]->x();
      double x3 = nodes[3]->x() - nodes[0]->x();

      double y1 = nodes[1]->y() - nodes[0]->y();
      double y2 = nodes[2]->y() - nodes[0]->y();
      double y3 = nodes[3]->y() - nodes[0]->y();

      double z1 = nodes[1]->z() - nodes[0]->z();
      double z2 = nodes[2]->z() - nodes[0]->z();
      double z3 = nodes[3]->z() - nodes[0]->z();

      double x12 = (nodes[1]->x() + nodes[0]->x()) * 0.5f;
      double x13 = (nodes[2]->x() + nodes[0]->x()) * 0.5f;
      double x14 = (nodes[3]->x() + nodes[0]->x()) * 0.5f;

      double y12 = (nodes[1]->y() + nodes[0]->y()) * 0.5f;
      double y13 = (nodes[2]->y() + nodes[0]->y()) * 0.5f;
      double y14 = (nodes[3]->y() + nodes[0]->y()) * 0.5f;

      double z12 = (nodes[1]->z() + nodes[0]->z()) * 0.5f;
      double z13 = (nodes[2]->z() + nodes[0]->z()) * 0.5f;
      double z14 = (nodes[3]->z() + nodes[0]->z()) * 0.5f;

      // Setup coefficient matrix
      tpMatrix<double> A(3,3);
      A.SetElement(1, 1, x1);
      A.SetElement(2, 1, x2);
      A.SetElement(3, 1, x3);

      A.SetElement(1, 2, y1);
      A.SetElement(2, 2, y2);
      A.SetElement(3, 2, y3);

      A.SetElement(1, 3, z1);
      A.SetElement(2, 3, z2);
      A.SetElement(3, 3, z3);

      // Setup solution matrix
      tpMatrix<double> B(3,1);
      B.SetElement(1, 1, x1*x12 + y1*y12 + z1*z12);
      B.SetElement(2, 1, x2*x13 + y2*y13 + z2*z13);
      B.SetElement(3, 1, x3*x14 + y3*y14 + z3*z14);

      // Solve for the centre coordinates
      tpMatrix<double> C = A.Solve(B);

      circumCentre = new clNode3D(1, C(1,1),C(2,1),C(3,1));
    }
    else
    {
      char message[MESLEN];
      sprintf(message, "Tetrahedron protected.\n    ELEMENTID = %d", GetElementID());
      throw clExceptionTree("clTetrahedron", "CalculateCircumCentre", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::CalculateCircumCentre");
    throw EX;
  }
}

void clTetrahedron::CalculateCentroid(void)
{
  //
  // Calculates the Centre of Gravity of the Tetrahedron
  //
  // IN    :
  // RETURN:
  //

  try
  {
    if(!IsHaloCell())
    {
      // Retrieve the nodes references
      int nodeIndices[4];
      nodeIndices[0]=parentGrid->GetNodeIndex(GetNodeID(0));
      nodeIndices[1]=parentGrid->GetNodeIndex(GetNodeID(1));
      nodeIndices[2]=parentGrid->GetNodeIndex(GetNodeID(2));
      nodeIndices[3]=parentGrid->GetNodeIndex(GetNodeID(3));

      // Retrieve the nodes
      clNode3D *nodes[4];
      nodes[0]=parentGrid->GetNodePointer(nodeIndices[0]);
      nodes[1]=parentGrid->GetNodePointer(nodeIndices[1]);
      nodes[2]=parentGrid->GetNodePointer(nodeIndices[2]);
      nodes[3]=parentGrid->GetNodePointer(nodeIndices[3]);

      double xm = (nodes[0]->x() + nodes[1]->x() + nodes[2]->x() + nodes[3]->x())*0.25f;
      double ym = (nodes[0]->y() + nodes[1]->y() + nodes[2]->y() + nodes[3]->y())*0.25f;
      double zm = (nodes[0]->z() + nodes[1]->z() + nodes[2]->z() + nodes[3]->z())*0.25f;

      centroid = new clNode3D(1, xm, ym, zm);
    }
    else
    {
      char message[MESLEN];
      sprintf(message, "Tetrahedron protected.\n    ELEMENTID = %d", GetElementID());
      throw clExceptionTree("clTetrahedron", "CalculateCentroid", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::CalculateCentroid");
    throw EX;
  }
}

void clTetrahedron::CalculateRadiusCircumsphere(void)
{
  try
  {
    if(IsHaloCell())
    {
      radiusCircumsphere = (double) (-1);
    }
    else
    {
      // TODO: Is centrePoint hier ALTIJD al uitgerekend -> needs checking
      int              nodeIndex = parentGrid->GetNodeIndex(GetNodeID(0));
      clNode3D        *node      = parentGrid->GetNodePointer(nodeIndex);

      double           x = node->x()-circumCentre->x();
      double           y = node->y()-circumCentre->y();
      double           z = node->z()-circumCentre->z();

      radiusCircumsphere = sqrt(x*x + y*y + z*z);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::CalculateRadiusCircumsphere");
    throw EX;
  }
}

void clTetrahedron::CalculateArea(void)
{
  // TODO: Calculate the area of a tetrahedron
  SetArea(-1.0f);
}

void clTetrahedron::CalculateVolume(void)
{
  try
  {
    if(IsHaloCell())
    {
      SetVolume(-1.0f);
    }
    else
    {
      // Retrieve the nodes references
      int nodeIndices[4];
      nodeIndices[0]=parentGrid->GetNodeIndex(GetNodeID(0));
      nodeIndices[1]=parentGrid->GetNodeIndex(GetNodeID(1));
      nodeIndices[2]=parentGrid->GetNodeIndex(GetNodeID(2));
      nodeIndices[3]=parentGrid->GetNodeIndex(GetNodeID(3));

      // Retrieve the nodes
      clNode3D *nodes[4];
      nodes[0]=parentGrid->GetNodePointer(nodeIndices[0]);
      nodes[1]=parentGrid->GetNodePointer(nodeIndices[1]);
      nodes[2]=parentGrid->GetNodePointer(nodeIndices[2]);
      nodes[3]=parentGrid->GetNodePointer(nodeIndices[3]);

      double x1 = nodes[1]->x() - nodes[0]->x();
      double x2 = nodes[1]->y() - nodes[0]->y();
      double x3 = nodes[1]->z() - nodes[0]->z();

      double y1 = nodes[2]->x() - nodes[0]->x();
      double y2 = nodes[2]->y() - nodes[0]->y();
      double y3 = nodes[2]->z() - nodes[0]->z();

      double z1 = nodes[3]->x() - nodes[0]->x();
      double z2 = nodes[3]->y() - nodes[0]->y();
      double z3 = nodes[3]->z() - nodes[0]->z();

      double det1 = x1*(y2*z3-y3*z2);
      double det2 = x2*(y1*z3-y3*z1);
      double det3 = x3*(y1*z2-y2*z1);
      double det  = det1 - det2 + det3;

      double volume = det/6.0f;
      SetVolume(volume);
//      if(volume < PRECISION)
//      {
//        string message = "Invalid Tetrahedron: Zero or Negative volume\n";
//        message += GetElementDescription();
//        throw clExceptionTree("clTetrahedron", "CalculateVolume", message);
//      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTetrahedron::CalculateVolume");
    throw EX;
  }
}

double clTetrahedron::GetArea(void)
{
  if(clElement3D::GetArea() < PRECISION) CalculateArea();
  return(clElement3D::GetArea());
}

double clTetrahedron::GetVolume(void)
{
  if(clElement3D::GetVolume() < PRECISION) CalculateVolume();
  return(clElement3D::GetVolume());
}

clNode3D clTetrahedron::GetCircumCentre(void)
{
  if(!circumCentre) CalculateCircumCentre();
  return(*circumCentre);
}

clNode3D clTetrahedron::GetCentroid(void)
{
  if(!centroid) CalculateCentroid();
  return(*centroid);
}

double clTetrahedron::GetRadiusCircumsphere(void)
{
  if(radiusCircumsphere < PRECISION) CalculateRadiusCircumsphere();
  return(radiusCircumsphere);
}

bool clTetrahedron::IsValidElement(void)
{
  bool success = true;

  success = success && (GetVolume() > PRECISION);

  return(success);
}
