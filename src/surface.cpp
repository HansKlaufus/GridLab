/**************************************************
*** Body file for definition of class clSurface
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <fstream>
#include <stdio.h>
#include <math.h>
#include <vector>

#include "carpet.h"
#include "exception.h"
#include "surface.h"
#include "patch.h"

/*
** ---------------
** Private methods
** ---------------
*/

int clSurface::SetNumberID(const int number)
{
  numberID = number;

  return (numberID);
}


/*
** Connectivity methods
*/

int clSurface::AddConnection(const int numberID,
                             const int north, const int east,
                             const int south, const int west)
{
  int numConnect;

  // Create new entry
  stConnect newConnection;
  newConnection.numberID = numberID;
  newConnection.north    = north;
  newConnection.east     = east;
  newConnection.south    = south;
  newConnection.west     = west;

  // Add to list
  numConnect = connectList.AddItem(newConnection);

  // Add also to remainList when not all connections could be established.
  // (Reason: performance)
  if(north==-1 || east==-1 || south==-1 || west==-1) remainList.push_back(numConnect);

  return(numConnect);
}

void clSurface::ConnectPatch(const int self)
{
  // Reset the values
  int numberID = -1;
  int north    = -1;
  int south    = -1;
  int west     = -1;
  int east     = -1;

  int remainNorth = -1;
  int remainSouth = -1;
  int remainEast  = -1;
  int remainWest  = -1;

  // Calculate the corner co-ordinates
  dMatrix selfSW  = patchList[self]->GetCoordinates(0, 0);
  dMatrix selfSE  = patchList[self]->GetCoordinates(1, 0);
  dMatrix selfNW  = patchList[self]->GetCoordinates(0, 1);
  dMatrix selfNE  = patchList[self]->GetCoordinates(1, 1);

  for(unsigned int i=0; i<remainList.size(); i++)
  {
    int other = remainList[i];

    if(other != self)
    {
      dMatrix otherSW = patchList[other]->GetCoordinates(0, 0);
      dMatrix otherSE = patchList[other]->GetCoordinates(1, 0);
      dMatrix otherNW = patchList[other]->GetCoordinates(0, 1);
      dMatrix otherNE = patchList[other]->GetCoordinates(1, 1);

      // Check for cennectivity
      if(selfNW==otherSW && selfNE==otherSE)
      {
        north       = other;
        remainNorth = i;
      }
      else if(selfNE==otherNW && selfSE==otherSW)
      {
        east       = other;
        remainEast = i;
      }
      else if(selfSE==otherNE && selfSW==otherNW)
      {
        south       = other;
        remainSouth = i;
      }
      else if(selfSW==otherSE && selfNW==otherNE)
      {
        west       = other;
        remainWest = i;
      }
    }
  }

  // Update existing connectivity entries
  if(north!=-1)
  {
    connectList[north]->south = self;

    // If the north patch is connected on all sides it can be deleted from the remainList
    if (connectList[north]->west != -1 && connectList[north]->north != -1 && connectList[north]->east != -1)
    {
      remainList.erase(remainList.begin() + remainNorth);
      if (remainEast  > remainNorth) remainEast--;
      if (remainSouth > remainNorth) remainSouth--;
      if (remainWest  > remainNorth) remainWest--;
    }
  }

  if(east !=-1)
  {
    connectList[east]->west  = self;

    // If the east patch is connected on all sides it can be deleted from the remainList
    if (connectList[east]->north != -1 && connectList[east]->east != -1 && connectList[east]->south != -1)
    {
      remainList.erase(remainList.begin() + remainEast);
      if (remainSouth > remainEast) remainSouth--;
      if (remainWest  > remainEast) remainWest--;
    }
  }

  if(south!=-1)
  {
    connectList[south]->north = self;

    // If the south patch is connected on all sides it can be deleted from the remainList
    if (connectList[south]->east != -1 && connectList[south]->south != -1 && connectList[south]->west != -1)
    {
      remainList.erase(remainList.begin() + remainSouth);
      if (remainWest > remainSouth) remainWest--;
    }
  }

  if(west !=-1)
  {
    connectList[west]->east  = self;

    // If the west patch is connected on all sides it can be deleted from the remainList
    if (connectList[west]->south != -1 && connectList[west]->west != -1 && connectList[west]->north != -1)
    {
      remainList.erase(remainList.begin() + remainWest);
    }
  }

  // Get Identification
  numberID = patchList[self]->GetNumberID();

  // Add new connectivity entry
  AddConnection(numberID, north, east, south, west);
}

