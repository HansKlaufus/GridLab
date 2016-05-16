/******************************************************
*** Header file for declaration of class clObject
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>

using namespace std;

class clObject
{
private:
  string         name;
  int            objectID;

  void           ParseGTSGrid(const char*);

protected:
  /*
  ** 3D: 1 polyhedron consists out of u polygons
  ** 2D: 1 object     consists out of u polygons
  **     1 polygon    consists out of v edges
  **     1 edge       consists out of 2 vertices
  */

  vector< vector<double> > vertexList;     // Defines x, y, z of vertices
  vector< vector<int> >    edgeList;       // Defines v1 and v2 of the edges (= line segments)
  vector< vector<int> >    polygonList;    // Defines the edges for each polygon
  vector< vector<int> >    polyhedronList; // Defines the polygons for each polyhedron - only in 3D

public:
  clObject(void);
  clObject(const int, const string = "noName");
  clObject(const clObject&);
  ~clObject(void);

  void           SetName(const string newName) {name = newName;};
  string         GetName(void) const {return name;};

  void           SetObjectID(const int newID) {objectID = newID;};
  int            GetObjectID(void) const {return objectID;};

  unsigned int   GetNumberVertices(void) const    {return (vertexList.size());};
  unsigned int   GetNumberEdges(void) const       {return (edgeList.size());};
  unsigned int   GetNumberPolygons(void) const    {return (polygonList.size());};
  unsigned int   GetNumberPolyhedrons(void) const {return (polyhedronList.size());};

  double         X(int index) const {return (vertexList[index][0]);};
  double         Y(int index) const {return (vertexList[index][1]);};
  double         Z(int index) const {return (vertexList[index][2]);};

  double         GetMinX(void) const;
  double         GetMaxX(void) const;
  double         GetMinY(void) const;
  double         GetMaxY(void) const;
  double         GetMinZ(void) const;
  double         GetMaxZ(void) const;

  vector< vector<double> > GetVertices(void) const;

  clObject&      Assign(const clObject&);
  clObject&      operator=(const clObject&);

  void           ReadGTSGrid(const string);
  void           ReadGTSGrid(const char*);

  void           WriteLegacyVTKFile(void);
  void           WriteLegacyVTKFile(const string);
  void           WriteLegacyVTKFile(const char*);
};

#endif
