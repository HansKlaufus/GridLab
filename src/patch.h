/**************************************************
*** Header file for declaration of class clPatch
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef PATCH_H
#define PATCH_H

#include "globals.h"
#include "matrix.h"

typedef tpMatrix<double> dMatrix;

class clPatch
{
private:
  int     initialised;

  int     numberID;
  dMatrix GX;
  dMatrix GY;
  dMatrix GZ;
  dMatrix QX;
  dMatrix QY;
  dMatrix QZ;
  char    blendType;

  double  xMin, xMax;
  double  yMin, yMax;
  double  zMin, zMax;

  void    Initialise(void);
  void    BlendIt(void);
  void    Hermite(void);
  void    Bezier(void);
  void    CalculateExtremes(void);

public:
  clPatch(void);
  clPatch(const int,
          const dMatrix&, const dMatrix&, const dMatrix&, const dMatrix&,
          const dMatrix&, const dMatrix&, const dMatrix&, const dMatrix&,
          const dMatrix&, const dMatrix&, const dMatrix&, const dMatrix&, const char);
  clPatch(const int, const dMatrix&, const dMatrix&, const dMatrix&, const char);
  clPatch(const clPatch&);
  ~clPatch(void);

  int      GetInitialised(void) const;

  int      SetNumberID(const int);
  int      GetNumberID(void) const;

  void     SetGX(const dMatrix&);
  void     SetGX(const dMatrix&, const dMatrix&, const dMatrix&, const dMatrix&);
  dMatrix  GetGX(void) const;

  void     SetGY(const dMatrix&);
  void     SetGY(const dMatrix&, const dMatrix&, const dMatrix&, const dMatrix&);
  dMatrix  GetGY(void) const;

  void     SetGZ(const dMatrix&);
  void     SetGZ(const dMatrix&, const dMatrix&, const dMatrix&, const dMatrix&);
  dMatrix  GetGZ(void) const;

  char     SetBlendType(const char);
  char     GetBlendType(void) const;

  void     HermiteToBezier(void);

  dMatrix  GetNormal(const double, const double);

  double   GetMinX(void);
  double   GetMaxX(void);
  double   GetMinY(void);
  double   GetMaxY(void);
  double   GetMinZ(void);
  double   GetMaxZ(void);

  clPatch& Assign(const clPatch&);
  clPatch& operator=(const clPatch&);

  dMatrix  GetCoordinates(const double, const double);
  dMatrix  operator()(const double, const double);
  dMatrix  GetCoordinates(const dMatrix&, const dMatrix&);
  dMatrix  operator()(const dMatrix&, const dMatrix&);
};

#endif
