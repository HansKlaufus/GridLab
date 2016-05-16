/***************************************************
*** Body file for definition of class clTriangle
***
*** J.L. klaufus - Haarlem 2009
**/

#include <stdio.h>

#include <fstream>

#include "array.h"
#include "exceptiontree.h"
#include "list.h"
#include "matrix.h"
#include "node2d.h"
#include "triangle.h"
#include "unstructured2d.h"

using namespace std;

clTriangle::clTriangle(void) : clElement2D(NULL, 0, 3, 3, 2)
{
  try
  {
    SetObjectType(otTriangle);
    SetArea(-1.0f);
    SetVolume(-1.0f);
    SetMinimumAngle(361.0f);

    circumCentre       = NULL;
    centroid           = NULL;
    radiusCircumcircle = -1.0f;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::clTriangle");
    throw EX;
  }
}

clTriangle::clTriangle(clUnstructured2D *grid) : clElement2D(grid, 0, 3, 3, 2)
{
  try
  {
    SetObjectType(otTriangle);
    SetArea(-1.0f);
    SetVolume(-1.0f);
    SetMinimumAngle(361.0f);

    circumCentre       = NULL;
    centroid           = NULL;
    radiusCircumcircle = -1.0f;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::clTriangle");
    throw EX;
  }
}

clTriangle::clTriangle(clUnstructured2D *grid, const int ID) : clElement2D(grid, ID, 3, 3, 2)
{
  try
  {
    SetObjectType(otTriangle);
    SetArea(-1.0f);
    SetVolume(-1.0f);
    SetMinimumAngle(361.0f);

    circumCentre       = NULL;
    centroid           = NULL;
    radiusCircumcircle = -1.0f;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::clTriangle");
    throw EX;
  }
}

clTriangle::clTriangle(const clTriangle &source) : clElement2D(source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::clTriangle");
    throw EX;
  }
}

clTriangle::~clTriangle(void)
{
  if(circumCentre) delete circumCentre;
  if(centroid) delete centroid;

  //cerr << "DESTROYING TETRAHEDRON: " << GetNumberID() << endl;
}

void clTriangle::GetFaceNodes(const int faceIndex, int *faceNodes) const
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
      throw clException("clTriangle", "GetFaceNodes", message);
    }
    else
    {
      switch(faceIndex)
      {
        case 0:
          faceNodes[0]=1;
          faceNodes[1]=2;
          break;
        case 1:
          faceNodes[0]=2;
          faceNodes[1]=0;
          break;
        case 2:
          faceNodes[0]=0;
          faceNodes[1]=1;
          break;
        default:
          char message[MESLEN];
          sprintf(message, "Invalid face entry.\n  FACE = %d\n", faceIndex);
          throw clException("clTriangle", "GetFaceNodes", message);
          break;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::GetFaceNodes");
    throw EX;
  }
}

int clTriangle::GetFaceNode(const int faceIndex, const int nodeIndex) const
{
  /*
  ** Returns the index of the triangle node, given the node index of a face index
  */

  int ret = -1;

  try
  {
    int faceNodes[GetNumberFaceNodes()];
    GetFaceNodes(faceIndex, &faceNodes[0]);

    ret = faceNodes[nodeIndex];
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::GetFaceNode");
    throw EX;
  }

  return(ret);
}

void clTriangle::GetNeighbourNodes(const int nodeIndex, int *neighbourNodes) const
{
  /*
  ** Returns the indeces of the nodes in tetrahedron system, given the face index
  */

  try
  {
    GetFaceNodes(nodeIndex, neighbourNodes);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::GetFaceNodes");
    throw EX;
  }
}

int clTriangle::GetNeighbourNode(const int nodeIndex, const int neighbourIndex) const
{
  /*
  ** Returns the index of the triangle node, given the node index of a face index
  */

  int ret = -1;

  try
  {
    ret = GetFaceNode(nodeIndex, neighbourIndex);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::GetFaceNode");
    throw EX;
  }

  return(ret);
}

