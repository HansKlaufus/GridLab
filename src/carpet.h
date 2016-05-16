/*****************************************************
*** Header file for definition of class clCarpet
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef CARPET_H
#define CARPET_H

#include "globals.h"
#include "matrix.h"

typedef tpMatrix<double> dMatrix;

class clCarpet
{
private:
  int     initialised;

  int     numberID;

  dMatrix x, xu, xv, xuv;
  dMatrix y, yu, yv, yuv;
  dMatrix z, zu, zv, zuv;

  int  SetNumberID(const int);
  void SetCoordinates(const dMatrix&, const dMatrix&, const dMatrix&);

  void Initialise(void);
  void CalcFirstDerivativesU(void);
  void CalcFirstDerivativesV(void);
  void CalcCrossDerivatives(void);

public:
  clCarpet(const int, const dMatrix&, const dMatrix&, const dMatrix&);
  clCarpet(const clCarpet&);
  ~clCarpet(void);

  int       GetNumberID(void) const;
  void      GetCoordinates(dMatrix&, dMatrix&, dMatrix&) const;
  void      GetFirstDerivativesU(dMatrix&, dMatrix&, dMatrix&);
  void      GetFirstDerivativesV(dMatrix&, dMatrix&, dMatrix&);
  void      GetCrossDerivatives(dMatrix&, dMatrix&, dMatrix&);

  clCarpet& Assign(const clCarpet&);
  clCarpet& operator=(const clCarpet&);
};

#endif
