/*************************************************
*** Header file for declaration of class clSpline
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef SPLINE_H
#define SPLINE_H

#include "globals.h"
#include "matrix.h"

typedef tpMatrix<double> dMatrix;

class clSpline
{
private:
  int     initialised;
  double  splineLength;

  int     numberID;
  dMatrix u;
  dMatrix X; dMatrix X1; dMatrix X2;
  dMatrix Y; dMatrix Y1; dMatrix Y2;
  dMatrix Z; dMatrix Z1; dMatrix Z2;

  void Initialise(void);
  void Parameterise(void);
  void FirstDerivatives(void);
  void SecondDerivatives(void);

public:
  clSpline(void);
  clSpline(const int, const dMatrix&);
  clSpline(const int, const dMatrix&, const dMatrix&, const dMatrix&);
  clSpline(const clSpline&);
  ~clSpline(void);

  int       SetNumberID(const int);
  int       GetNumberID(void) const;

  void      SetControlPoints(const dMatrix&);
  void      SetControlPoints(const dMatrix&, const dMatrix&, const dMatrix&);
  dMatrix   GetControlPoints(void) const;

  double    GetSplineLength(void);

  dMatrix   GetFirstDerivatives(void);
  dMatrix   GetSecondDerivatives(void);

  dMatrix   GetCoordinates(const double);
  dMatrix   GetCoordinates(const dMatrix&);

  clSpline& Assign(const clSpline&);
  clSpline& operator=(const clSpline&);
};

#endif