void clSurface::CheckConnection(void)
{
  int NSLoop = 0;  // 0 = no north-south loop; 1 = north-south loop
  int EWLoop = 0;  // 0 = no east-west loop; 1 east-west loop

  int startPatch, nextPatch, southPatch, westPatch;

  // Check if any patches exist
  if(GetNumberPatches() < 1)
  {
    throw clException("clSurface", "CheckConnection", "Surface does not contain any patches.");
  }

  // Check for north-south closed loop connection
  startPatch = 0;
  nextPatch  = connectList[startPatch]->north;
  while(nextPatch!=-1 && nextPatch!=startPatch)
  {
    nextPatch = connectList[nextPatch]->north;
  }

  if(nextPatch==startPatch)
  {
    NSLoop = 1;
  }

  // Check for east-west closed loop connection
  startPatch = 0;
  nextPatch  = connectList[startPatch]->east;
  while(nextPatch!=-1 && nextPatch!=startPatch)
  {
    nextPatch = connectList[nextPatch]->east;
  }

  if(nextPatch==startPatch)
  {
    EWLoop = 1;
  }

  // Try to correct
  if(NSLoop && EWLoop)
  {
    /*
    ** Double closed loop surface; cutting routine must still be implemented
    */

    throw clException("clSurface", "CheckConnection", "Double closed loop surface: Must still be implemented.");
  }
  else if(NSLoop && !EWLoop)
  {
    /*
    ** Single N-S closed loop surface; create E-W cut south of patch 0
    */

    // Cut patch 0
    nextPatch  = 0;
    southPatch = connectList[nextPatch]->south;
    connectList[nextPatch]->south  = -1;
    connectList[southPatch]->north = -1;

    // Cut all patches east of patch 0
    nextPatch = 0;
    while(connectList[nextPatch]->east != -1)
    {
      nextPatch  = connectList[nextPatch]->east;
      southPatch = connectList[nextPatch]->south;
      connectList[nextPatch]->south  = -1;
      connectList[southPatch]->north = -1;
    }

    // Cut all patches west of patch 0
    nextPatch = 0;
    while(connectList[nextPatch]->west != -1)
    {
      nextPatch = connectList[nextPatch]->west;
      southPatch = connectList[nextPatch]->south;
      connectList[nextPatch]->south  = -1;
      connectList[southPatch]->north = -1;
    }
  }
  else if(!NSLoop && EWLoop)
  {
    /*
    ** Single E-W closed loop surface; create N-S cut west of patch 0
    */

    // Cut patch 0
    nextPatch = 0;
    westPatch = connectList[nextPatch]->west;
    connectList[nextPatch]->west = -1;
    connectList[westPatch]->east = -1;

    // Cut all patches north of patch 0
    nextPatch = 0;
    while(connectList[nextPatch]->north != -1)
    {
      nextPatch = connectList[nextPatch]->north;
      westPatch = connectList[nextPatch]->west;
      connectList[nextPatch]->west = -1;
      connectList[westPatch]->east = -1;
    }

    // Cut all patches south of patch 0
    nextPatch = 0;
    while(connectList[nextPatch]->south != -1)
    {
      nextPatch = connectList[nextPatch]->south;
      westPatch = connectList[nextPatch]->west;
      connectList[nextPatch]->west = -1;
      connectList[westPatch]->east = -1;
    }
  }
}


/*
** Set up Parametric space
*/

void clSurface::Parameterise(void)
{
  // Get south-west patch
  int patchSW = 0;
  while(connectList[patchSW]->south!=-1)
  {
    patchSW = connectList[patchSW]->south;
  }

  while(connectList[patchSW]->west!=-1)
  {
    patchSW = connectList[patchSW]->west;
  }

  origin = patchSW;

  // Count number of nodes in u-direction
  int uCount       = 1;
  int patchPointer = patchSW;
  while(connectList[patchPointer]->east!=-1)
  {
    patchPointer = connectList[patchPointer]->east;
    uCount++;
  }
  uCount++; // 1 node more than patches

  // Count number of nodes in v-direction
  int vCount   = 1;
  patchPointer = patchSW;
  while(connectList[patchPointer]->north!=-1)
  {
    patchPointer = connectList[patchPointer]->north;
    vCount++;
  }
  vCount++; // 1 node more than patches

  // Calculate the delta's
  uDelta = 1.0/(uCount-1);
  vDelta = 1.0/(vCount-1);
}

