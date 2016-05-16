/******************************************************
*** Body file for definition of class clUnstructured2D
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "exceptiontree.h"
#include "globals.h"
//#include "grid.h"
#include "matrix.h"
#include "node2d.h"
#include "stack.h"
#include "triangle.h"
#include "unstructured2d.h"

using namespace std;

clUnstructured2D::clUnstructured2D(void) : tpUnstructured<clNode2D, clTriangle>()
{
  try
  {
    SetObjectType(otUnstructured2D);
    CreateInitialGrid();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::clUnstructured2D");
    throw EX;
  }
}

clUnstructured2D::clUnstructured2D(double minX, double maxX, double minY, double maxY) : tpUnstructured<clNode2D, clTriangle>()
{
  try
  {
    SetObjectType(otUnstructured2D);
    CreateInitialGrid(minX, maxX, minY, maxY);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::clUnstructured2D");
    throw EX;
  }
}

clUnstructured2D::clUnstructured2D(const int ID) : tpUnstructured<clNode2D, clTriangle>(ID)
{
  try
  {
    SetObjectType(otUnstructured2D);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::clUnstructured2D");
    throw EX;
  }
}

//clUnstructured2D::clUnstructured2D(const clUnstructured2D &source)
//{
//  try
//  {
//  }
//
//  catch(clExceptionTree EX)
//  {
//    EX.AddMethodToTree("clUnstructured2D::CreateGrid");
//    throw EX;
//  }
//}

clUnstructured2D::~clUnstructured2D(void)
{
  //cerr << "DESTROYING GRID: " << GetNumberID() << endl;
}

clTriangle* clUnstructured2D::CreateElement(void)
{
  clTriangle* element = new clTriangle(this, GetNewElementID());

  return(element);
}

int clUnstructured2D::CreateInitialGrid(void)
{
  try
  {
    CreateInitialGrid(-2.0f, 2.0f, -2.0f, 2.0f);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::CreateInitialGrid");
    throw EX;
  }

  return(GetNumberElements());
}

int clUnstructured2D::CreateInitialGrid(double minX, double maxX, double minY, double maxY)
{
  try
  {
    // Check on minimal sizes
    if((maxX - minX) < PRECISION)
    {
      minX = -2.0f;
      maxX = +2.0f;
    }

    if((maxY - minY) < PRECISION)
    {
      minY = -2.0f;
      maxY = +2.0f;
    }

    // Create notes for enbounding box
    int entry = 0;
    entry+=AddNode(minX, minY);
    entry+=AddNode(maxX, minY);
    entry+=AddNode(maxX, maxY);
    entry+=AddNode(minX, maxY);

    if(entry!=10)
    {
      char message[MESLEN];
      sprintf(message, "Entry total does not match.\n  TOTAL=%d [10]", entry);
      throw clExceptionTree("clUnstructured2D", "CreateInitialGrid", message);
    }

    // Setup the elements
    entry=0;
    {int nodes[]={1, -1, 2}; int faces[]={-1, 6, -1};
    entry+=AddElement(3, faces, 3, nodes);} // 1

    {int nodes[]={2, -1, 3}; int faces[]={-1, 6, -1};
    entry+=AddElement(3, faces, 3, nodes);} // 2

    {int nodes[]={3, -1, 4}; int faces[]={-1, 5, -1};
    entry+=AddElement(3, faces, 3, nodes);} // 3

    {int nodes[]={4, -1, 1}; int faces[]={-1, 5, -1};
    entry+=AddElement(3, faces, 3, nodes);} // 4

    {int nodes[]={1, 3, 4}; int faces[]={3, 4, 6};
    entry+=AddElement(3, faces, 3, nodes);} // 5

    {int nodes[]={1, 2, 3}; int faces[]={2, 5, 1};
    entry+=AddElement(3, faces, 3, nodes);} // 6

    if(entry!=21)
    {
      char message[MESLEN];
      sprintf(message, "Entry total does not match.\n  TOTAL=%d [21]", entry);
      throw clExceptionTree("clUnstructured2D", "CreateInitialGrid", message);
    }

    // Protect Halo nodes
    for(int n=0; n<GetNumberNodes(); n++)
    {
      clNode2D* theNode = GetNodePointer(n);
      theNode->SetHaloNode();
    }

    // Protect Halo cells
    for(int e=0; e<GetNumberElements(); e++)
    {
      clTriangle* theElement = GetElementPointer(e);
      if(e<4)
      {
        theElement->SetHaloCell();
      }
      else
      {
        theElement->ResetHaloCell();
      }
    }

    CheckGrid();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::CreateInitialGrid");
    throw EX;
  }

  return(GetNumberElements());
}

bool clUnstructured2D::CheckGrid(void)
{
  bool ret=true;

  try
  {
    // Check initial grid
    ret = ret && CheckElementNodes();
    ret = ret && CheckElementConnections();
    ret = ret && CheckNodeConnections();
    ret = ret && CheckElementParentGrid();
    ret = ret && CheckElementAreas();
    ret = ret && CheckInCircleCriterion();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::CheckGrid");
    throw EX;
  }

  return(ret);
}

bool clUnstructured2D::CheckElementAreas(void)
{
  bool ret=true;

  try
  {
    // Loop through all elements
    for(int e=0; e<GetNumberElements(); e++)
    {
      clTriangle* theElement = GetElementPointer(e);

      if(!theElement->IsHaloCell() && theElement->GetArea()<=PRECISION)
      {
        ret=false;

        char message[MESLEN];
        sprintf(message, "Detected element with zero or negative area.\n    ELEMENTID = %d", theElement->GetElementID());
        throw clExceptionTree("clUnstructured2D", "CheckElementAreas", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::CheckElementAreas");
    throw EX;
  }

  return(ret);
}

bool clUnstructured2D::CheckInCircleCriterion(void)
{
  bool ret=true;

  try
  {
    // Loop through all elements
    for(int e=0; e<GetNumberElements(); e++)
    {
      clTriangle* element = GetElementPointer(e);

      // Check element is not haloCell
      if(!element->IsHaloCell())
      {
        // Loop through all nodes
        for(int n=0; n<GetNumberNodes(); n++)
        {
          clNode2D* node = GetNodePointer(n);

          // Check node is not haloNode
          if(!node->IsHaloNode())
          {
            // Check node is not part of element
            int  i             = 0;
            bool isElementNode = false;
            while(i<element->GetNumberNodes() && !isElementNode)
            {
              if(element->GetNodeID(i) == node->GetNodeID())
              {
                isElementNode = true;
              }

              i++;
            }

            if(!isElementNode && element->InCircleCriterion(node))
            {
              ret=false;

              char message[MESLEN];
              sprintf(message, "Detected node breaching in-sphere criterion.\n    ELEMENTID = %d\n    NODEID      = %d", element->GetElementID(), node->GetNodeID());
              element->InCircleCriterion(node);
              throw clExceptionTree("clUnstructured2D", "CheckInCircleCriterion", message);
            }
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::CheckInCircleCriterion");
    throw EX;
  }

  return(ret);
}

bool clUnstructured2D::CheckTotalArea(void)
{
  bool   ret       = true;
  double totalArea = 0.0;

  try
  {
    for(int e=0; e<GetNumberElements(); e++)
    {
      clTriangle* element = GetElementPointer(e);
      if(!element->IsHaloCell())
      {
        totalArea = totalArea + element->GetArea();
      }
    }

    if(fabs(totalArea - (double)(64)) > PRECISION)
    {
      ret=false;

      char message[MESLEN];
      sprintf(message, "Total volume incorrect.\n    AREA = %f [64.00]", totalArea);
      throw clExceptionTree("clUnstructured2D", "CheckTotalArea", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::CheckTotalArea");
    throw EX;
  }

  return(ret);
}

/***************************************************************
***
***   Performing Node Characteristic Operation
***
**/

