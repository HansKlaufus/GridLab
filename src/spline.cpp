/**********************************************
*** Body file for definition of class clSpline
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <stdio.h>
#include <math.h>

#include "exception.h"
#include "matrix.h"
#include "spline.h"


/*
** ---------------
** Private Methods
** ---------------
*/

void clSpline::Initialise(void)
{
  if(!initialised)
  {
    Parameterise();
    SecondDerivatives();
    FirstDerivatives();

    initialised = 1;
  }
}

void clSpline::Parameterise(void)
{
  int rmax = X.GetNumberRows();

  u.SetNumberRows(rmax);
  u.SetNumberColumns(1);

  // Calculate the points distances
  u.SetElement(1, 1, 0.0);
  for(int i=2; i<=rmax; i++)
  {
    double xdist = X(i, 1) - X(i-1, 1);
    double ydist = Y(i, 1) - Y(i-1, 1);
    double zdist = Z(i, 1) - Z(i-1, 1);
    double dist  = u(i-1, 1) + sqrt(xdist*xdist + ydist*ydist + zdist*zdist);

    u.SetElement(i, 1, dist);
  }

  splineLength = u(rmax, 1);
}

void clSpline::FirstDerivatives(void)
{
  double deltaU;
  double deltaX;
  double deltaY;
  double deltaZ;

  // Get maximum number of rows
  int rmax = u.GetNumberRows();

  X1.SetNumberRows(rmax); X1.SetNumberColumns(1);
  Y1.SetNumberRows(rmax); Y1.SetNumberColumns(1);
  Z1.SetNumberRows(rmax); Z1.SetNumberColumns(1);

  for(int r=1; r<rmax; r++)
  {
    deltaU = u.GetElement(r+1, 1) - u.GetElement(r, 1);
    if(fabs(deltaU)<PRECISION)
    {
      deltaU = PRECISION;
      //throw clException("clSpline", "FirstDerivatives", "Division by zero.");
    }

    deltaX = X.GetElement(r+1, 1) - X.GetElement(r, 1);
    deltaY = Y.GetElement(r+1, 1) - Y.GetElement(r, 1);
    deltaZ = Z.GetElement(r+1, 1) - Z.GetElement(r, 1);

    X1.SetElement(r, 1, fabs(deltaX)*(deltaX/deltaU - deltaU*X2(r, 1)/3.0 - deltaU*X2(r+1, 1)/6.0));
    Y1.SetElement(r, 1, fabs(deltaY)*(deltaY/deltaU - deltaU*Y2(r, 1)/3.0 - deltaU*Y2(r+1, 1)/6.0));
    Z1.SetElement(r, 1, fabs(deltaZ)*(deltaZ/deltaU - deltaU*Z2(r, 1)/3.0 - deltaU*Z2(r+1, 1)/6.0));
  }

  // Do last control point; work backwards
  deltaU = u.GetElement(rmax-1, 1) - u.GetElement(rmax, 1);
  if(fabs(deltaU)<PRECISION)
  {
    deltaU = PRECISION;
    //throw clException("clSpline", "FirstDerivatives", "Division by zero.");
  }

  deltaX = X.GetElement(rmax-1, 1) - X.GetElement(rmax, 1);
  deltaY = Y.GetElement(rmax-1, 1) - Y.GetElement(rmax, 1);
  deltaZ = Z.GetElement(rmax-1, 1) - Z.GetElement(rmax, 1);

  X1.SetElement(rmax, 1, fabs(deltaX)*(deltaX/deltaU - deltaU*X2(rmax, 1)/3.0 - deltaU*X2(rmax-1, 1)/6.0));
  Y1.SetElement(rmax, 1, fabs(deltaY)*(deltaY/deltaU - deltaU*Y2(rmax, 1)/3.0 - deltaU*Y2(rmax-1, 1)/6.0));
  Z1.SetElement(rmax, 1, fabs(deltaZ)*(deltaZ/deltaU - deltaU*Z2(rmax, 1)/3.0 - deltaU*Z2(rmax-1, 1)/6.0));
}