/*
** Calculate the extremes
*/
void clSurface::CalculateExtremes(void)
{
  double xDummy;
  double yDummy;
  double zDummy;

  if(GetNumberPatches() < 1)
  {
    throw clException("clSurface", "CalculateExtremes", "Surface does not contain any patches.");
  }

  xMin = patchList[0]->GetMinX();
  xMax = patchList[0]->GetMaxX();
  yMin = patchList[0]->GetMinY();
  yMax = patchList[0]->GetMaxY();
  zMin = patchList[0]->GetMinZ();
  zMax = patchList[0]->GetMaxZ();
  for(int p=1; p<GetNumberPatches(); p++)
  {
    xDummy = patchList[p]->GetMinX();    xMin = (xMin < xDummy) ? xMin : xDummy;
    xDummy = patchList[p]->GetMaxX();    xMax = (xMax > xDummy) ? xMax : xDummy;
    yDummy = patchList[p]->GetMinY();    yMin = (yMin < yDummy) ? yMin : yDummy;
    yDummy = patchList[p]->GetMaxY();    yMax = (yMax > yDummy) ? yMax : yDummy;
    zDummy = patchList[p]->GetMinZ();    zMin = (zMin < zDummy) ? zMin : zDummy;
    zDummy = patchList[p]->GetMaxZ();    zMax = (zMax > zDummy) ? zMax : zDummy;
  }
}

