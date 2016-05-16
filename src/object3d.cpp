/**************************************************
*** Body file for definition of class clObject3D
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <fstream>
#include <string>
#include <vector>

#include "ac3dobject.h"
#include "exceptiontree.h"
#include "matrix.h"
#include "object3d.h"
#include "vector.h"

clObject3D::clObject3D(void) : clObject()
{
}

clObject3D::clObject3D(const int theID, const string theName) : clObject(theID, theName)
{
}

clObject3D::clObject3D(const clObject3D &source)
{
  Assign(source);
}

clObject3D::clObject3D(const clAc3dObject *source)
{
  Assign(source);
}

clObject3D::~clObject3D(void)
{
  //cerr << "DESTROYING OBJECT: " << GetObjectID() << endl;
}

clObject3D& clObject3D::Assign(const clObject3D &source)
{
  SetObjectID(source.GetObjectID());

  return(*this);
}

clObject3D& clObject3D::Assign(const clAc3dObject *source)
{
  if(this->GetName() == "NoName")
  {
    this->SetName(source->GetName());
  }

  int vertexStartIndex = vertexList.size();

  for(unsigned int v=0; v<source->GetNumberVertices(false); v++)
  {
    clNode3D *node3d = source->GetVertexPointer(v);

    vector<double> vertex;
    vertex.push_back(node3d->x());
    vertex.push_back(node3d->y());
    vertex.push_back(node3d->z());

    vertexList.push_back(vertex);
  }

  vector<int> polyhedron;

  for(unsigned int s=0; s<source->GetNumberSurfaces(false); s++)
  {
    clAc3dSurface *surface = source->GetSurfacePointer(s);

    vector<int> polygon;
    for(unsigned int v=0; v<surface->GetNumberVertices(); v++)
    {
      vector<int> edge;
      int         vertexIndex;

      vertexIndex = surface->GetVertexIndex(v);
      edge.push_back(vertexStartIndex + vertexIndex);

      vertexIndex = surface->GetVertexIndex((v+1)%surface->GetNumberVertices());
      edge.push_back(vertexStartIndex + vertexIndex);

      edgeList.push_back(edge);
      polygon.push_back(edgeList.size()-1);
    }
    polygonList.push_back(polygon);
    polyhedron.push_back(polygonList.size()-1);
  }

  polyhedronList.push_back(polyhedron);

  for(unsigned int k=0; k<source->GetNumberKids(false); k++)
  {
    clAc3dObject *ac3dObject = source->GetKidPointer(k);
    this->Assign(ac3dObject);
  }

  return(*this);
}

bool clObject3D::PointInPolygon(int polygonIndex, double x, double y, double z)
{
  /*
  **
  ** Implements Jordan curve theorem
  **
  */

  // See: http://en.wikipedia.org/wiki/Point_in_polygon
  // See: http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
  // See: http://www.alienryderflex.com/polygon/
  // See: http://mathforum.org/library/drmath/view/62814.html

  bool pointInPolygon = false;

  // Take mid of first edge as direction vector for ray
  int edgeIndex    = polygonList[polygonIndex][0];

  int v1           = edgeList[edgeIndex][0];
  int v2           = edgeList[edgeIndex][1];

  double PM[3];
  PM[0] = (vertexList[v1][0] + vertexList[v2][0])/2;
  PM[1] = (vertexList[v1][1] + vertexList[v2][1])/2;
  PM[2] = (vertexList[v1][2] + vertexList[v2][2])/2;

  double P1[3];
  P1[0] = x;
  P1[1] = y;
  P1[2] = z;

  double V1[3];
  V1[0] = PM[0]-P1[0];
  V1[1] = PM[1]-P1[1];
  V1[2] = PM[2]-P1[2];

  // Loop through all edges in this polygon
  for(unsigned int edge=0; edge<polygonList[polygonIndex].size(); edge++)
  {
    edgeIndex    = polygonList[polygonIndex][edge];

    v1           = edgeList[edgeIndex][0];
    v2           = edgeList[edgeIndex][1];

    double P2[3];
    P2[0] = vertexList[v1][0];
    P2[1] = vertexList[v1][1];
    P2[2] = vertexList[v1][2];

    double P3[3];
    P3[0] = vertexList[v2][0];
    P3[1] = vertexList[v2][1];
    P3[2] = vertexList[v2][2];

    double V2[3];
    V2[0] = P3[0]-P2[0];
    V2[1] = P3[1]-P2[1];
    V2[2] = P3[2]-P2[2];

    double P2P1[3];
    P2P1[0] = P2[0]-P1[0];
    P2P1[1] = P2[1]-P1[1];
    P2P1[2] = P2[2]-P1[2];

    double V1V2[3];
    V1V2[0] = +V1[1]*V2[2]-V2[1]*V1[2];
    V1V2[1] = -V1[0]*V2[2]+V2[0]*V1[2];
    V1V2[2] = +V1[0]*V2[1]-V2[0]*V1[1];

    double LengthV1V2 = sqrt(V1V2[0]*V1V2[0] + V1V2[1]*V1V2[1] + V1V2[2]*V1V2[2]);

    double P2P1V2[3];
    P2P1V2[0] = +P2P1[1]*V2[2]-V2[1]*P2P1[2];
    P2P1V2[1] = -P2P1[0]*V2[2]+V2[0]*P2P1[2];
    P2P1V2[2] = +P2P1[0]*V2[1]-V2[0]*P2P1[1];

    double LengthP2P1V2 = sqrt(P2P1V2[0]*P2P1V2[0] + P2P1V2[1]*P2P1V2[1] + P2P1V2[2]*P2P1V2[2]);

    if(LengthV1V2 > PRECISION)
    {
      double a = LengthP2P1V2 / LengthV1V2;
//      double a = P2P1V2[0]/V1V2[0];

      // only check 'right' of P1
      if(a>0)
      {
        double C[3];
        C[0] = P1[0] + a*V1[0];
        C[1] = P1[1] + a*V1[1];
        C[2] = P1[2] + a*V1[2];

        // Check if the intersection points fall within the ranges of the edges
        bool xCondition = ((P2[0]-PRECISION<=C[0]) && (C[0]-PRECISION<=P3[0])) || ((P3[0]-PRECISION<=C[0]) && (C[0]-PRECISION<=P2[0]));
        bool yCondition = ((P2[1]-PRECISION<=C[1]) && (C[1]-PRECISION<=P3[1])) || ((P3[1]-PRECISION<=C[1]) && (C[1]-PRECISION<=P2[1]));
        bool zCondition = ((P2[2]-PRECISION<=C[2]) && (C[2]-PRECISION<=P3[2])) || ((P3[2]-PRECISION<=C[2]) && (C[2]-PRECISION<=P2[2]));

        if(xCondition && yCondition && zCondition)
        {
          pointInPolygon =!pointInPolygon;
        }
      }
    }
  }

  return pointInPolygon;
}

