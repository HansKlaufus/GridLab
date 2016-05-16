/********************************************************
*** Body file for definition of class clAc3dSurface
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <fstream>
#include <stdlib.h>

#include "ac3dsurface.h"
#include "exceptiontree.h"
#include "globals.h"

using namespace std;

clAc3dSurface::clAc3dSurface(void)
{
  this->SetObjectType(otAc3dSurface);
  this->SetObjectID(0);
}

clAc3dSurface::clAc3dSurface(const int ID, const string theName)
{
  this->SetObjectType(otAc3dObject);
  this->SetObjectID(0);
  this->SetName(theName);
}

clAc3dSurface::clAc3dSurface(const clAc3dSurface &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clAc3dSurface::clAc3dSurface");
    throw EX;
  }
}

clAc3dSurface::~clAc3dSurface(void)
{
  //cerr << "DESTROYING SURFACEGRID: " << GetNumberID() << endl;
}

clAc3dSurface& clAc3dSurface::Assign(const clAc3dSurface &source)
{
  try
  {

  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clAc3dSurface::Assign");
    throw EX;
  }

  return(*this);
}

clAc3dSurface& clAc3dSurface::operator=(const clAc3dSurface &source)
{
  return(Assign(source));
}

