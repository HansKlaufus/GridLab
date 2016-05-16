/******************************************************
*** Body file for definition of class clUnstructured3D
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "exceptiontree.h"
#include "globals.h"
#include "matrix.h"
#include "node3d.h"
#include "stack.h"
#include "tetrahedron.h"
#include "unstructured3d.h"

using namespace std;

clUnstructured3D::clUnstructured3D(void) : tpUnstructured<clNode3D, clTetrahedron>()
{
  try
  {
    SetObjectType(otUnstructured3D);
    CreateInitialGrid();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::clUnstructured3D");
    throw EX;
  }
}

clUnstructured3D::clUnstructured3D(double minX, double maxX, double minY, double maxY, double minZ, double maxZ) : tpUnstructured<clNode3D, clTetrahedron>()
{
  try
  {
    SetObjectType(otUnstructured3D);
    CreateInitialGrid(minX, maxX, minY, maxY, minZ, maxZ);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::clUnstructured3D");
    throw EX;
  }
}

clUnstructured3D::clUnstructured3D(const int ID) : tpUnstructured<clNode3D, clTetrahedron>(ID)
{
  try
  {
    SetObjectType(otUnstructured3D);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::clUnstructured3D");
    throw EX;
  }
}

//clUnstructured3D::clUnstructured3D(const clUnstructured3D &source)
//{
//  try
//  {
//  }
//
//  catch(clExceptionTree EX)
//  {
//    EX.AddMethodToTree("clUnstructured3D::CreateGrid");
//    throw EX;
//  }
//}

clUnstructured3D::~clUnstructured3D(void)
{
  //cerr << "DESTROYING GRID: " << GetNumberID() << endl;
}

clTetrahedron *clUnstructured3D::CreateElement(void)
{
  clTetrahedron *element = new clTetrahedron(this, GetNewElementID());

  return(element);
}

int clUnstructured3D::CreateInitialGrid(void)
{
  try
  {
    CreateInitialGrid(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::CreateInitialGrid");
    throw EX;
  }

  return(GetNumberElements());
}

int clUnstructured3D::CreateInitialGrid(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
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

    if((maxZ - minZ) < PRECISION)
    {
      minZ = -2.0f;
      maxZ = +2.0f;
    }

    // Create notes for enbounding box
    int entry = 0;
    entry+=AddNode(minX, minY, minZ);
    entry+=AddNode(maxX, minY, minZ);
    entry+=AddNode(maxX, maxY, minZ);
    entry+=AddNode(minX, maxY, minZ);
    entry+=AddNode(minX, minY, maxZ);
    entry+=AddNode(maxX, minY, maxZ);
    entry+=AddNode(maxX, maxY, maxZ);
    entry+=AddNode(minX, maxY, maxZ);

    if(entry!=36)
    {
      char message[MESLEN];
      sprintf(message, "Entry total for nodes does not match.\n  TOTAL=%d [36]", entry);
      throw clExceptionTree("clUnstructured3D", "CreateInitialGrid", message);
    }

    // Setup the elements
    entry=0;
    {int nodes[]={-1, 6, 8, 7}; int faces[]={16, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 1

    {int nodes[]={-1, 5, 8, 6}; int faces[]={15, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 2

    {int nodes[]={-1, 1, 8, 5}; int faces[]={15, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 3

    {int nodes[]={-1, 1, 4, 8}; int faces[]={14, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 4

    {int nodes[]={-1, 4, 3, 8}; int faces[]={14, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 5

    {int nodes[]={-1, 3, 7, 8}; int faces[]={16, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 6

    {int nodes[]={-1, 3, 6, 7}; int faces[]={16, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 7

    {int nodes[]={-1, 2, 6, 3}; int faces[]={13, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 8

    {int nodes[]={-1, 1, 5, 6}; int faces[]={15, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 9

    {int nodes[]={-1, 1, 6, 2}; int faces[]={13, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 10

    {int nodes[]={-1, 1, 3, 4}; int faces[]={14, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 11

    {int nodes[]={-1, 1, 2, 3}; int faces[]={13, -1, -1, -1};
    entry+=AddElement(faces, nodes);} // 12

    {int nodes[]={1, 2, 3, 6}; int faces[]={8, 17, 10, 12};
    entry+=AddElement(faces, nodes);} // 13

    {int nodes[]={1, 3, 4, 8}; int faces[]={5, 4, 17, 11};
    entry+=AddElement(faces, nodes);} // 14

    {int nodes[]={1, 5, 6, 8}; int faces[]={2, 17, 3, 9};
    entry+=AddElement(faces, nodes);} // 15

    {int nodes[]={3, 6, 7, 8}; int faces[]={1, 6, 17, 7};
    entry+=AddElement(faces, nodes);} // 16

    {int nodes[]={1, 3, 8, 6}; int faces[]={16, 15, 13, 14};
    entry+=AddElement(faces, nodes);} // 17

    if(entry!=153)
    {
      char message[MESLEN];
      sprintf(message, "Entry total for elements does not match.\n  TOTAL=%d [153]", entry);
      throw clExceptionTree("clUnstructured3D", "CreateInitialGrid", message);
    }

    // Protect Halo Nodes
    for(int n=0; n<GetNumberNodes(); n++)
    {
      clNode3D *theNode = GetNodePointer(n);
      theNode->SetHaloNode();
    }

    // Protect Halo cells
    for(int e=0; e<GetNumberElements(); e++)
    {
      clTetrahedron *theElement = GetElementPointer(e);
      if(e<12)
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
    EX.AddMethodToTree("clUnstructured3D::CreateInitialGrid");
    throw EX;
  }

  return(GetNumberElements());
}

/***************************************************************
***
***   Performing Node Characteristic Operation
***
**/