int clUnstructured2D::AddNode(const double x, const double y)
{
  //
  // IN : x         = x-coordinate of new node
  //      y         = y-coordinate of new node
  // OUT: newNodeID = ID of new node
  //

  int newNodeID = 1;

  try
  {
    if(GetNumberNodes()>0)
    {
      clNode2D  *lastNode  = GetNodePointer(GetNumberNodes()-1);
      newNodeID            = lastNode->GetNodeID()+1;
    }

    clNode2D *newNode = new clNode2D(newNodeID, x, y);
    AddNode(newNode);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::AddNode");
    throw EX;
  }

  return(newNodeID);
}

int clUnstructured2D::AddNode(const double coord[2])
{
  //
  // IN : coord[3]  = coordinates of new node
  // OUT: newNodeID = ID of new node
  //
  int newNodeID = 1;

  try
  {
    newNodeID = AddNode(coord[0], coord[1]);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::AddNode");
    throw EX;
  }

  return(newNodeID);
}

/***************************************************************
***
***  Perforing Element Characteristic Operations
***
**/
int clUnstructured2D::AddElement(const int numberFaces, const int *faces, const int numberNodes, const int *nodes)
{
  //
  // IN : face1         = ID of element, neighbouring the face formed by vertices V2, V3
  //      face2         = ID of element, neighbouring the face formed by vertices V3, V1
  //      face3         = ID of element, neighbouring the face formed by vertices V1, V2
  //      node1         = ID of node1
  //      node2         = ID of node2
  //      node3         = ID of node3
  // OUT: newElementID  = ID of new element
  //

  int newElementID = 1;

  try
  {
    newElementID = GetNewElementID();
    clTriangle *newElement = new clTriangle(this, newElementID);

    for(int n=0; n<numberFaces; n++)
    {
      newElement->SetNeighbourID(n, faces[n]);
    }

    for(int n=0; n<numberNodes; n++)
    {
      newElement->SetNodeID(n, nodes[n]);
    }

    AddElement(newElement);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::AddElement");
    throw EX;
  }

  return(newElementID);
}

