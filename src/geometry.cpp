/**************************************************
*** Body file for definition of class clGeometry
***
*** J.L. Klaufus - 1999
**/

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "exceptiontree.h"
#include "geometry.h"
#include "surface.h"

/*
** ---------------
** Private methods
** ---------------
*/

int clGeometry::SetNumberID(const int number)
{
  numberID = number;

  return (numberID);
}

void clGeometry::CalculateExtremes(void)
{
  try
  {
    double xDummy;
    double yDummy;
    double zDummy;

    if(GetNumberSurfaces() < 1)
    {
      throw clExceptionTree("clGeometry", "CalculateExtremes", "Geometry does not contain any surfaces.");
    }

    xMin = surfaceList[0]->GetMinX();
    xMax = surfaceList[0]->GetMaxX();
    yMin = surfaceList[0]->GetMinY();
    yMax = surfaceList[0]->GetMaxY();
    zMin = surfaceList[0]->GetMinZ();
    zMax = surfaceList[0]->GetMaxZ();
    for(unsigned int s=1; s<surfaceList.size(); s++)
    {
      xDummy = surfaceList[s]->GetMinX();    xMin = (xMin < xDummy) ? xMin : xDummy;
      xDummy = surfaceList[s]->GetMaxX();    xMax = (xMax > xDummy) ? xMax : xDummy;
      yDummy = surfaceList[s]->GetMinY();    yMin = (yMin < yDummy) ? yMin : yDummy;
      yDummy = surfaceList[s]->GetMaxY();    yMax = (yMax > yDummy) ? yMax : yDummy;
      zDummy = surfaceList[s]->GetMinZ();    zMin = (zMin < zDummy) ? zMin : zDummy;
      zDummy = surfaceList[s]->GetMaxZ();    zMax = (zMax > zDummy) ? zMax : zDummy;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::CalculateExtremes");
    throw(EX);
  }
}

void clGeometry::Initialise(void)
{
  try
  {
    if(!initialised)
    {
      CalculateExtremes();

      initialised = 1;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::Initialise");
    throw(EX);
  }
}

/*
** --------------
** Public methods
** --------------
*/

/*
** Constructor and destructor
*/

clGeometry::clGeometry(void)
{
  SetNumberID(0);
  SetName("noName");
  initialised = 0;
}

clGeometry::clGeometry(const int number, const string theName)
{
  SetNumberID(0);
  SetName(theName);
  initialised = 0;
}

clGeometry::clGeometry(const clGeometry &source)
{
  Assign(source);
  initialised = 0;
}

clGeometry::~clGeometry(void)
{
  for(unsigned int s=0; s<surfaceList.size(); s++)
  {
    clSurface *surface = surfaceList[s];
    delete surface;
  }

  //cerr << "DESTROYING GEOMETRY: " << GetNumberID() << endl;
}

string clGeometry::GetName(void)
{
  return (name);
}

inline void clGeometry::SetName(const string theName)
{
  name = theName;
}

inline int clGeometry::GetNumberID(void) const
{
  return (numberID);
}

inline int clGeometry::GetNumberSurfaces(void) const
{
  return (surfaceList.size());
}

int clGeometry::GetSurfaceIndex(const int ID) const
{
  int index = -1;

  for(unsigned int i=0; i<surfaceList.size() && index==-1; i++)
  {
    if(surfaceList[i]->GetNumberID() == ID)
    {
      index=i;
    }
  }

  return (index);
}

int clGeometry::AddSurface(clSurface *newSurface)
{
  // Add the new surface
  surfaceList.push_back(newSurface);

  // Reset the initialisation-flag
  initialised = 0;

  return(surfaceList.size()-1);
}

int clGeometry::DeleteSurfaceByID(const int ID)
{
  int index = GetSurfaceIndex(ID);

  if(index==-1)
  {
    throw clExceptionTree("clGeometry", "DeleteSurfaceID", "No surface with specified ID.");
  }
  else
  {
    surfaceList.erase(surfaceList.begin() + index);

  // Reset the initialisation-flag
  initialised = 0;
  }

  return (surfaceList.size());
}

int clGeometry::DeleteSurfaceByIndex(const int index)
{
  // Delete the surface
  surfaceList.erase(surfaceList.begin()+index);

  // Reset the initialisation-flag
  initialised = 0;

  return (surfaceList.size());
}

clSurface clGeometry::GetSurface(const int index) const
{
  if(index<0 || index>=(int)surfaceList.size())
  {
    throw clExceptionTree("clGeometry", "GetSurface", "No surface with specified index.");
  }

  return(*surfaceList[index]);
}

clSurface* clGeometry::GetSurfacePointer(const int index) const
{
  if(index<0 || index>=(int)surfaceList.size())
  {
    throw clExceptionTree("clGeometry", "GetSurface", "No surface with specified index.");
  }

  return(surfaceList[index]);
}

double clGeometry::GetMinX(void)
{
  try
  {
    if(!initialised)
    {
      Initialise();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::GetMinX");
    throw(EX);
  }

  return(xMin);
}

double clGeometry::GetMaxX(void)
{
  try
  {
    if(!initialised)
    {
      Initialise();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::GetMaxX");
    throw(EX);
  }

  return(xMax);
}

double clGeometry::GetMinY(void)
{
  try
  {
    if(!initialised)
    {
      Initialise();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::GetMinY");
    throw(EX);
  }

  return(yMin);
}

double clGeometry::GetMaxY(void)
{
  try
  {
    if(!initialised)
    {
      Initialise();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::GetMaxY");
    throw(EX);
  }

  return(yMax);
}

double clGeometry::GetMinZ(void)
{
  try
  {
    if(!initialised)
    {
      Initialise();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::GetMinZ");
    throw(EX);
  }

  return(zMin);
}

double clGeometry::GetMaxZ(void)
{
  try
  {
    if(!initialised)
    {
      Initialise();
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometry::GetMaxZ");
    throw(EX);
  }

  return(zMax);
}

/*
** Methods for operations
*/

clGeometry& clGeometry::Assign(const clGeometry &source)
{
  SetNumberID(source.GetNumberID());

  for(int i=0; i<source.GetNumberSurfaces(); i++)
  {
    clSurface *surface = new clSurface(source.GetSurface(i));
    AddSurface(surface);
  }

  return(*this);
}

clGeometry& clGeometry::operator=(const clGeometry &source)
{
  return(Assign(source));
}