bool clTriangle::InCircleCriterion(const clNode2D *theNode)
{
  //
  // IN    :  *theNode = the node to be tested for
  // RETURN:  1 if node falls within the circumsphere of current tetrahedron
  //          0 if node falls outside the circumsphere of current tetrahedron
  //

  bool  ret=false;

  try
  {
    clNode2D         centre = GetCircumCentre();
    double           radius = GetRadiusCircumcircle();
    tpMatrix<double> coord  = theNode->GetCoordinates();

    double r=sqrt((theNode->x() - centre.x())*(theNode->x() - centre.x()) +
                  (theNode->y() - centre.y())*(theNode->y() - centre.y()));

//    if(r<radius)
    if((radius - r) > PRECISION)
    {
//      cout << "ID= " << GetNumberID() << "; r= " << r << "; radius= " << radius << endl;
      ret=true;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::InCircleCriterion");
    throw EX;
  }

  return(ret);
}

void clTriangle::CalculateCircumCentre(void)
{
  //
  // IN    :
  // RETURN:
  //

  try
  {
    if(!IsHaloCell())
    {
      int numberNodes = GetNumberNodes();

      // Retrieve the nodes
      int       nodeIndices[numberNodes];
      clNode2D *nodes[numberNodes];
      for(int n=0; n<numberNodes; n++)
      {
        nodeIndices[n] = parentGrid->GetNodeIndex(GetNodeID(n));
        nodes[n]       = parentGrid->GetNodePointer(nodeIndices[n]);
      }

      double x1 = nodes[1]->x() - nodes[0]->x();
      double x2 = nodes[2]->x() - nodes[0]->x();

      double y1 = nodes[1]->y() - nodes[0]->y();
      double y2 = nodes[2]->y() - nodes[0]->y();

      double x12 = (nodes[1]->x() + nodes[0]->x()) * 0.5f;
      double x13 = (nodes[2]->x() + nodes[0]->x()) * 0.5f;

      double y12 = (nodes[1]->y() + nodes[0]->y()) * 0.5f;
      double y13 = (nodes[2]->y() + nodes[0]->y()) * 0.5f;

      // Setup coefficient matrix
      tpMatrix<double> A(2,2);
      A.SetElement(1, 1, x1);
      A.SetElement(2, 1, x2);

      A.SetElement(1, 2, y1);
      A.SetElement(2, 2, y2);

      // Setup solution matrix
      tpMatrix<double> B(2,1);
      B.SetElement(1, 1, x1*x12 + y1*y12);
      B.SetElement(2, 1, x2*x13 + y2*y13);

      // Solve for the centre coordinates
      tpMatrix<double> C = A.Solve(B);

      circumCentre = new clNode2D(1, C(1,1),C(2,1));
    }
    else
    {
      char message[MESLEN];
      sprintf(message, "Tetrahedron protected.\n    ELEMENTID = %d", GetElementID());
      throw clExceptionTree("clTriangle", "CalculateCircumCentre", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::CalculateCircumCentre");
    throw EX;
  }
}

void clTriangle::CalculateCentroid(void)
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
      // Retrieve the nodes
      int       numberNodes = GetNumberNodes();
      int       nodeIndices[numberNodes];
      clNode2D *nodes[numberNodes];
      for(int n=0; n<numberNodes; n++)
      {
        nodeIndices[n] = parentGrid->GetNodeIndex(GetNodeID(n));
        nodes[n]       = parentGrid->GetNodePointer(nodeIndices[n]);
      }

      double xm = (nodes[0]->x() + nodes[1]->x() + nodes[2]->x())/3.0f;
      double ym = (nodes[0]->y() + nodes[1]->y() + nodes[2]->y())/3.0f;

      centroid = new clNode2D(1, xm, ym);
    }
    else
    {
      char message[MESLEN];
      sprintf(message, "Triangle protected.\n    ELEMENTID = %d", GetElementID());
      throw clExceptionTree("clTriangle", "CalculateCentroid", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::CalculateCentroid");
    throw EX;
  }
}

void clTriangle::CalculateRadiusCircumsphere(void)
{
  try
  {
    if(IsHaloCell())
    {
      radiusCircumcircle = (double) (-1);
    }
    else
    {
      // TODO: Is centrePoint hier ALTIJD al uitgerekend -> needs checking
      int              nodeIndex = parentGrid->GetNodeIndex(GetNodeID(0));
      clNode2D        *node      = parentGrid->GetNodePointer(nodeIndex);

      double           x = node->x()-circumCentre->x();
      double           y = node->y()-circumCentre->y();

      radiusCircumcircle = sqrt(x*x + y*y);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::CalculateRadiusCircumsphere");
    throw EX;
  }
}

void clTriangle::CalculateArea(void)
{
  /*
  ** Returns the are of the element
  */
  try
  {
    if(IsHaloCell())
    {
      SetArea(-1.0f);
    }
    else
    {
      // Retrieve the nodes
      int       numberNodes = GetNumberNodes();
      int       nodeIndices[numberNodes];
      clNode2D *nodes[numberNodes];
      for(int n=0; n<numberNodes; n++)
      {
        nodeIndices[n] = parentGrid->GetNodeIndex(GetNodeID(n));
        nodes[n]       = parentGrid->GetNodePointer(nodeIndices[n]);
      }

      double x1 = nodes[0]->x();
      double y1 = nodes[0]->y();

      double x2 = nodes[1]->x();
      double y2 = nodes[1]->y();

      double x3 = nodes[2]->x();
      double y3 = nodes[2]->y();

      double det1 = x2*y3 - x3*y2;
      double det2 = x1*y3 - x3*y1;
      double det3 = x1*y2 - x2*y1;
      double det  = det1 - det2 + det3;

      double area = det/2.0f;
      SetArea(area);
//      if(area < PRECISION)
//      {
//        string message = "Invalid Triangle: Zero or Negative area\n";
//        message += GetElementDescription();
//        throw clExceptionTree("clTriangle", "CalculateArea", message);
//      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::CalculateArea");
    throw EX;
  }
}

void clTriangle::CalculateVolume(void)
{
  SetVolume(0.0f);
}

void clTriangle::CalculateMinimumAngle(void)
{
  try
  {
    if(IsHaloCell())
    {
      SetMinimumAngle(-1.0f);
    }
    else
    {
      // Retrieve the nodes
      int numberNodes = GetNumberNodes();
      if(numberNodes>2)
      {
        SetMinimumAngle(361.0f);

        for(int n=0; n<numberNodes; n++)
        {
          int nodeID1 = GetNodeID((n+0) % numberNodes);
          int nodeID2 = GetNodeID((n+1) % numberNodes);
          int nodeID3 = GetNodeID((n+2) % numberNodes);

          int nodeIndex1 = parentGrid->GetNodeIndex(nodeID1);
          int nodeIndex2 = parentGrid->GetNodeIndex(nodeID2);
          int nodeIndex3 = parentGrid->GetNodeIndex(nodeID3);

          clNode2D *node1 = parentGrid->GetNodePointer(nodeIndex1);
          clNode2D *node2 = parentGrid->GetNodePointer(nodeIndex2);
          clNode2D *node3 = parentGrid->GetNodePointer(nodeIndex3);

          double x1 = node1->x();
          double y1 = node1->y();

          double x2 = node2->x();
          double y2 = node2->y();

          double x3 = node3->x();
          double y3 = node3->y();

          double vx1 = x1 - x2;
          double vy1 = y1 - y2;

          double vx2 = x3 - x2;
          double vy2 = y3 - y2;

          double length1 = sqrt(vx1*vx1 + vy1*vy1);
          double length2 = sqrt(vx2*vx2 + vy2*vy2);

          double dotProduct = vx1*vx2 + vy1*vy2;

          double angle = acos(dotProduct / (length1*length2)) * 180 / PI;

//          if(angle < PRECISION)
//          {
//            string message = "Invalid Triangle: Zero or Negative minimum Angle\n";
//            message += GetElementDescription();
//            throw clExceptionTree("clElement2D", "CalculateMinimumAngle", message);
//          }

          if(angle<clElement2D::GetMinimumAngle()) SetMinimumAngle(angle);
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clTriangle::CalculateMinimumAngle");
    throw EX;
  }
}

double clTriangle::GetArea(void)
{
  if(clElement2D::GetArea() < PRECISION) CalculateArea();
  return(clElement2D::GetArea());
}

double clTriangle::GetVolume(void)
{
  if(clElement2D::GetVolume() < PRECISION) CalculateVolume();
  return(clElement2D::GetVolume());
}

double clTriangle::GetMinimumAngle(void)
{
  if(clElement2D::GetMinimumAngle() < PRECISION) CalculateMinimumAngle();
  return(clElement2D::GetMinimumAngle());
}

clNode2D clTriangle::GetCircumCentre(void)
{
  if(!circumCentre) CalculateCircumCentre();
  return(*circumCentre);
}

clNode2D clTriangle::GetCentroid(void)
{
  if(!centroid) CalculateCentroid();
  return(*centroid);
}

double clTriangle::GetRadiusCircumcircle(void)
{
  if(radiusCircumcircle < PRECISION) CalculateRadiusCircumsphere();
  return(radiusCircumcircle);
}

bool clTriangle::IsValidElement(void)
{
  bool success = true;

  success = success && (GetArea() > PRECISION);
  success = success && (GetMinimumAngle() > PRECISION);

  return(success);
}