void clSpline::SecondDerivatives(void)
{
  // Get maximum number of rows
  int rmax = u.GetNumberRows();

  // Check for minimum size matrix
  if(rmax<3)
  {
    throw clException("clSpline", "SecondDerivatives", "Need at least 3 points to create spline.");
  }

  // Set up matrices
  dMatrix tri(rmax-2, 3);

  dMatrix SX(rmax-2, 1);
  dMatrix SY(rmax-2, 1);
  dMatrix SZ(rmax-2, 1);

  // Set up all coefficients
  for(int i=2; i<rmax; i++)
  {
    double alpha = u(i, 1)-u(i-1, 1)       + PRECISION;
    double beta  = 2*(u(i+1, 1)-u(i-1, 1)) + PRECISION;
    double gamma = u(i+1, 1)-u(i, 1)       + PRECISION;

    int j=i-1;
    tri.SetElement(j, 1, alpha);
    tri.SetElement(j, 2, beta);
    tri.SetElement(j, 3, gamma);

    SX.SetElement(j, 1, 6*((X(i+1, 1) - X(i  , 1))/gamma - (X(i  , 1) - X(i-1, 1))/alpha) + PRECISION);
    SY.SetElement(j, 1, 6*((Y(i+1, 1) - Y(i  , 1))/gamma - (Y(i  , 1) - Y(i-1, 1))/alpha) + PRECISION);
    SZ.SetElement(j, 1, 6*((Z(i+1, 1) - Z(i  , 1))/gamma - (Z(i  , 1) - Z(i-1, 1))/alpha) + PRECISION);
  }

  // Calculate the second derivatives
  tri.SolveTri(SX);
  tri.SolveTri(SY);
  tri.SolveTri(SZ);

  // Substitute in appropiate vectors
  X2.SetNumberRows(rmax); X2.SetNumberColumns(1);
  Y2.SetNumberRows(rmax); Y2.SetNumberColumns(1);
  Z2.SetNumberRows(rmax); Z2.SetNumberColumns(1);

  {
    for(int i=1; i<=rmax; i++)
    {
      if(i==1 || i==rmax)
      {
        X2.SetElement(i, 1, (double)(0));
        Y2.SetElement(i, 1, (double)(0));
        Z2.SetElement(i, 1, (double)(0));
      }
      else
      {
        X2.SetElement(i, 1, SX(i-1, 1));
        Y2.SetElement(i, 1, SY(i-1, 1));
        Z2.SetElement(i, 1, SZ(i-1, 1));
      }
    }
  }
}

/*
** --------------
** Public Methods
** --------------
*/

/*
** Constructors and Destructor
*/

clSpline::clSpline(void)
{
  SetNumberID(0);
}

clSpline::clSpline(const int ID, const dMatrix &cPoints)
{
  initialised = 0;

  SetNumberID(ID);
  SetControlPoints(cPoints);
}

clSpline::clSpline(const int ID, const dMatrix &cPointsX, const dMatrix &cPointsY, const dMatrix &cPointsZ)
{
  initialised = 0;

  SetNumberID(ID);
  SetControlPoints(cPointsX, cPointsY, cPointsZ);
}

clSpline::clSpline(const clSpline &source)
{
  Assign(source);
}

clSpline::~clSpline(void)
{
  //cerr << "DESTROYING SPLINE: " << GetNumberID() << endl;
}

/*
** Methods to set and query the member variables
*/

int clSpline::SetNumberID(const int ID)
{
  numberID = ID;

  return(numberID);
}

int clSpline::GetNumberID(void) const
{
  return(numberID);
}

void clSpline::SetControlPoints(const dMatrix &cPoints)
{
  // Check if matrix is in correct format.
  //   3 columns; 1 = x; 2 = y; 3 = z
  //   Number of rows is number of control points
  if(cPoints.GetNumberColumns() != 3)
  {
    throw clException("clSpline", "SetControlPoints", "Incorrect number of columns.");
  }
  else
  {
    int rmax = cPoints.GetNumberColumns();
    X.SetNumberRows(rmax); X.SetNumberColumns(1);
    Y.SetNumberRows(rmax); Y.SetNumberColumns(1);
    Z.SetNumberRows(rmax); Z.SetNumberColumns(1);

    for(int i=1; i<=rmax; i++)
    {
      X.SetElement(i, 1, cPoints(i, 1));
      Y.SetElement(i, 1, cPoints(i, 2));
      Z.SetElement(i, 1, cPoints(i, 3));
    }
  }
}

void clSpline::SetControlPoints(const dMatrix &cPointsX, const dMatrix &cPointsY, const dMatrix &cPointsZ)
{
  // Check if dimensions are correct
  if((cPointsX.GetNumberColumns() != 1 || cPointsY.GetNumberColumns() != 1 || cPointsZ.GetNumberColumns() != 1) ||
     (cPointsX.GetNumberRows() != cPointsY.GetNumberRows()) ||
     (cPointsX.GetNumberRows() != cPointsZ.GetNumberRows()))
  {
    throw clException("clSpline", "SetControlPoints", "Dimensions of matrices do not match.");
  }
  else
  {
    X = cPointsX;
    Y = cPointsY;
    Z = cPointsZ;
  }
}

