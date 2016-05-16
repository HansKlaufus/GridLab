/**************************************************
*** Body file for definition of class clObject2D
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <fstream>
#include <math.h>
#include <string>
#include <vector>

#include "exceptiontree.h"
#include "object2d.h"

clObject2D::clObject2D(void) : clObject()
{
}

clObject2D::clObject2D(const int theID, const string theName) : clObject(theID, theName)
{
}

clObject2D::clObject2D(const clObject2D &source)
{
  Assign(source);
}

clObject2D::~clObject2D(void)
{
  //cerr << "DESTROYING OBJECT: " << GetObjectID() << endl;
}

clObject2D& clObject2D::Assign(const clObject2D &source)
{
  SetObjectID(source.GetObjectID());

  return(*this);
}

bool clObject2D::PointInPolygon(int polygonIndex, double x, double y)
{
  /*
  **
  ** Implements Jordan curve theorem
  **
  */

  // See: http://en.wikipedia.org/wiki/Point_in_polygon
  // See: http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
  // See: http://www.alienryderflex.com/polygon/

  bool oddNodes = false;
//  int  count    = 0;

  // Loop through all edges in this polygon
  for(unsigned int edge=0; edge<polygonList[polygonIndex].size(); edge++)
  {
    int edgeIndex    = polygonList[polygonIndex][edge];

    int v1           = edgeList[edgeIndex][0];
    int v2           = edgeList[edgeIndex][1];

    double v1x       = vertexList[v1][0];
    double v1y       = vertexList[v1][1];

    double v2x       = vertexList[v2][0];
    double v2y       = vertexList[v2][1];

    // Check if y fals within the edge range [v1y, v2y]
    if((v1y > y) != (v2y > y))
    {
      // Check at which x coordinate this edge crosses the imaginary horizontal line through y
      double xx =  (v1x + (v2x - v1x) * (y - v1y) / (v2y - v1y));

      // Only check edges left of x
      if(xx < x)
      {
        oddNodes = !oddNodes;
//        count++;
      }
    }
  }

  return oddNodes;
}

bool clObject2D::PointInObject(double x, double y)
{
  bool pointInObject  = false;

  int  numberPolygons = polygonList.size();
  for(int p=0; p<numberPolygons; p++)
  {
    if(PointInPolygon(p, x, y))
    {
      pointInObject = !pointInObject;
    }
  }

  return pointInObject;
}

int clObject2D::IsEncroachingPolygon(int polygonIndex, double x, double y)
{
  /*
  **
  ** Implements Ruppert's algoritm
  **
  */

  bool isEncroachingPolygon = false;
  int returnEdge = -1;

  // Find encroaching edge
  for(unsigned int edge=0; edge<polygonList[polygonIndex].size() && !isEncroachingPolygon; edge++)
  {
    int edgeIndex    = polygonList[polygonIndex][edge];

    int v1           = edgeList[edgeIndex][0];
    int v2           = edgeList[edgeIndex][1];

    double v1x       = vertexList[v1][0];
    double v1y       = vertexList[v1][1];

    double v2x       = vertexList[v2][0];
    double v2y       = vertexList[v2][1];

    // Confirm that x, y are not the vertices of this edge
    if(fabs(v1x-x)>PRECISION && fabs(v1y-y)>PRECISION && fabs(v2x-x)>PRECISION && fabs(v2y-y)>PRECISION)
    {
      double centreX   = (v1x + v2x)/2;
      double centreY   = (v1y + v2y)/2;

      double radius    = sqrt((v1x-centreX)*(v1x-centreX) + (v1y-centreY)*(v1y-centreY));
      double distance  = sqrt((x-centreX)*(x-centreX) + (y-centreY)*(y-centreY));

      if(distance<radius)
      {
        isEncroachingPolygon = true;
        returnEdge = edgeIndex;
      }
    }
  }

  return returnEdge;
}

