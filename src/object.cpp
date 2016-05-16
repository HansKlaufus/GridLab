/**************************************************
*** Body file for definition of class clObject
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "exceptiontree.h"
#include "matrix.h"
#include "object.h"

clObject::clObject(void)
{
  SetObjectID(0);
  SetName("noName");
}

clObject::clObject(const int theID, const string theName)
{
  SetObjectID(theID);
  SetName(theName);
}

clObject::clObject(const clObject &source)
{
  Assign(source);
}

clObject::~clObject(void)
{
  //cerr << "DESTROYING OBJECT: " << GetObjectID() << endl;
}

clObject& clObject::Assign(const clObject &source)
{
  SetObjectID(source.GetObjectID());

  return(*this);
}

clObject& clObject::operator=(const clObject &source)
{
  return(Assign(source));
}

double clObject::GetMinX(void) const
{
  double minX = vertexList[0][0];

  try
  {
    for(unsigned int v=1; v<vertexList.size(); v++)
    {
      if(vertexList[v][0] < minX)
      {
        minX = vertexList[v][0];
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::GetMinX");
    throw EX;
  }

  return (minX);
}

double clObject::GetMaxX(void) const
{
  double maxX = vertexList[0][0];

  try
  {
    for(unsigned int v=1; v<vertexList.size(); v++)
    {
      if(vertexList[v][0] > maxX)
      {
        maxX = vertexList[v][0];
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::GetMaxX");
    throw EX;
  }

  return (maxX);
}

double clObject::GetMinY(void) const
{
  double minY = vertexList[0][1];

  try
  {
    for(unsigned int v=1; v<vertexList.size(); v++)
    {
      if(vertexList[v][1] < minY)
      {
        minY = vertexList[v][1];
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::GetMinY");
    throw EX;
  }

  return (minY);
}

double clObject::GetMaxY(void) const
{
  double maxY = vertexList[0][1];

  try
  {
    for(unsigned int v=1; v<vertexList.size(); v++)
    {
      if(vertexList[v][1] > maxY)
      {
        maxY = vertexList[v][1];
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::GetMaxY");
    throw EX;
  }

  return (maxY);
}

double clObject::GetMinZ(void) const
{
  double minZ = vertexList[0][2];

  try
  {
    for(unsigned int v=1; v<vertexList.size(); v++)
    {
      if(vertexList[v][2] < minZ)
      {
        minZ = vertexList[v][2];
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::GetMinZ");
    throw EX;
  }

  return (minZ);
}

double clObject::GetMaxZ(void) const
{
  double maxZ = vertexList[0][2];

  try
  {
    for(unsigned int v=1; v<vertexList.size(); v++)
    {
      if(vertexList[v][2] > maxZ)
      {
        maxZ = vertexList[v][2];
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::GetMaxZ");
    throw EX;
  }

  return (maxZ);
}

vector< vector<double> > clObject::GetVertices(void) const
{
  return(vertexList);
}

void clObject::ReadGTSGrid(const string fileName)
{
  try
  {
    ReadGTSGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::ReadGTSGrid");
    throw EX;
  }
}

void clObject::ReadGTSGrid(const char* fileName)
{
  try
  {
    ParseGTSGrid(fileName);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::ReadGTSGrid");
    throw EX;
  }
}

void clObject::ParseGTSGrid(const char* fileName)
{
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

        vector<double> coordinates;
        coordinates.push_back(x);
        coordinates.push_back(y);
        coordinates.push_back(z);

        vertexList.push_back(coordinates);
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read node: %f; %f; %f\n", x, y, z);

      int vertex1, vertex2;

      progress = 0;
      for(int e=0; e<numberEdges; e++)
      {
        if(100*e/numberEdges > progress)
        {
          progress = 100*e/numberEdges;
          printf("[%3d%%] Reading edges...\n", progress);
        }

        file >> vertex1;
        file >> vertex2;

        vector<int> edge;
        edge.push_back(vertex1);
        edge.push_back(vertex2);

        edgeList.push_back(edge);
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read edge: %d; %d\n", nodeID1, nodeID2);


      int edge1, edge2, edge3;

      progress = 0;
      for(int t=0; t<numberTriangles; t++)
      {
        if(100*t/numberTriangles > progress)
        {
          progress = 100*t/numberTriangles;
          printf("[%3d%%] Reading polygons...\n", progress);
        }

        file >> edge1;
        file >> edge2;
        file >> edge3;

        vector<int> polygon;
        polygon.push_back(edge1);
        polygon.push_back(edge2);
        polygon.push_back(edge3);

        polygonList.push_back(polygon);
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read triangle: %d; %d; %d\n", edge1, edge2, edge3);

      file.close();
    }
    else
    {
      throw clExceptionTree("clObject", "ParseGTSGrid", "Could not open input file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::ParseGTSGrid");
    throw EX;
  }
}

void clObject::WriteLegacyVTKFile(void)
{
  try
  {
    WriteLegacyVTKFile("object3d.vtk");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::WriteLegacyVTKfile");
    throw EX;
  }
}

void clObject::WriteLegacyVTKFile(const string fileName)
{
  try
  {
    WriteLegacyVTKFile(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::WriteLegacyVTKFile");
    throw EX;
  }
}

void clObject::WriteLegacyVTKFile(const char* fileName)
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
      fout << "DATASET POLYDATA" << endl;

      // POINTS
      fout << "POINTS " << vertexList.size() << " double" << endl;
      for(unsigned int v=0; v<vertexList.size(); v++)
      {
        for(unsigned int c=0; c<vertexList[v].size(); c++)
        {
          fout << vertexList[v][c] << " ";
        }
        for(unsigned int c=vertexList[v].size(); c<3; c++)
        {
          fout << "0.0 ";
        }
        fout << endl;
      }
      fout << endl;

      int cellListSize = 0;
      stringstream tempStream;  // Create temporary buffer, as to first to determine the cell list size
      for(unsigned int p=0; p<polygonList.size(); p++)
      {
        cellListSize++;
        tempStream << polygonList[p].size() << " ";

        for(unsigned int e=0; e<polygonList[p].size(); e++)
        {
          int edgeIndex   = polygonList[p][e];
          int vertexIndex = edgeList[edgeIndex][0];

          tempStream << vertexIndex << " ";

          cellListSize++;
        }
        tempStream << endl;
      }
      fout << "POLYGONS " << polygonList.size() << " " << cellListSize << endl;
      fout << tempStream.rdbuf(); // Dump temporary bugger to file
      fout << endl;

      fout.close();
    }
    else
    {
      throw clExceptionTree("clObject", "WriteLegacyVTKFile", "Could not open output file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject::WriteLegacyVTKFile");
    throw EX;
  }
}