bool clObject3D::PointInPolyhedron(int polyhedronIndex, double x, double y, double z)
{
  // See: http://en.wikipedia.org/wiki/Intersection_of_a_polyhedron_with_a_line
  // See: http://en.wikipedia.org/wiki/Line-plane_intersection

  /*
  ** Steps to take:
  ** 0) The faces of a polyhedron are effectively defined by polygons
  ** 1) Calculate the intersection point between the horizontal ray through point x, y, z and the plane through a face
  ** 2) Check if intersection point is left of x, y, z
  ** 3) Check if this intersection point falls within the polygon - this is effectively a 2d problem
  ** 4) If so, the ray crosses a face, increase face count, and continue with the next face
  ** 5) If not, then continue with the next face
  */

  /*
  ** (xa, ya, za) and (xb, yb, zb) two points defining line through x, y, z
  ** (x0, y0, z0) and (x1, y1, z1) and (x2, y2, z2) three points defining the plane
  */

  bool pointInPolyhedron = false;

  // Define horizontal line through x, y, z
  double xa = x;
  double ya = y;
  double za = z;

  double xb = x-5.0f;
  double yb = y;
  double zb = z;

  // Loop through all polygons in this polyhedron
  for(unsigned int p=0; p<polyhedronList[polyhedronIndex].size(); p++)
  {
    // Select the polygon
    int polygonIndex = polyhedronList[polyhedronIndex][p];

    // From this polygon select 2 edges
    int e1 = polygonList[polygonIndex][0];
    int e2 = polygonList[polygonIndex][1];

    // From these two edges select three vertices defining three points of the plane through this polygon
    int v1 = edgeList[e1][0];
    int v2 = edgeList[e1][1];
    int v3 = ((edgeList[e2][0] != v1) && (edgeList[e2][0] != v2)) ? edgeList[e2][0] : edgeList[e2][1];

    double x0 = vertexList[v1][0];
    double y0 = vertexList[v1][1];
    double z0 = vertexList[v1][2];

    double x1 = vertexList[v2][0];
    double y1 = vertexList[v2][1];
    double z1 = vertexList[v2][2];

    double x2 = vertexList[v3][0];
    double y2 = vertexList[v3][1];
    double z2 = vertexList[v3][2];

    tpMatrix<double> m1(3, 3);
    m1.SetElement(1, 1, xa-xb); m1.SetElement(1, 2, x1-x0); m1.SetElement(1, 3, x2-x0);
    m1.SetElement(2, 1, ya-yb); m1.SetElement(2, 2, y1-y0); m1.SetElement(2, 3, y2-y0);
    m1.SetElement(3, 1, za-zb); m1.SetElement(3, 2, z1-z0); m1.SetElement(3, 3, z2-z0);

    tpMatrix<double> m2(3, 1);
    m2.SetElement(1, 1, xa-x0);
    m2.SetElement(2, 1, ya-y0);
    m2.SetElement(3, 1, za-z0);

    // Solve - solution will be stored in m1 - (t, u, v)
    bool isSingular = false;
    try
    {
      m1.Solve(m2);
    }

    catch(clExceptionTree EX)
    {
      isSingular = true;
    }

    // If m1 is singular, then the line is either parallel to the plane, or lies in the plane
    if(!isSingular)
    {
      // Calculate the intersection coordinates using the parametric line equation
      double xi = xa + (xb-xa) * m1(1, 1);
      double yi = ya + (yb-ya) * m1(1, 1);
      double zi = za + (zb-za) * m1(1, 1);

      // Check if the intersection point is left of (x, y, z) and falls within the polygon
      if((xi<x) && PointInPolygon(polygonIndex, xi, yi, zi))
      {
        pointInPolyhedron = !pointInPolyhedron;

//        cout << "The ray through Point (" << x << ", " << y << ", " << z << ") intersects with Polygon " << polygonIndex << " of Polyhedron "<< polyhedronIndex << " through coordinates (" << xi << ", " << yi << ", " << zi << ")"<< endl;
      }
    } // TODO: Something if line lies in the plane
  }

  return pointInPolyhedron;
}