bool clObject2D::IsEncroachingObject(double &x, double &y, bool splitEdge = false)
{
  bool isEncroachingObject = false;

  int numberPolygons = polygonList.size();
  for(int p=0; p<numberPolygons && !isEncroachingObject; p++)
  {
    int edge = IsEncroachingPolygon(p, x, y);
    if(edge!=-1)
    {
      isEncroachingObject = true;
      if(splitEdge)
      {
        // SplitEdge overwrites x, y with the midpoint of the edge
        SplitEdge(edge, x, y);
      }
    }
  }

  return isEncroachingObject;
}

void clObject2D::SplitEdge(int edge, double &v3x, double &v3y)
{
  // Get mid of edge
  int    v1  = edgeList[edge][0];
  int    v2  = edgeList[edge][1];

  double v1x = vertexList[v1][0];
  double v1y = vertexList[v1][1];

  double v2x = vertexList[v2][0];
  double v2y = vertexList[v2][1];

  v3x = (v1x + v2x)/2;
  v3y = (v1y + v2y)/2;

  // Add new vertex
  vector<double> coordinates;
  coordinates.push_back(v3x);
  coordinates.push_back(v3y);

  vertexList.push_back(coordinates);
  int v3  = vertexList.size()-1;

  // Add 2 new edges
  vector<int> edge1, edge2;

  edge1.push_back(v1);
  edge1.push_back(v3);

  edgeList.push_back(edge1);
  unsigned int edge1Index = edgeList.size()-1;

  edge2.push_back(v3);
  edge2.push_back(v2);

  edgeList.push_back(edge2);
  unsigned int edge2Index = edgeList.size()-1;

  // Search for edge in polygons
  for(unsigned int p=0; p<polygonList.size(); p++)
  {
    for(unsigned int e=0; e<polygonList[p].size(); e++)
    {
      if(polygonList[p][e]==edge)
      {
        // Erase existing reference to edge
        polygonList[p].erase(polygonList[p].begin()+e);

        // Add references to edge1 and edge2
        polygonList[p].push_back(edge1Index);
        polygonList[p].push_back(edge2Index);
      }
    }
  }
}

clObject2D& clObject2D::operator=(const clObject2D &source)
{
  return(Assign(source));
}

void clObject2D::ReadO2DFile(const string fileName)
{
  try
  {
    ReadO2DFile(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject2D::ReadO2DFile");
    throw EX;
  }
}

void clObject2D::ReadO2DFile(const char* fileName)
{
  try
  {
    ParseO2DFile(fileName);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject2D::ReadO2DFile");
    throw EX;
  }
}

void clObject2D::ParseO2DFile(const char* fileName)
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
      int numberPolygons;

      file >> numberVertices;
      file >> numberEdges;
      file >> numberPolygons;

      cout << "Vertices:  " << numberVertices << endl;
      cout << "Edges:     " << numberEdges << endl;
      cout << "Polygons:  " << numberPolygons << endl;

      double x, y;

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

//        cout << "X: " << x << "; Y: " << y <<  " << endl;

        vector<double> coordinates;
        coordinates.push_back(x);
        coordinates.push_back(y);

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


      progress = 0;
      for(int p=0; p<numberPolygons; p++)
      {
        if(100*p/numberPolygons > progress)
        {
          progress = 100*p/numberPolygons;
          printf("[%3d%%] Reading polygons...\n", progress);
        }

        int edgesInPolygon;
        file >> edgesInPolygon;

        int         edge;
        vector<int> polygon;
        for(int pe=0; pe<edgesInPolygon; pe++)
        {
          file >> edge;
          polygon.push_back(edge);
        }
        polygonList.push_back(polygon);
      }
      printf("[100%%] Done...\n");
//      printf(">>> Last read polygon: %d; %d; %d\n", edge1, edge2, edge3);

      file.close();
    }
    else
    {
      throw clExceptionTree("clObject2D", "ParseO2DFile", "Could not open input file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject2D::ParseO2DFile");
    throw EX;
  }
}
