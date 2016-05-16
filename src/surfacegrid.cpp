/********************************************************
*** Body file for definition of class clSurfaceGrid
***
*** J.L. Klaufus - Haarlem 2009
**/

#include <fstream>
#include <map>
#include <string>

#include <stdlib.h>

#include "exception.h"
#include "geometry.h"
#include "globals.h"
#include "matrix.h"
#include "node2d.h"
#include "str.h"
#include "surface.h"
#include "surfacegrid.h"
#include "unstructured2d.h"

clSurfaceGrid::clSurfaceGrid(void) : tpGrid<clNode3D, clTriangle>()
{
  this->SetObjectType(otSurfaceGrid);
}

clSurfaceGrid::clSurfaceGrid(const int ID, const string theName) : tpGrid<clNode3D, clTriangle>(ID, theName)
{
  this->SetObjectType(otSurfaceGrid);
}

clSurfaceGrid::clSurfaceGrid(const clSurfaceGrid &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::clSurfaceGrid");
    throw EX;
  }
}

clSurfaceGrid::~clSurfaceGrid(void)
{
  //cerr << "DESTROYING SURFACEGRID: " << GetNumberID() << endl;
}

clSurfaceGrid& clSurfaceGrid::Assign(const clSurfaceGrid &source)
{
  try
  {
    for(int n=0; n<source.GetNumberNodes(); n++)
    {
      clNode3D *node = new clNode3D(source.GetNode(n));
      AddNode(node);
    }

    for(int e=0; e<source.GetNumberElements(); e++)
    {
      clTriangle *element = new clTriangle(source.GetElement(e));
      AddElement(element);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::Assign");
    throw EX;
  }

  return(*this);
}

clSurfaceGrid& clSurfaceGrid::operator=(const clSurfaceGrid &source)
{
  return(Assign(source));
}

double clSurfaceGrid::GetMinX(void) const
{
  return (GetMinCoordinate(_X_));
}

double clSurfaceGrid::GetMaxX(void) const
{
  return (GetMaxCoordinate(_X_));
}

double clSurfaceGrid::GetMinY(void) const
{
  return (GetMinCoordinate(_Y_));
}

double clSurfaceGrid::GetMaxY(void) const
{
  return (GetMaxCoordinate(_Y_));
}

double clSurfaceGrid::GetMinZ(void) const
{
  return (GetMinCoordinate(_Z_));
}

double clSurfaceGrid::GetMaxZ(void) const
{
  return (GetMaxCoordinate(_Z_));
}

void clSurfaceGrid::CreateGrid(void)
{
  //TODO: Create surfacegrid automatically
}

void clSurfaceGrid::CreateUnstructured(clSurface *surface, const int numU, const int numV, const enDistributionType distributionType)
{
  try
  {
    // Create a distribution
    dMatrix nodesU;
    dMatrix nodesV;
    Distribute(distributionType, numU, numV, nodesU, nodesV);

    // Create a 2d grid + initial grid
    clUnstructured2D unstructured2d(-2.0f, 2.0f, -2.0f, 2.0f);
    for(int v=0; v<numV; v++)
    {
      for(int u=0; u<numU; u++)
      {
        // Create a new node
        clNode2D *newNode = new clNode2D(unstructured2d.GetNewNodeID(), nodesU(u+1,1), nodesV(v+1,1));

        if(v==0 || v==numV-1 || u==0 || u==numU-1)
        {
          newNode->SetBoundaryNode();
        }

        unstructured2d.InsertNode(newNode);
      }
    }

    // Perform some maintenance
//    unstructured2d.CorrectGrid();
    unstructured2d.DeleteInitialGrid();
//    unstructured2d.Smooth();
    unstructured2d.CheckGrid();

    // Create 3D parametric grid
    MergeGrid(surface, &unstructured2d);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::CreateUnstructured");
    throw EX;
  }
}

void clSurfaceGrid::CreateUnstructured(clGeometry *geometry, const int numU, const int numV, const enDistributionType distributionType)
{
  try
  {
    int numberSurfaces = geometry->GetNumberSurfaces();
    for(int s=0; s<numberSurfaces; s++)
    {
      clSurface *surface = geometry->GetSurfacePointer(s);
      CreateUnstructured(surface, numU, numV, distributionType);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::CreateUnstructured");
    throw EX;
  }
}

void clSurfaceGrid::Distribute(const enDistributionType dType, const int numU, const int numV, dMatrix &nodesU, dMatrix &nodesV) const
{
  nodesU.SetNumberRows(numU);
  nodesU.SetNumberColumns(1);

  nodesV.SetNumberRows(numV);
  nodesV.SetNumberColumns(1);

  if(dType==dtUniform)
  {
    // Distribution in U
    double deltaU = 1.0/(numU-1);

    for(int r=1; r<=numU; r++)
    {
      nodesU.SetElement(r, 1, (r-1)*deltaU);
    }

    // Distribution in V
    double deltaV = 1.0/(numV-1);

    {
      for(int r=1; r<=numV; r++)
      {
        nodesV.SetElement(r, 1, (r-1)*deltaV);
      }
    }
  }
  else if(dType==dtNonUniform)
  {
    double mu    = 0.5;
    double sigma = 0.95/4;

    // Create a normal distribution in U
    double totalDeltaU = 0.0;
    double deltaU[numU];

    deltaU[0] = 0.0;
    for(int r=1; r<numU; r++)
    {
      double u      = (double)(r)/(double)(numU-1);
      double factor = (double)(1)/(sigma*sqrt(2*PI));
      double num    = -pow(u-mu,2);
      double den    = 2*pow(sigma,2);

      deltaU[r]     = pow(2*factor, num/den);
      totalDeltaU  += deltaU[r];
    }

    // Correct and fill nodesU
    nodesU.SetElement(1, 1, 0.0);
    {
      for(int r=1; r<numU; r++)
      {
        nodesU.SetElement(r+1, 1, nodesU(r, 1)+deltaU[r]/totalDeltaU);
      }
    }

    // Create a normal distribution in V
    double totalDeltaV = 0.0;
    double deltaV[numV];

    deltaV[0] = 0.0;
    {
      for(int r=1; r<numV; r++)
      {
        double v      = (double)(r)/(double)(numV-1);
        double factor = (double)(1)/(sigma*sqrt(2*PI));
        double num    = -pow(v-mu,2);
        double den    = 2*pow(sigma,2);

        deltaV[r]     = pow(2*factor, num/den);
        totalDeltaV  += deltaV[r];
      }
    }

    // Correct and fill nodesV
    nodesV.SetElement(1, 1, 0.0);
    {
      for(int r=1; r<numV; r++)
      {
        nodesV.SetElement(r+1, 1, nodesV(r, 1)+deltaV[r]/totalDeltaV);
      }
    }
  }
  else if(dType==dtUserDefined)
  {
    throw clException("clSurfaceGrid", "Distribute", "DistributionType set to UserDefined: Invalid call to method Distribute.");
  }
  else
  {
    throw clException("clSurfaceGrid", "Distribute", "Unknown DistributionType.");
  }
}

void clSurfaceGrid::MergeGrid(clSurface *surface, clUnstructured2D *unstructured2d)
{
  try
  {
    InsertGrid(surface, unstructured2d);
    DeleteDuplicateNodes();
    DeleteInferiorElements();
    StitchElements();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::StitchGrids");
    throw EX;
  }
}

void clSurfaceGrid::InsertGrid(clSurface *surface, clUnstructured2D *unstructured2d)
{
  /*
    NOTE:
    Nodes might be different in the parametric space, but identical in the 3d space.
    This can cause flat triangles...
    Example is Sphere.geo
  */

  try
  {
    //
    // Insert Nodes
    //

    // Create a dictionary for the nodes
    map<int, int> nodeDictionary;

    // Copy nodes to surface grid, and update nodeIDs on the fly to ensure uniqueness
    for(int n=0; n<unstructured2d->GetNumberNodes(); n++)
    {
      clNode2D *sourceNode  = unstructured2d->GetNodePointer(n);
      dMatrix   coord3d     = surface->GetCoordinates(sourceNode->x(), sourceNode->y());

      int       newNodeID   = this->GetNewNodeID();
      clNode3D *newNode     = new clNode3D(newNodeID, coord3d);

      // Add node to dictionary
      nodeDictionary.insert(pair<int, int>(sourceNode->GetNodeID(), newNodeID));

      //Verify nodeID
      if(AddNode(newNode) != newNodeID)
      {
        cerr << "Warning: NodeID changed" << endl;
      }
    }


    //
    // Insert Elements
    //

    // Create a dictionary for the elements
    map<int, int> elementDictionary;

    // Copy elements to surface grid, and update elementIDs on the fly to ensure uniqueness
    for(int e=0; e<unstructured2d->GetNumberElements(); e++)
    {
      clTriangle *sourceElement = unstructured2d->GetElementPointer(e);

      int         newElementID  = this->GetNewElementID();
      clTriangle *newElement    = new clTriangle((clUnstructured2D*) this, newElementID);

      // Add element to dictionary
      elementDictionary.insert(pair<int, int>(sourceElement->GetElementID(), newElementID));

      // Connect to new neighbour elements
      for(int f=0; f<sourceElement->GetNumberFaces(); f++)
      {
        int sourceNeighbourID = sourceElement->GetNeighbourID(f);

        if(sourceNeighbourID>=0)
        {
          int         sourceNeighbourIndex = unstructured2d->GetElementIndex(sourceNeighbourID);
          clTriangle *sourceNeighbour      = unstructured2d->GetElementPointer(sourceNeighbourIndex);

          // Search for neighbourFace
          int nf = 0;
          while((nf<sourceElement->GetNumberFaces())&&!this->IsEqualFace(sourceElement, f, sourceNeighbour, nf))
          {
             nf++;
          }

          if(nf==sourceElement->GetNumberFaces())
          {
            throw clExceptionTree("clSurfaceGrid", "InsertGrid", "Could not find neighbourFace.");
          }

          // Update the references to neighbour elements
          // NOTE: it is possible that the neighbour element IDs are only updated during a next iteration
          //       Therefore the references might be invalid temporarily
          map<int, int>::iterator theIterator = elementDictionary.find(sourceNeighbourID);
          if(theIterator!=elementDictionary.end())
          {
            // Create reference to/from newNeighbour
            int         newNeighbourID      = theIterator->second;
            int         newNeighbourIndex   = this->GetElementIndex(newNeighbourID);
            clTriangle *newNeighbourElement = this->GetElementPointer(newNeighbourIndex);

            newNeighbourElement->SetNeighbourID(nf, newElementID);
            newElement->SetNeighbourID(f, newNeighbourID);
          }
          else
          {
            // Temporarily set neighbour to -1
            newElement->SetNeighbourID(f, -1);
          }
        }
      }

      // Connect to the new nodes
      for(int n=0; n<sourceElement->GetNumberNodes(); n++)
      {
        int nodeID = sourceElement->GetNodeID(n);
        if(nodeID>=0)
        {
          map<int, int>::iterator theIterator = nodeDictionary.find(nodeID);
          if(theIterator!=nodeDictionary.end())
          {
            int newNodeID = theIterator->second;
            newElement->SetNodeID(n, newNodeID);
          }
          else
          {
            throw clExceptionTree("clSurfaceGrid", "InsertGrid", "Could not find node.");
          }
        }
      }

      // Verify elementID
      if(AddElement(newElement)!=newElementID)
      {
        cerr << "Warning: ElementID changed" << endl;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::InsertGrid");
    throw EX;
  }
}

void clSurfaceGrid::DumpGrid(void)
{
  try
  {
    DumpGrid("surfaceGrid.csv");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::DumpGrid");
    throw EX;
  }
}

void clSurfaceGrid::DumpGrid(const string fileName)
{
  try
  {
    DumpGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::DumpGrid");
    throw EX;
  }
}

void clSurfaceGrid::DumpGrid(const char* fileName)
{
  try
  {
    fstream fout;
    fout.open(fileName, ios::out);
    fout.precision(3);
    if (fout.good())
    {
      DumpNodes(fout);
      DumpElements(fout);
      //TODO: DumpAreas, etc.

      fout.close();
    }
    else
    {
      throw clExceptionTree("clSurfaceGrid", "DumpGrid", "Could not open dump-file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::DumpGrid");
    throw EX;
  }
}

void clSurfaceGrid::WriteGnuplotGrid(void)
{
  try
  {
    WriteGnuplotGrid("surfaceGrid.gnu");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::WriteGnuplotGrid");
    throw EX;
  }
}

void clSurfaceGrid::WriteGnuplotGrid(const string fileName)
{
  try
  {
    WriteGnuplotGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::WriteGnuplotGrid");
    throw EX;
  }
}

void clSurfaceGrid::WriteGnuplotGrid(const char* fileName)
{
  try
  {
    fstream fout;
    fout.open(fileName, ios::out);
    fout.precision(3);
    if (fout.good())
    {
      fout << "#\n# surfaceGrid created by gridLab\n#\n";

      for(int e=0; e<this->GetNumberElements(); e++)
      {
        clTriangle *element = this->GetElementPointer(e);

        if(!element->IsHaloCell())
        {
          fout << "# ELEMENT ID: " << element->GetElementID() << "\n";
          int numberNodes = element->GetNumberNodes();
          for(int n=0; n<numberNodes+1; n++)
          {
            int       nodeID    = element->GetNodeID(n%numberNodes);
            int       nodeIndex = this->GetNodeIndex(nodeID);
            clNode3D *node = this->GetNodePointer(nodeIndex);

            fout << node->x() << " " << node->y() << " " << node->z() << "\n";
          }

          fout << "\n";
        }
      }

      fout.close();
    }
    else
    {
      throw clExceptionTree("clSurfaceGrid", "WriteGnuplotGrid", "Could not open output file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::WriteGnuplotGrid");
    throw EX;
  }
}

void clSurfaceGrid::ReadGTSGrid(const std::string fileName)
{
  // Reads GTS files: GNU Triangulated Surface
  // See: gts.sourcegorge.net

  try
  {
    ReadGTSGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::ReadGTSGrid");
    throw EX;
  }
}

void clSurfaceGrid::ReadGTSGrid(const char* fileName)
{
  // Reads GTS files: GNU Triangulated Surface
  // See: gts.sourcegorge.net

  try
  {
    ParseGTSGrid(fileName);
    DeleteDuplicateNodes();
    StitchElements();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::ReadGTSGrid");
    throw EX;
  }
}

void clSurfaceGrid::ParseGTSGrid(const char* fileName)
{
  // Parses GTS files: GNU Triangulated Surface
  // See: gts.sourcegorge.net

  try
  {
    fstream file;
    file.open(fileName, ios::in);
    file.precision(3);
    if (file.good())
    {
      int progress;

      int numberVertices;
      int numberEdges;
      int numberTriangles;

      file >> numberVertices;
      file >> numberEdges;
      file >> numberTriangles;

      cout << "Vertices:  " << numberVertices << endl;
      cout << "Edges:     " << numberEdges << endl;
      cout << "Triangles: " << numberTriangles << endl;

      double x, y, z;

      progress = 0;
      for(int v=0; v<numberVertices; v++)
      {
        if(100*v/numberVertices > progress)
        {
          progress = 100*v/numberVertices;
          printf("[%3d%%] Reading vertices...\n", progress);
        }

        file >> x;
        file >> y;
        file >> z;

//        cout << "X: " << x << "; Y: " << y <<  "; Z: " << z << endl;

        clNode3D *newNode = new clNode3D(v, x, y, z);
        this->AddNode(newNode);
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read node: %f; %f; %f\n", x, y, z);


      int nodeID1, nodeID2;
      int edgeArray[numberEdges][2]; // 2 = numberNodes per edge

      progress = 0;
      for(int e=0; e<numberEdges; e++)
      {
        if(100*e/numberEdges > progress)
        {
          progress = 100*e/numberEdges;
          printf("[%3d%%] Reading edges...\n", progress);
        }

        file >> nodeID1;
        file >> nodeID2;

        edgeArray[e][0] = nodeID1-1;
        edgeArray[e][1] = nodeID2-1;
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read edge: %d; %d\n", nodeID1, nodeID2);


      progress = 0;
      for(int t=0; t<numberTriangles; t++)
      {
        if(100*t/numberTriangles > progress)
        {
          progress = 100*t/numberTriangles;
          printf("[%3d%%] Reading triangles...\n", progress);
        }

        int edges[3];

        file >> edges[0];
        file >> edges[1];
        file >> edges[2];

//        cout << "Edge 1: " << edge1 << "; Edge 2: " << edge2 << "; Edge 3: " << edge3 << endl;
//        cout << "T: " << t << "; Node 1: " << edgeArray[edge1][0] << "; Node 2: " << edgeArray[edge2][0] << "; Node 3: " << edgeArray[edge3][0] << endl;

        clTriangle *newTriangle = new clTriangle((clUnstructured2D*) this, t);

        vector<int> nodes;
        for(int e=0; e<3; e++)
        {
          for(int n=0; n<2; n++)
          {
            if(find(nodes.begin(), nodes.end(), edgeArray[edges[e]-1][n]) == nodes.end())
            {
              nodes.push_back(edgeArray[edges[e]-1][n]);
            }
          }
        }

        newTriangle->SetNodeID(0, nodes[0]);
        newTriangle->SetNodeID(1, nodes[1]);
        newTriangle->SetNodeID(2, nodes[2]);

        this->AddElement(newTriangle);
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read triangle: %d; %d; %d\n", edge1, edge2, edge3);

      file.close();
    }
    else
    {
      throw clExceptionTree("clSurfaceGrid", "ParseGTSGrid", "Could not open input file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::ParseGTSGrid");
    throw EX;
  }
}

void clSurfaceGrid::WriteLegacyVTKGrid(void)
{
  // Writes VTK files: the Visualisation Toolkit (incl. Paraview)
  // See: http://www.vtk.org/

  try
  {
    WriteLegacyVTKGrid("surfaceGrid.vtk");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clSurfaceGrid::WriteLegacyVTKGrid(const string fileName)
{
  // Writes VTK files: the Visualisation Toolkit (incl. Paraview)
  // See: http://www.vtk.org/

  try
  {
    WriteLegacyVTKGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clSurfaceGrid::WriteLegacyVTKGrid(const char* fileName)
{
  // Writes VTK files: the Visualisation Toolkit (incl. Paraview)
  // See: http://www.vtk.org/

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
      fout << "CELL_TYPES " << this->GetNumberElements() << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        fout << "5" << endl;  // VTK_TRIANGLE
      }
      fout << endl;

      // DATASET ATTRIBUTES
      fout << "CELL_DATA " << this->GetNumberElements() << endl;
      fout << "SCALARS elementArea double 1"<< endl;
      fout << "LOOKUP_TABLE default" << endl;
      for(int e=0; e<this->GetNumberElements(); e++)
      {
        fout << 0.0f << endl;
      }
      fout << endl;

      fout.close();
    }
    else
    {
      throw clExceptionTree("clSurfaceGrid", "WriteLegacyVTKGrid", "Could not open output file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::WriteLegacyVTKGrid");
    throw EX;
  }
}

void clSurfaceGrid::ReadOBJGrid(const std::string fileName)
{
  // Reads OBJ files: Wavefront obj files
  // See: http://en.wikipedia.org/wiki/Wavefront_.obj_file

  try
  {
    ReadOBJGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::ReadOBJGrid");
    throw EX;
  }
}

void clSurfaceGrid::ReadOBJGrid(const char* fileName)
{
  // Reads OBJ files: Wavefront obj files
  // See: http://en.wikipedia.org/wiki/Wavefront_.obj_file

  try
  {
    ParseOBJGrid(fileName);
//    DeleteDuplicateNodes();
//    StitchElements();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::ReadOBJGrid");
    throw EX;
  }
}

void clSurfaceGrid::ParseOBJGrid(const char* fileName)
{
  // Parses OBJ files: Wavefront obj files
  // See: http://en.wikipedia.org/wiki/Wavefront_.obj_file

  int numberVertices  = 0;
  int numberTriangles = 0;

  try
  {
    std::cout << "Reading OBJ file..." << std::endl;

    fstream file;
    file.open(fileName, ios::in);
    file.precision(3);
    if (file.good())
    {
      // To print progress
      file.seekg(0, file.end);
      int  length = file.tellg();
      file.seekg(file.beg);

      int newPosition = 0;
      int oldPosition = 0;

      std::string token;
      while(!file.eof())
      {
          token = "";
          file >> token;

          // Print progress
          newPosition = file.tellg()*100.0/length;
          if((newPosition - oldPosition)>2)
          {
              std::cout << "Progress: " << newPosition << "%" << std::endl;
              oldPosition = newPosition;
          }

          if(token == "")
          {
              // Empty line
          }
          else if(token == "#")
          {
              // Comment, skip rest of line
              std::string line;
              getline(file, line);
          }
          else if(token == "mtllib")
          {
              // materials library, skip rest of line
              std::string line;
              getline(file, line);
          }
          else if(token == "usemtl")
          {
              // materials library, skip rest of line
              std::string line;
              getline(file, line);
          }
          else if(token == "v")
          {
              // Read vertex
              numberVertices++;

              double x, y, z;
              file >> x;
              file >> y;
              file >> z;

//              std::cout << token << "; " << x << ";  " << y << ";  " << z << ";  " << std::endl; //DEBUG

              clNode3D *newNode = new clNode3D(numberVertices, x, y, z);
              this->AddNode(newNode);
          }
          else if (token == "vt")
          {
              // Skip texture coordinates of vertex
              std::string line;
              getline(file, line);
          }
          else if (token == "vn")
          {
              // Skip normal coordinates of vertex
              std::string line;
              getline(file, line);
          }
          else if (token == "vp")
          {
              // Skip parameter space coordinates
              std::string line;
              getline(file, line);
          }
          else if (token == "f")
          {
              //Read face definition
              numberTriangles++;

              // TODO: in general a face can have more than three vertices
              std::string s1, s2, s3;
              file >> s1;
              file >> s2;
              file >> s3;

              // Find delimeter /, only use integer before first delimeter
              if(s1.find("/")>0)
              {
                s1 = s1.substr(0, s1.find("/"));
              }

              if(s2.find("/")>0)
              {
                s2 = s2.substr(0, s2.find("/"));
              }

              if(s3.find("/")>0)
              {
                s3 = s3.substr(0, s3.find("/"));
              }

              int n1, n2, n3;
              n1 = atoi(s1.c_str())-1;
              n2 = atoi(s2.c_str())-1;
              n3 = atoi(s3.c_str())-1;

//              std::cout << token << "; " << n1 << ";  " << n2 << ";  " << n2 << ";  " << std::endl; //DEBUG

              clTriangle *newTriangle = new clTriangle((clUnstructured2D*) this, numberTriangles);
              newTriangle->SetNodeID(0, this->GetNodePointer(n1)->GetNodeID());
              newTriangle->SetNodeID(1, this->GetNodePointer(n2)->GetNodeID());
              newTriangle->SetNodeID(2, this->GetNodePointer(n3)->GetNodeID());

              this->AddElement(newTriangle);
          }
          else
          {
            throw clExceptionTree("clSurfaceGrid", "ParseOBJGrid", "ERROR: Unknown token.");
          }
      }
      file.close();

      std::cout << "Number Vertices: "<< numberVertices  << std::endl;
      std::cout << "Number Faces:    "<< numberTriangles << std::endl;
    }
    else
    {
      throw clExceptionTree("clSurfaceGrid", "ParseOBJGrid", "ERROR: Could not open input file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGrid::ParseOBJGrid");
    throw EX;
  }
}
