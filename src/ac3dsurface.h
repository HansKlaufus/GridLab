/******************************************************
*** Header file for declaration of class clAc3dSurface
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef AC3DSURFACE_H
#define AC3DSURFACE_H

#include <string>
#include <vector>

#include "globals.h"

using namespace std;

class clAc3dSurface
{
private:
  enObjectType  objectType;
  int           objectID;

  string        name;
  int           type;
  int           flags;
  int           mat;
  vector<int>   vertexIndexList;
  vector<float> texCoordsX, texCoordsY;

protected:

public:
  clAc3dSurface(void);
  clAc3dSurface(const int, string = "noName");
  clAc3dSurface(const clAc3dSurface&);
  ~clAc3dSurface(void);

  clAc3dSurface& Assign(const clAc3dSurface&);
  clAc3dSurface& operator=(const clAc3dSurface&);

  void          SetObjectType(enObjectType type)      {objectType = type;};
  void          SetObjectID(int id)                   {objectID = id;};

  void          SetName(const string newName)         {name = newName;};
  void          SetType(const int newType)            {type = newType;};
  void          SetFlags(const int newFlags)          {flags = newFlags;};
  void          SetMat(const int newMat)              {mat = newMat;};

  void          AddVertexIndex(const int index)       {vertexIndexList.push_back(index);};

  enObjectType  GetObjectType(void) const             {return (objectType);};
  int           GetNumberID(void) const               {return (objectID);};
  string        GetName(void) const                   {return (name);};
  int           GetType(void) const                   {return (type);};
  int           GetFlags(void) const                  {return (flags);};
  int           GetMat(void) const                    {return (mat);};

  unsigned int  GetNumberVertices(void) const         {return (vertexIndexList.size());};
  int           GetVertexIndex(const int index) const {return (vertexIndexList[index]);};
};

#endif