void clUnstructured2D::Smooth(void)
{
  try
  {
    tpList<int> neighbourNodeList;
    tpList<int> indexList;

    ComposeNeighbourNodeList(&neighbourNodeList, &indexList);

    // TODO: Nice; but doesn't do anything...
    BalanceNodes(&neighbourNodeList, &indexList);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::Smooth");
    throw EX;
  }
}

void clUnstructured2D::ComposeNeighbourNodeList(tpList<int> *neighbourNodeList, tpList<int> *indexList)
{
  try
  {
    // Loop through all nodes
    for(int n=0; n<GetNumberNodes(); n++)
    {
      clNode2D *node = GetNodePointer(n);

      indexList->AddItem(neighbourNodeList->GetNumberItems());

      if(!node->IsHaloNode())
      {
        // Loop through all elements
        for(int e=0; e<node->GetNumberElements(); e++)
        {
          int         elementID    = node->GetElementID(e);
          int         elementIndex = GetElementIndex(elementID);
          clTriangle *element      = GetElementPointer(elementIndex);

          // Get the nodeIndex in the element system
          int nodeIndex = element->GetNodeIndex(node->GetNodeID());

          // Get the nodes of the face with faceIndex == nodeIndex (face0 opposite node0 has node1 and node2)
          int neighbourIndex_1 = element->GetNeighbourNode(nodeIndex, 0);
          int neighbourIndex_2 = element->GetNeighbourNode(nodeIndex, 1);

          int neighbourID_1    = element->GetNodeID(neighbourIndex_1);
          int neighbourID_2    = element->GetNodeID(neighbourIndex_2);

          neighbourNodeList->AddItem(neighbourID_1);
          neighbourNodeList->AddItem(neighbourID_2);
        }
      }
    }

    // Check on size of indexList
    if(indexList->GetNumberItems() != GetNumberNodes())
    {
      char message[MESLEN];
      sprintf(message, "Size of indexList incorrect\n\tSize indexList = %d [%d]", indexList->GetNumberItems(), GetNumberNodes());
      throw clException("clUnstructured2D", "ComposeNeighbourList", message);
    }

    // Debug Info
    cout << endl << "IndexList: ";
    for (int i=0; i<indexList->GetNumberItems(); i++) cout << indexList->GetItem(i) << " ";

    cout << endl << "NeighbourNodeList: ";
    for (int i=0; i<neighbourNodeList->GetNumberItems(); i++) cout << neighbourNodeList->GetItem(i) << " ";

    cout << endl;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::ComposeNeighbourNodeList");
    throw EX;
  }
}