double clSpline::GetSplineLength(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(splineLength);
}

dMatrix clSpline::GetControlPoints(void) const
{
  dMatrix dummy;

  dummy.SetNumberRows(X.GetNumberRows());
  dummy.SetNumberColumns(3);

  for(int i=1; i<=X.GetNumberRows(); i++)
  {
    dummy.SetElement(i, 1, X(i, 1));
    dummy.SetElement(i, 2, Y(i, 1));
    dummy.SetElement(i, 3, Z(i, 1));
  }

  return(dummy);
}

dMatrix  clSpline::GetFirstDerivatives(void)
{
  dMatrix dummy;

  if(!initialised)
  {
    Initialise();
  }

  dummy.SetNumberRows(X1.GetNumberRows());
  dummy.SetNumberColumns(3);

  for(int i=1; i<=X1.GetNumberRows(); i++)
  {
    dummy.SetElement(i, 1, fabs(X1(i, 1)) < PRECISION ? 0.0 : X1(i, 1));
    dummy.SetElement(i, 2, fabs(Y1(i, 1)) < PRECISION ? 0.0 : Y1(i, 1));
    dummy.SetElement(i, 3, fabs(Z1(i, 1)) < PRECISION ? 0.0 : Z1(i, 1));
  }

  return(dummy);
}

dMatrix  clSpline::GetSecondDerivatives(void)
{
  dMatrix dummy;

  if(!initialised)
  {
    Initialise();
  }

  dummy.SetNumberRows(X2.GetNumberRows());
  dummy.SetNumberColumns(3);

  for(int i=1; i<=X2.GetNumberRows(); i++)
  {
    dummy.SetElement(i, 1, fabs(X2(i, 1)) < PRECISION ? 0.0 : X2(i, 1));
    dummy.SetElement(i, 2, fabs(Y2(i, 1)) < PRECISION ? 0.0 : Y2(i, 1));
    dummy.SetElement(i, 3, fabs(Z2(i, 1)) < PRECISION ? 0.0 : Z2(i, 1));
  }

  return(dummy);
}

dMatrix clSpline::GetCoordinates(const double uSpec)
{
  dMatrix uDummy(1, 1);
  uDummy.SetElement(1, 1, uSpec);

  return(GetCoordinates(uDummy));
}

dMatrix clSpline::GetCoordinates(const dMatrix &uSpec)
{
  dMatrix coord;

  if(uSpec.GetNumberColumns() != 1 || uSpec.GetNumberRows() == 0)
  {
    throw clException("clSpline", "GetCoordinates", "Invalid dimensions of matrix uSpec.");
  }
  else
  {
    coord.SetNumberRows(uSpec.GetNumberRows());
    coord.SetNumberColumns(3);

    if(!initialised)
    {
      Initialise();
    }

    // Do for all specified u's
    for(int i=1; i<=uSpec.GetNumberRows(); i++)
    {
      if(uSpec(i, 1) < 0 || uSpec(i, 1) > 1)
      {
        throw clException("clSpline", "GetCoordinates", "Invalid value for uSpec.");
      }
      else
      {
        // Now find the position of uSpec
        double uu = uSpec(i, 1)*GetSplineLength();

        int j = 1;
        while((uu - u(j+1, 1) > 0) && (j<u.GetNumberRows()-1))
        {
          j++;
        }

        // Now calculate the coefficients
        double A = (u(j+1, 1)-uu)/(u(j+1, 1)-u(j,1));
        double B = 1-A;
        double C = (A*A*A-A)/6 * (u(j+1, 1)-u(j, 1))*(u(j+1, 1)-u(j, 1));
        double D = (B*B*B-B)/6 * (u(j+1, 1)-u(j, 1))*(u(j+1, 1)-u(j, 1));

        // Finally calculate the coordinates
        coord.SetElement(i, 1, A*X(j, 1) + B*X(j+1, 1) + C*X2(j, 1) + D*X2(j+1, 1));
        coord.SetElement(i, 2, A*Y(j, 1) + B*Y(j+1, 1) + C*Y2(j, 1) + D*Y2(j+1, 1));
        coord.SetElement(i, 3, A*Z(j, 1) + B*Z(j+1, 1) + C*Z2(j, 1) + D*Z2(j+1, 1));
      }
    }
  }

  return (coord);
}

/*
** Methods for operations
*/

clSpline& clSpline::Assign(const clSpline &source)
{
  initialised = 0;

  SetNumberID(source.GetNumberID());
  SetControlPoints(source.GetControlPoints());

  return(*this);
}

clSpline& clSpline::operator=(const clSpline &source)
{
  return(Assign(source));
}