bool clObject3D::PointInObject(double x, double y, double z)
{
  bool pointInObject     = false;
  int  numberPolyhedrons = polyhedronList.size();

  for(int p=0; p<numberPolyhedrons; p++)
  {
    if(PointInPolyhedron(p, x, y, z))
    {
      pointInObject = !pointInObject;
    }
  }

  return pointInObject;
}

int clObject3D::IsEncroachingPolygon(int polygonIndex, double x, double y, double z)
{
  /*
  **
  ** Implements Ruppert's algoritm
  **
  */

  bool isEncroachingPolygon = false;
  int returnEdge = -1;

  // Find encroaching edge
  for(unsigned int e=0; e<polygonList[polygonIndex].size() && !isEncroachingPolygon; e++)
  {
    int edgeIndex    = polygonList[polygonIndex][e];

    int v1           = edgeList[edgeIndex][0];
    int v2           = edgeList[edgeIndex][1];

    double v1x       = vertexList[v1][0];
    double v1y       = vertexList[v1][1];
    double v1z       = vertexList[v1][2];

    double v2x       = vertexList[v2][0];
    double v2y       = vertexList[v2][1];
    double v2z       = vertexList[v1][2];

    // Confirm that x, y, z are not the vertices of this edge
    if(fabs(v1x-x)>PRECISION && fabs(v1y-y)>PRECISION && fabs(v1z-z)>PRECISION && fabs(v2x-x)>PRECISION && fabs(v2y-y)>PRECISION && fabs(v2z-z)>PRECISION)
    {
      double centreX   = (v1x + v2x)/2;
      double centreY   = (v1y + v2y)/2;
      double centreZ   = (v1z + v2z)/2;

      double radius    = sqrt((v1x-centreX)*(v1x-centreX) + (v1y-centreY)*(v1y-centreY) + (v1z-centreZ)*(v1z-centreZ));
      double distance  = sqrt((x-centreX)*(x-centreX) + (y-centreY)*(y-centreY) + (z-centreZ)*(z-centreZ));

      if(distance<radius)
      {
        isEncroachingPolygon = true;
        returnEdge = edgeIndex;
      }
    }
  }

  return returnEdge;
}