/*
** Method initialise
*/
void clSurface::Initialise(void)
{
  if(!initialised)
  {
    CheckConnection();
    Parameterise();
    CalculateExtremes();

    initialised = 1;
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

clSurface::clSurface(void)
{
  SetNumberID(0);

  initialised = 0;
}

clSurface::clSurface(const int number)
{
  SetNumberID(number);

  initialised = 0;
}

clSurface::clSurface(const clSurface &source)
{
  Assign(source);

  initialised = 0;
}

clSurface::clSurface(const dMatrix &pointsX, const dMatrix &pointsY, const dMatrix &pointsZ, const int number=0)
{
  SetNumberID(number);

  initialised = 0;

  CreatePatches(pointsX, pointsY, pointsZ);
}

clSurface::~clSurface(void)
{
  for(unsigned int p=0; p<patchList.size(); p++) delete patchList[p];

  //cerr << "DESTROYING SURFACE: " << GetNumberID() << endl;
}


/*
** Query methods
*/

int clSurface::GetNumberID(void) const
{
  return (numberID);
}

int clSurface::GetNumberConnect(void) const
{
  return(connectList.GetNumberItems());
}

int clSurface::GetNumberPatches(void) const
{
  return (patchList.size());
}

int clSurface::GetPatchEntry(const int i, const int j) const
{
  // Find correct patch
  int patchPointer = origin;

  int ii=0;
  while(connectList[patchPointer]->east!=-1 && ii<i)
  {
    patchPointer = connectList[patchPointer]->east;
    ii++;
  }

  int jj=0;
  while(connectList[patchPointer]->north!=-1 && jj<j)
  {
    patchPointer = connectList[patchPointer]->north;
    jj++;
  }

  if(ii<i || jj<j)
  {
    char message[MESLEN];
    sprintf(message, "Invalid values for i and/or j.\n  I = %d\n  J = %d", i, j);
    throw clException("clSurface", "GetPatchEntry", message);
  }

  return(patchPointer);
}


/*
** Patch methods
*/

int clSurface::AddPatch(clPatch *newPatch)
{
  // Add to list
  patchList.push_back(newPatch);
  int entryNumber = patchList.size()-1;

  // Create new connectivity list
  ConnectPatch(entryNumber);

  // Reset initialisation-flag
  initialised = 0;

  return(entryNumber);
}

int clSurface::AddPatchDelayed(clPatch *newPatch)
{
  // Add to list
  patchList.push_back(newPatch);
  int entryNumber = patchList.size()-1;

  // Reset initialisation-flag
  initialised = 0;

  return(entryNumber);
}

int clSurface::DeletePatch(const int entryNumber)
{
  if(entryNumber<0 || entryNumber>=(int)patchList.size())
  {
    throw clException("clSurface", "DeletePatch", "No patch with specified entry.");
  }

  patchList.erase(patchList.begin() + entryNumber);

  return (patchList.size());
}

clPatch clSurface::GetPatch(const int entryNumber) const
{
  if(entryNumber<0 || entryNumber>=(int)patchList.size())
  {
    throw clException("clSurface", "GetPatch", "No patch with specified entry.");
  }

  return(*patchList[entryNumber]);
}

clPatch* clSurface::GetPatchPointer(const int entryNumber) const
{
  if(entryNumber<0 || entryNumber>=(int)patchList.size())
  {
    throw clException("clSurface", "GetPatch", "No patch with specified entry.");
  }

  return(patchList[entryNumber]);
}

void clSurface::BuildPatchConnectivity(void)
{
  int numPatch   = patchList.size();
  int numConnect = GetNumberConnect();
  int numRemain  = remainList.size();

  // Delete old connections
  for (int i=0; i<numConnect; i++)
  {
    connectList.DeleteItem(i);
  }

  {
    for (int i=0; i<numRemain; i++)
    {
      remainList.erase(remainList.begin() + i);
    }
  }

  // Create new connections
  {
    for(int i=0; i<numPatch; i++)
    {
      ConnectPatch(i);
    }
  }
}


/*
** Method to create a surface out of defined points
*/
void clSurface::CreatePatches(const dMatrix &pointsX, const dMatrix &pointsY, const dMatrix &pointsZ)
{
  // Declare the derivative matrices
  dMatrix pointsXu,  pointsYu,  pointsZu;
  dMatrix pointsXv,  pointsYv,  pointsZv;
  dMatrix pointsXuv, pointsYuv, pointsZuv;

  // Create a carpet
  clCarpet carpet(1, pointsX, pointsY, pointsZ);
  carpet.GetFirstDerivativesU(pointsXu, pointsYu, pointsZu);
  carpet.GetFirstDerivativesV(pointsXv, pointsYv, pointsZv);
  carpet.GetCrossDerivatives(pointsXuv, pointsYuv, pointsZuv);

  // Count the patches
  int numberPatches = 0;

  // Get matrix dimensions
  int rmax = pointsX.GetNumberRows();     // rows    = constant v-values
  int cmax = pointsX.GetNumberColumns();  // columns = constant u-values

  // Declare geometry matrices
  dMatrix X(2, 2), XU(2, 2), XV(2, 2), XUV(2, 2);
  dMatrix Y(2, 2), YU(2, 2), YV(2, 2), YUV(2, 2);
  dMatrix Z(2, 2), ZU(2, 2), ZV(2, 2), ZUV(2, 2);

  // Remember: 1 element less than nodes (both directions)
  for(int r=1; r<rmax; r++)
  {
    for(int c=1; c<cmax; c++)
    {
      // Fill the X geometry matrices
      X.SetElement(1, 1, pointsX(r, c));       X.SetElement(1, 2, pointsX(r+1, c));
      X.SetElement(2, 1, pointsX(r, c+1));     X.SetElement(2, 2, pointsX(r+1, c+1));

      XU.SetElement(1, 1, pointsXu(r, c));     XU.SetElement(1, 2, pointsXu(r+1, c));
      XU.SetElement(2, 1, pointsXu(r, c+1));   XU.SetElement(2, 2, pointsXu(r+1, c+1));

      XV.SetElement(1, 1, pointsXv(r, c));     XV.SetElement(1, 2, pointsXv(r+1, c));
      XV.SetElement(2, 1, pointsXv(r, c+1));   XV.SetElement(2, 2, pointsXv(r+1, c+1));

      XUV.SetElement(1, 1, pointsXuv(r, c));   XUV.SetElement(1, 2, pointsXuv(r+1, c));
      XUV.SetElement(2, 1, pointsXuv(r, c+1)); XUV.SetElement(2, 2, pointsXuv(r+1, c+1));

      // Fill the Y geometry matrices
      Y.SetElement(1, 1, pointsY(r, c));       Y.SetElement(1, 2, pointsY(r+1, c));
      Y.SetElement(2, 1, pointsY(r, c+1));     Y.SetElement(2, 2, pointsY(r+1, c+1));

      YU.SetElement(1, 1, pointsYu(r, c));     YU.SetElement(1, 2, pointsYu(r+1, c));
      YU.SetElement(2, 1, pointsYu(r, c+1));   YU.SetElement(2, 2, pointsYu(r+1, c+1));

      YV.SetElement(1, 1, pointsYv(r, c));     YV.SetElement(1, 2, pointsYv(r+1, c));
      YV.SetElement(2, 1, pointsYv(r, c+1));   YV.SetElement(2, 2, pointsYv(r+1, c+1));

      YUV.SetElement(1, 1, pointsYuv(r, c));   YUV.SetElement(1, 2, pointsYuv(r+1, c));
      YUV.SetElement(2, 1, pointsYuv(r, c+1)); YUV.SetElement(2, 2, pointsYuv(r+1, c+1));


      // Fill the Z geometry matrices
      Z.SetElement(1, 1, pointsZ(r, c));       Z.SetElement(1, 2, pointsZ(r+1, c));
      Z.SetElement(2, 1, pointsZ(r, c+1));     Z.SetElement(2, 2, pointsZ(r+1, c+1));

      ZU.SetElement(1, 1, pointsZu(r, c));     ZU.SetElement(1, 2, pointsZu(r+1, c));
      ZU.SetElement(2, 1, pointsZu(r, c+1));   ZU.SetElement(2, 2, pointsZu(r+1, c+1));

      ZV.SetElement(1, 1, pointsZv(r, c));     ZV.SetElement(1, 2, pointsZv(r+1, c));
      ZV.SetElement(2, 1, pointsZv(r, c+1));   ZV.SetElement(2, 2, pointsZv(r+1, c+1));

      ZUV.SetElement(1, 1, pointsZuv(r, c));   ZUV.SetElement(1, 2, pointsZuv(r+1, c));
      ZUV.SetElement(2, 1, pointsZuv(r, c+1)); ZUV.SetElement(2, 2, pointsZuv(r+1, c+1));

      // Create a new patch
      clPatch *patch = new clPatch(numberPatches++, X, XU, XV, XUV, Y, YU, YV, YUV, Z, ZU, ZV, ZUV, 'H');

      // Add patch to surface
      AddPatchDelayed(patch);
    }
  }

  // Build the connectivity list
  BuildPatchConnectivity();
}


/*
** Method for calculating the normal vector to the surface
*/
dMatrix clSurface::GetNormal(const double u, const double v)
{
  dMatrix normal;

  normal.SetNumberRows(1);
  normal.SetNumberColumns(3);

  if(!initialised)
  {
    Initialise();
  }

  double uu = u;
  double vv = v;

  // Truncate uu and vv upon PRECISION
  if(fabs(uu - 0.0) < PRECISION)      uu = 0.0;
  else if(fabs(uu - 1.0) < PRECISION) uu = 1.0;

  if(fabs(vv - 0.0) < PRECISION)      vv = 0.0;
  else if(fabs(vv - 1.0) < PRECISION) vv = 1.0;

  // Check validity of uu and vv
  if(uu < 0.0 || uu > 1.0 || vv < 0.0 || vv > 1.0)
  {
    char message[MESLEN];
    sprintf(message, "Invalid value for u and/or v.\n  U = %f\n V = %f", uu, vv);
    throw clException("clSurface", "GetNormal", message);
  }

  // Find correct patch
  int i = 0; // Points to vertical node lines
  while(uu-(i+1)*uDelta > PRECISION)
  {
    i++;
  }

  int j = 0;  // Points to horizontal nodes lines
  while(vv-(j+1)*vDelta > PRECISION)
  {
    j++;
  }

  int patchPointer = GetPatchEntry(i, j);

  // Transform to patch local co-ordinate system
  double uPatch = (uu - i*uDelta)/uDelta;
  double vPatch = (vv - j*vDelta)/vDelta;

  // Truncate uPatch and vPatc upon PRECISION
  if(fabs(uPatch - 0.0) < PRECISION)      uPatch = 0.0;
  else if(fabs(uPatch - 1.0) < PRECISION) uPatch = 1.0;

  if(fabs(vPatch - 0.0) < PRECISION)      vPatch = 0.0;
  else if(fabs(vPatch - 1.0) < PRECISION) vPatch = 1.0;


  // Calculate the normal
  normal = patchList[patchPointer]->GetNormal(uPatch, vPatch);

  return (normal);
}

/*
** Methods for obtaining the extremes
*/

double clSurface::GetMinX(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(xMin);
}

double clSurface::GetMaxX(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(xMax);
}

double clSurface::GetMinY(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(yMin);
}

double clSurface::GetMaxY(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(yMax);
}

double clSurface::GetMinZ(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(zMin);
}

double clSurface::GetMaxZ(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(zMax);
}


/*
** Methods for operations
*/

clSurface& clSurface::Assign(const clSurface &source)
{
  SetNumberID(source.GetNumberID());

  for(int i=0; i<source.GetNumberPatches(); i++)
  {
    clPatch *patch = new clPatch(source.GetPatch(i));
    AddPatchDelayed(patch);
  }

  BuildPatchConnectivity();

  return (*this);
}

clSurface& clSurface::operator=(const clSurface &source)
{
  return(Assign(source));
}

dMatrix clSurface::GetCoordinates(const double u, const double v)
{
  dMatrix uDummy(1, 1);
  dMatrix vDummy(1, 1);

  uDummy.SetElement(1, 1, u);
  vDummy.SetElement(1, 1, v);

  return(GetCoordinates(uDummy, vDummy));
}

dMatrix clSurface::operator()(const double u, const double v)
{
  return(GetCoordinates(u, v));
}

dMatrix clSurface::GetCoordinates(const dMatrix &u, const dMatrix &v)
{
  dMatrix coord;

  if((u.GetNumberColumns() != 1 || u.GetNumberRows() == 0) ||
     (v.GetNumberColumns() != 1 || v.GetNumberRows() != u.GetNumberRows()))
  {
    throw clException("clSurface", "GetCoordinates", "Invalid dimesions for matrices u and/or v.");
  }
  else
  {
    coord.SetNumberRows(u.GetNumberRows());
    coord.SetNumberColumns(3);

    if(!initialised)
    {
      Initialise();
    }

    // Do for all specified points
    for(int r=1; r<=u.GetNumberRows(); r++)
    {
      double uu = u(r, 1);
      double vv = v(r, 1);

      // Truncate uu and vv upon PRECISION
      if(fabs(uu - 0.0) < PRECISION)      uu = 0.0;
      else if(fabs(uu - 1.0) < PRECISION) uu = 1.0;

      if(fabs(vv - 0.0) < PRECISION)      vv = 0.0;
      else if(fabs(vv - 1.0) < PRECISION) vv = 1.0;

      // Check validity of uu and vv
      if(uu < 0.0 || uu > 1.0 || vv < 0.0 || vv > 1.0)
      {
        throw clException("clSurface", "GetCoordinates", "Invalid value for u and/or v.");
      }

      // Find correct patch
      int i = 0; // Points to vertical node lines
      while(uu-(i+1)*uDelta > PRECISION)
      {
        i++;
      }

      int j = 0;  // Points to horizontal nodes lines
      while(vv-(j+1)*vDelta > PRECISION)
      {
        j++;
      }

      int patchPointer = GetPatchEntry(i, j);

      // Transform to patch local co-ordinate system
      double uPatch = (uu - i*uDelta)/uDelta;
      double vPatch = (vv - j*vDelta)/vDelta;

      // Truncate uPatch and vPatc upon PRECISION
      if(fabs(uPatch - 0.0) < PRECISION)      uPatch = 0.0;
      else if(fabs(uPatch - 1.0) < PRECISION) uPatch = 1.0;

      if(fabs(vPatch - 0.0) < PRECISION)      vPatch = 0.0;
      else if(fabs(vPatch - 1.0) < PRECISION) vPatch = 1.0;


      // Calculate the co-ordinates
      dMatrix temp = patchList[patchPointer]->GetCoordinates(uPatch, vPatch);

      coord.SetElement(r, 1, temp(1, 1));
      coord.SetElement(r, 2, temp(1, 2));
      coord.SetElement(r, 3, temp(1, 3));
    }
  }

  return (coord);
}

dMatrix clSurface::operator()(const dMatrix &u, const dMatrix &v)
{
  return(GetCoordinates(u, v));
}