int clUnstructured3D::AddNode(const double x, const double y, const double z)
{
  //
  // IN : x         = x-coordinate of new node
  //      y         = y-coordinate of new node
  //      z         = z-coordinate of new node
  // OUT: newNodeID = ID of new node
  //

  int newNodeID = 1;

  try
  {
    if(GetNumberNodes()>0)
    {
      clNode3D  *lastNode = GetNodePointer(GetNumberNodes()-1);
      newNodeID           = lastNode->GetNodeID()+1;
    }

    clNode3D *newNode = new clNode3D(newNodeID, x, y, z);
    AddNode(newNode);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::AddNode");
    throw EX;
  }

  return(newNodeID);
}

int clUnstructured3D::AddNode(const double coord[3])
{
  //
  // IN : coord[3]  = coordinates of new node
  // OUT: newNodeID = ID of new node
  //
  int newNodeID = 1;

  try
  {
    newNodeID = AddNode(coord[0], coord[1], coord[2]);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::AddNode");
    throw EX;
  }

  return(newNodeID);
}

/***************************************************************
***
***  Perforing Element Characteristic Operations
***
**/

//TODO: Make more general instead programming '4' etc.
int clUnstructured3D::AddElement(const int faces[4], const int nodes[4])
{
  //
  // IN : faces         = IDs of neighbouring elements
  //      nodes         = IDs of nodes
  // OUT: newElementID  = ID of new element
  //

  int newElementID = 1;

  try
  {
    newElementID = AddElement(faces[0], faces[1], faces[2], faces[3], nodes[0], nodes[1], nodes[2], nodes[3]);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::AddElement");
    throw EX;
  }

  return(newElementID);
}

//TODO: Make more general instead programming '4' etc.
int clUnstructured3D::AddElement(const int face1, const int face2, const int face3, const int face4,
                                 const int node1, const int node2, const int node3, const int node4)
{
  //
  // IN : face1         = ID of element, neighbouring the face formed by vertices V2, V3, V4
  //      face2         = ID of element, neighbouring the face formed by vertices V1, V4, V3
  //      face3         = ID of element, neighbouring the face formed by vertices V1, V2, V4
  //      face4         = ID of element, neighbouring the face formed by vertices V1, V3, V2
  //      node1         = ID of node1
  //      node2         = ID of node2
  //      node3         = ID of node3
  //      node4         = ID of node4
  // OUT: newElementID  = ID of new element
  //

  int newElementID = 1;

  try
  {
    if(GetNumberElements()>0)
    {
      newElementID = GetNewElementID();
    }

    clTetrahedron *newElement = new clTetrahedron(this, newElementID);

    newElement->SetNeighbourID(0, face1);
    newElement->SetNeighbourID(1, face2);
    newElement->SetNeighbourID(2, face3);
    newElement->SetNeighbourID(3, face4);

    newElement->SetNodeID(0, node1);
    newElement->SetNodeID(1, node2);
    newElement->SetNodeID(2, node3);
    newElement->SetNodeID(3, node4);

    AddElement(newElement);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::AddElement");
    throw EX;
  }

  return(newElementID);
}