bool clObject3D::IsEncroachingPolyhedron(int polyhedronIndex, double &x, double &y, double &z, bool splitEdge = false)
{
  bool isEncroachingPolyhedron = false;

  int numberPolygons = polyhedronList[polyhedronIndex].size();
  for(int p=0; p<numberPolygons && !isEncroachingPolyhedron; p++)
  {
    int edge = IsEncroachingPolygon(p, x, y, z);
    if(edge!=-1)
    {
      isEncroachingPolyhedron = true;
      if(splitEdge)
      {
        // SplitEdge overwrites x, y with the midpoint of the edge
        SplitEdge(edge, x, y, z);
      }
    }
  }

  return isEncroachingPolyhedron;
}

bool clObject3D::IsEncroachingObject(double &x, double &y, double &z, bool splitEdge = false)
{
  bool isEncroachingObject = false;

  int numberPolyhedrons = polyhedronList.size();
  for(int p=0; p<numberPolyhedrons && !isEncroachingObject; p++)
  {
    isEncroachingObject = IsEncroachingPolyhedron(p, x, y, z, splitEdge);
  }

  return isEncroachingObject;
}

void clObject3D::SplitEdge(int edge, double &v3x, double &v3y, double &v3z)
{
  // Get mid of edge
  int    v1  = edgeList[edge][0];
  int    v2  = edgeList[edge][1];

  double v1x = vertexList[v1][0];
  double v1y = vertexList[v1][1];
  double v1z = vertexList[v1][2];

  double v2x = vertexList[v2][0];
  double v2y = vertexList[v2][1];
  double v2z = vertexList[v2][2];

  v3x = (v1x + v2x)/2;
  v3y = (v1y + v2y)/2;
  v3z = (v1z + v2z)/2;

  // Add new vertex
  vector<double> coordinates;
  coordinates.push_back(v3x);
  coordinates.push_back(v3y);
  coordinates.push_back(v3z);

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

clObject3D& clObject3D::operator=(const clObject3D &source)
{
  return(Assign(source));
}


void clObject3D::ReadO3DFile(const string fileName)
{
  try
  {
    ReadO3DFile(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject3D::ReadO3DFile");
    throw EX;
  }
}

void clObject3D::ReadO3DFile(const char* fileName)
{
  try
  {
    ParseO3DFile(fileName);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject3D::ReadO3DFile");
    throw EX;
  }
}

void clObject3D::ParseO3DFile(const char* fileName)
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
      int numberPolyhedrons;

      file >> numberVertices;
      file >> numberEdges;
      file >> numberPolygons;
      file >> numberPolyhedrons;

      cout << "Vertices:    " << numberVertices << endl;
      cout << "Edges:       " << numberEdges << endl;
      cout << "Polygons:    " << numberPolygons << endl;
      cout << "Polyhedrons: " << numberPolyhedrons << endl;

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

//        cout << "X: " << x << "; Y: " << y <<  " << endl;

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

      progress = 0;
      for(int p=0; p<numberPolyhedrons; p++)
      {
        if(100*p/numberPolyhedrons > progress)
        {
          progress = 100*p/numberPolyhedrons;
          printf("[%3d%%] Reading polyhedrons...\n", progress);
        }

        int polygonsInPolygon;
        file >> polygonsInPolygon;

        int         polygon;
        vector<int> polyhedron;
        for(int pe=0; pe<polygonsInPolygon; pe++)
        {
          file >> polygon;
          polyhedron.push_back(polygon);
        }
        polyhedronList.push_back(polyhedron);
      }
      printf("[100%%] Done...\n");

      file.close();
    }
    else
    {
      throw clExceptionTree("clObject3D", "ParseO3DFile", "Could not open input file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject3D::ParseO3DFile");
    throw EX;
  }
}