void clUnstructured2D::BalanceNodes(tpList<int> *neighbourNodeList, tpList<int> *indexList)
{
  try
  {
    int    iteration = 0;
    double omega     = 0.8f;
    double maxDelta  = 1000.0f;

    while(maxDelta>PRECISION)
    {
      iteration++;
      cout << "Iteration: " << iteration << " - Residual: " << maxDelta << endl;

      double maxDeltaX = 0.0f;
      double maxDeltaY = 0.0f;

      // Loop through all nodes
      for(int n=0; n<GetNumberNodes(); n++)
      {
        clNode2D *node = GetNodePointer(n);

        if(!node->IsHaloNode())
        {
          int       index     = indexList->GetItem(n);
          int       count     = 0;
          double    averageX  = 0.0f;
          double    averageY  = 0.0f;

          if(n<GetNumberNodes()-1)
          {
            // All nodes but last
            int nextIndex = indexList->GetItem(n+1);

            for(int nn=index; nn<nextIndex; nn++)
            {
              int       neighbourNodeID    = neighbourNodeList->GetItem(nn);
              int       neighbourNodeIndex = GetNodeIndex(neighbourNodeID);
              clNode2D *neighbourNode      = GetNodePointer(neighbourNodeIndex);

              averageX += neighbourNode->x();
              averageY += neighbourNode->y();

              count++;
            }
          }
          else
          {
            // Last node
            for(int nn=index; nn<GetNumberNodes(); nn++)
            {
              int       neighbourNodeID    = neighbourNodeList->GetItem(nn);
              int       neighbourNodeIndex = GetNodeIndex(neighbourNodeID);
              clNode2D *neighbourNode      = GetNodePointer(neighbourNodeIndex);

              averageX += neighbourNode->x();
              averageY += neighbourNode->y();

              count++;
            }
          }

          if(count>0)
          {
            averageX /= count;
            averageY /= count;
          }

          double deltaX = averageX - node->x();
          double deltaY = averageY - node->y();

          double newX = node->x() + omega*deltaX;
          double newY = node->y() + omega*deltaY;

          cout << "X = " << newX << " Y = " << newY << endl;

          node->SetCoordinates(newX, newY);

          maxDeltaX = (fabs(deltaX)>maxDeltaX ? fabs(deltaX) : maxDeltaX);
          maxDeltaY = (fabs(deltaY)>maxDeltaY ? fabs(deltaY) : maxDeltaY);
        }
      }

      double oldMaxDelta = maxDelta;
      maxDelta = (maxDeltaX > maxDeltaY) ? maxDeltaX : maxDeltaY;

      if(maxDelta>oldMaxDelta)
      {
        char message[MESLEN];
        sprintf(message, "Diverging after iteration = %d\n", iteration);
        throw clException("clUnstructured2D", "BalanceNodes", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::BalanceNodes");
    throw EX;
  }
}

void clUnstructured2D::WriteLegacyVTKGrid(void)
{
  try
  {
    WriteLegacyVTKGrid("unstructured2d.vtk");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clUnstructured2D::WriteLegacyVTKGrid(const string fileName)
{
  try
  {
    WriteLegacyVTKGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clUnstructured2D::WriteLegacyVTKGrid(const char* fileName)
{
  try
  {
    fstream fout;
    fout.open(fileName, ios::out);
    fout.precision(3);
    if (fout.good())
    {
      // HEADER
      fout << "# vtk DataFile Version 2.0" << endl;

      // TITLE
      fout << "Grid generated by gridLab. FileName: " << fileName << "\n";

      // DATA TYPE
      fout << "ASCII" << endl;

      // DATASET
      fout << "DATASET UNSTRUCTURED_GRID" << endl;

      // POINTS
      fout << "POINTS " << this->GetNumberNodes() << " double" << endl;
      for(int n=0; n<this->GetNumberNodes(); n++)
      {
        clNode2D *node = this->GetNodePointer(n);
        tpMatrix<double> coordinates = node->GetCoordinates();

        fout << coordinates(1,1);
        for(int r=2; r<=coordinates.GetNumberRows(); r++)
        {
          fout << " " << coordinates(r, 1);
        }
        fout << " 0" << endl; // Add 3rd Coordinate
      }
      fout << endl;

      // CELLS
      int numberElements = this->GetNumberElements() ;
      fout << "CELLS " << numberElements << " " << (4*numberElements) << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        clTriangle *element = this->GetElementPointer(e);
        fout << element->GetNumberNodes();
        for(int en=0; en<element->GetNumberNodes(); en++)
        {
          fout << " " << this->GetNodeIndex(element->GetNodeID(en));
        }
        fout << endl;
      }
      fout << endl;

      // CELL_TYPES
      fout << "CELL_TYPES " << numberElements << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        fout << "5" << endl;  // VTK_TRIANGLE
      }
      fout << endl;

      // DATASET ATTRIBUTES
      fout << "CELL_DATA " << numberElements << endl;
      fout << "SCALARS elementArea double 1"<< endl;
      fout << "LOOKUP_TABLE default" << endl;
      for(int e=0; e<numberElements; e++)
      {
        clTriangle *element = this->GetElementPointer(e);
        fout << element->GetArea() << endl;
      }
      fout << endl;

      fout.close();
    }
    else
    {
      throw clExceptionTree("clUnstructured2D", "WriteLegacyVTKGrid", "Could not open output file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured2D::WriteLegacyVTKGrid");
    throw EX;
  }
}