void clUnstructured3D::WriteGnuplotGrid(void)
{
  try
  {
    WriteGnuplotGrid("volumeGrid.gnu");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::WriteGnuplotGrid");
    throw EX;
  }
}

void clUnstructured3D::WriteGnuplotGrid(const string fileName)
{
  try
  {
    WriteGnuplotGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::WriteGnuplotGrid");
    throw EX;
  }
}

void clUnstructured3D::WriteGnuplotGrid(const char* fileName)
{
  try
  {
    fstream fout;
    fout.open(fileName, ios::out);
    fout.precision(3);
    if (fout.good())
    {
      fout << "#\n# Unstructured 3D grid created by gridLab\n#\n";

      for(int e=0; e<this->GetNumberElements(); e++)
      {
        clTetrahedron *element = this->GetElementPointer(e);

        if(!element->IsHaloCell())
        {
          fout << "# ELEMENT ID: " << element->GetElementID() << "\n";
          for(int f=0; f<element->GetNumberFaces(); f++)
          {
            int numberFaceNodes = element->GetNumberFaceNodes();
            for(int n=0; n<numberFaceNodes+1; n++)
            {
              int       nodeNumber = element->GetFaceNode(f, n%numberFaceNodes);
              int       nodeID     = element->GetNodeID(nodeNumber);
              int       nodeIndex  = this->GetNodeIndex(nodeID);
              clNode3D *node       = this->GetNodePointer(nodeIndex);

              fout << node->x() << " " << node->y() << " " << node->z() << " '" << nodeID <<"'\n";
            }

            fout << "\n";
          }
        }
      }

      fout.close();
    }
    else
    {
      throw clExceptionTree("clUnstructured3D", "WriteGnuplotGrid", "Could not open output file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::WriteGnuplotGrid");
    throw EX;
  }
}

void clUnstructured3D::WriteLegacyVTKGrid(void)
{
  try
  {
    WriteLegacyVTKGrid("unstructured3d.vtk");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clUnstructured3D::WriteLegacyVTKGrid(const string fileName)
{
  try
  {
    WriteLegacyVTKGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clUnstructured3D::WriteLegacyVTKGrid(const char* fileName)
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
        clNode3D *node = this->GetNodePointer(n);
        tpMatrix<double> coordinates = node->GetCoordinates();

        fout << coordinates(1,1);
        for(int r=2; r<=coordinates.GetNumberRows(); r++)
        {
          fout << " " << coordinates(r, 1);
        }
        fout << endl;
      }
      fout << endl;

      // CELLS
      fout << "CELLS " << this->GetNumberElements() << " " << (this->GetNumberElements()+4*this->GetNumberElements()) << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        clTetrahedron *element = this->GetElementPointer(e);
        fout << element->GetNumberNodes();
        for(int en=0; en<element->GetNumberNodes(); en++)
        {
          fout << " " << this->GetNodeIndex(element->GetNodeID(en));
        }
        fout << endl;
      }
      fout << endl;

      // CELL_TYPES
      fout << "CELL_TYPES " << this->GetNumberElements() << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        fout << "10" << endl;  // VTK_TETRA
      }
      fout << endl;

      // DATASET ATTRIBUTES
      fout << "CELL_DATA " << this->GetNumberElements() << endl;
      fout << "SCALARS elementVolume double 1"<< endl;
      fout << "LOOKUP_TABLE default" << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        clTetrahedron *element = this->GetElementPointer(e);
        fout << element->GetVolume() << endl;
      }
      fout << endl;

      fout.close();
    }
    else
    {
      throw clExceptionTree("clUnstructured3D", "WriteLegacyVTKGrid", "Could not open output file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::WriteLegacyVTKGrid");
    throw EX;
  }
}