void clObject3D::ReadSTLFile(const string fileName)
{
  try
  {
    ReadSTLFile(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject3D::ReadSTLFile");
    throw EX;
  }
}

void clObject3D::ReadSTLFile(const char* fileName)
{
  try
  {
    ParseSTLFile(fileName);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject3D::ReadSTLFile");
    throw EX;
  }
}

void clObject3D::ParseSTLFile(const char* fileName)
{
  try
  {
    // Clear all lists - ? These are actually vector in vector; will this work?
    vertexList.clear();
    edgeList.clear();
    polygonList.clear();
    polyhedronList.clear();

    // Open file in binary mode
    fstream file;
    file.open(fileName, ios::in | ios::binary);
    file.precision(3);
    if (file.good())
    {
      // Read header information
      char header[80];
      int  numberTriangles;

      file.read(header, sizeof(header));
      file.read((char*) &numberTriangles, sizeof(numberTriangles));

      cout << "Reading object: " << header << endl;
      cout << "Number of triangles: " << numberTriangles << endl;

      vector<int> polyhedron;

      for(int t=0; t<numberTriangles; t++)
      {
        int progress = 100*t/numberTriangles;
        printf("[%3d%%] Reading triangles...\n", progress);

        float coordinates[3];

        // Read the normal vector
        file.read((char*) coordinates, sizeof(coordinates));

        // Read the vertices of a triangle
        for(int v=0; v<3; v++)
        {
          file.read((char*) coordinates, sizeof(coordinates));

          vector<double> vertex;
          vertex.push_back(coordinates[0]);
          vertex.push_back(coordinates[1]);
          vertex.push_back(coordinates[2]);

          vertexList.push_back(vertex);
        }

        // Create 3 edges
        vector<int> edge1;
        edge1.push_back(vertexList.size()-3);
        edge1.push_back(vertexList.size()-2);

        vector<int> edge2;
        edge2.push_back(vertexList.size()-2);
        edge2.push_back(vertexList.size()-1);

        vector<int> edge3;
        edge3.push_back(vertexList.size()-1);
        edge3.push_back(vertexList.size()-3);

        edgeList.push_back(edge1);
        edgeList.push_back(edge2);
        edgeList.push_back(edge3);

        // Create polygon
        vector<int> polygon;
        polygon.push_back(edgeList.size()-3);
        polygon.push_back(edgeList.size()-2);
        polygon.push_back(edgeList.size()-1);

        polygonList.push_back(polygon);

        // Add to polyhedron
        polyhedron.push_back(polygonList.size()-1);

        // Read attribute byte count
        char attribute[2];
        file.read(attribute, sizeof(attribute));
      }
      polyhedronList.push_back(polyhedron);

      printf("[100%%] Done...\n");

      file.close();
    }
    else
    {
      throw clExceptionTree("clObject3D", "ParseSTLFile", "Could not open input file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clObject3D::ParseSTLFile");
    throw EX;
  }
}
