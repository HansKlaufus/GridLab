/******************************************************
*** Header file for declaration of class clSurface
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef SURFACE_H
#define SURFACE_H

#include <vector>

#include "globals.h"
#include "list.h"
#include "matrix.h"
#include "patch.h"

using namespace std;

typedef tpMatrix<double> dMatrix;

class clSurface
{
private:
  typedef struct sConnect
  {
    int      numberID;
    int      north, west, south, east;
  } stConnect;

  int       initialised;

  int       numberID;
  vector<clPatch*>  patchList;
  vector<int>       remainList;
  tpList<stConnect> connectList;

  double    xMin, xMax;
  double    yMin, yMax;
  double    zMin, zMax;

  int       origin;
  double    uDelta;
  double    vDelta;

  int       SetNumberID(const int);

  int       AddConnection(const int, const int, const int, const int, const int);
  void      ConnectPatch(const int);
  void      CheckConnection(void);
  void      Parameterise(void);
  void      CalculateExtremes(void);
  void      Initialise(void);

public:
  clSurface(void);
  clSurface(const int);
  clSurface(const clSurface&);
  clSurface(const dMatrix&, const dMatrix&, const dMatrix&, const int);
  ~clSurface(void);

  int        GetNumberID(void) const;
  int        GetNumberPatches(void) const;
  int        GetNumberConnect(void) const;
  int        GetPatchEntry(const int, const int) const;

  int        AddPatch(clPatch*);
  int        AddPatchDelayed(clPatch*);
  int        DeletePatch(const int);
  clPatch    GetPatch(const int) const;
  clPatch*   GetPatchPointer(const int) const;
  void       BuildPatchConnectivity(void);
  void       CreatePatches(const dMatrix&, const dMatrix&, const dMatrix&);

  double     GetMinX(void);
  double     GetMaxX(void);
  double     GetMinY(void);
  double     GetMaxY(void);
  double     GetMinZ(void);
  double     GetMaxZ(void);

  clSurface& Assign(const clSurface&);
  clSurface& operator=(const clSurface&);

  dMatrix GetNormal(const double, const double);
  dMatrix GetCoordinates(const double, const double);
  dMatrix operator()(const double, const double);
  dMatrix GetCoordinates(const dMatrix&, const dMatrix&);
  dMatrix operator()(const dMatrix&, const dMatrix&);
};

#endif
