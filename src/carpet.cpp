/*****************************************************
*** Body file for definition of class clCarpet
***
*** J.L. Klaufus - Cranfield 1999
**/

#include "carpet.h"
#include "exception.h"
#include "matrix.h"
#include "spline.h"

/*
**----------------
** Private methods
**----------------
*/

int clCarpet::SetNumberID(const int number)
{
  if(number<0)
  {
    throw clException("clCarpet", "SetNumberID", "NumberID must be greater than zero.");
  }

  numberID = number;

  return(numberID);
}

void clCarpet::SetCoordinates(const dMatrix &pointsX, const dMatrix &pointsY, const dMatrix &pointsZ)
{
  int rmax = pointsX.GetNumberRows();
  int cmax = pointsX.GetNumberColumns();
  
  // Check for matching dimensions of matrices
  if(pointsY.GetNumberRows()!=rmax    || pointsZ.GetNumberRows()!=rmax ||
     pointsY.GetNumberColumns()!=cmax || pointsZ.GetNumberColumns()!=cmax)
  {
    throw clException("clCarpet", "SetCoordinates", "Dimensions of matrices do not match.");
  }

  // Check for minimum dimensions of matrices
  if(rmax<2 || cmax<2)
  {
    throw clException("clCarpet", "SetCoordinates", "Dimensions of matrices too small to spline.");
  }

  // Copy the matrices
  x = pointsX;
  y = pointsY;
  z = pointsZ;

  // If rmax==2 or cmax==2 then third points have to be calculated
  if(rmax==2)
  {
    dMatrix xDummy(x);
    dMatrix yDummy(y);
    dMatrix zDummy(z);

    // Set the number of rows
    x.SetNumberRows(3);
    y.SetNumberRows(3);
    z.SetNumberRows(3);

    rmax = 3;

    for(int c=1; c<=cmax; c++)
    {
      // Copy the first row
      x.SetElement(1, c, xDummy(1, c));
      y.SetElement(1, c, yDummy(1, c));
      z.SetElement(1, c, zDummy(1, c));

      // Copy the second row to the third row
      x.SetElement(3, c, xDummy(2, c));
      y.SetElement(3, c, yDummy(2, c));
      z.SetElement(3, c, zDummy(2, c));

      // Calculate the second row by linear interpolation
      x.SetElement(2, c, (x(1, c)+x(3, c))/2.0f);
      y.SetElement(2, c, (y(1, c)+y(3, c))/2.0f);
      z.SetElement(2, c, (z(1, c)+z(3, c))/2.0f);
    }
  }

  if(cmax==2)
  {
    dMatrix xDummy(x);
    dMatrix yDummy(y);
    dMatrix zDummy(z);

    // Set the number of columns
    x.SetNumberColumns(3);
    y.SetNumberColumns(3);
    z.SetNumberColumns(3);

    cmax = 3;

    for(int r=1; r<=rmax; r++)
    {
      // Copy the first column
      x.SetElement(r, 1, xDummy(r, 1));
      y.SetElement(r, 1, yDummy(r, 1));
      z.SetElement(r, 1, zDummy(r, 1));

      // Copy the second columns to the third column
      x.SetElement(r, 3, xDummy(r, 2));
      y.SetElement(r, 3, yDummy(r, 2));
      z.SetElement(r, 3, zDummy(r, 2));

      // Calculate the second column by linear interpolation
      x.SetElement(r, 2, (x(r, 1)+x(r, 3))/2.0f);
      y.SetElement(r, 2, (y(r, 1)+y(r, 3))/2.0f);
      z.SetElement(r, 2, (z(r, 1)+z(r, 3))/2.0f);
    }
  }
}

void clCarpet::Initialise(void)
{
  if(!initialised)
  {
    CalcFirstDerivativesU();
    CalcFirstDerivativesV();
    CalcCrossDerivatives();

    initialised = 1;
  }
}

void clCarpet::CalcFirstDerivativesU(void)
{
  // Find matrix dimensions
  int rmax = x.GetNumberRows();
  int cmax = x.GetNumberColumns();

  // Set all matrix dimensions
  xu.SetNumberRows(rmax); xu.SetNumberColumns(cmax);
  yu.SetNumberRows(rmax); yu.SetNumberColumns(cmax);
  zu.SetNumberRows(rmax); zu.SetNumberColumns(cmax);

  // Set up temporary vectors
  dMatrix tempX(cmax, 1);
  dMatrix tempY(cmax, 1);
  dMatrix tempZ(cmax, 1);

  // Spline u-nodes = spline rows
  for(int r=1; r<=rmax; r++)
  {
    // Fill temporary 
    for(int c=1; c<=cmax; c++)
    {
      tempX.SetElement(c, 1, x(r, c));
      tempY.SetElement(c, 1, y(r, c));
      tempZ.SetElement(c, 1, z(r, c));
    }
    
    // Spline the temporary arrays
    clSpline splineU(0, tempX, tempY, tempZ);

    // Calculate the first derivatives
    dMatrix tempXYZ = splineU.GetFirstDerivatives();

    // Copy back into appropriate matrices
    {
      for(int c=1; c<=cmax; c++)
      {
        xu.SetElement(r, c, tempXYZ(c, 1));
        yu.SetElement(r, c, tempXYZ(c, 2));
        zu.SetElement(r, c, tempXYZ(c, 3));
      }
    }
  }
}

void clCarpet::CalcFirstDerivativesV(void)
{
  // Find matrix dimensions
  int rmax = x.GetNumberRows();
  int cmax = x.GetNumberColumns();

  // Set all matrix dimensions; U in columns; V in rows
  xv.SetNumberRows(rmax); xv.SetNumberColumns(cmax);
  yv.SetNumberRows(rmax); yv.SetNumberColumns(cmax);
  zv.SetNumberRows(rmax); zv.SetNumberColumns(cmax);

  // Set up temporary matrices
  dMatrix tempX(rmax, 1);
  dMatrix tempY(rmax, 1);
  dMatrix tempZ(rmax, 1);

  // Spline v-nodes = spline columns
  for(int c=1; c<=cmax; c++)
  {
    // Fill temporary matrices
    for(int r=1; r<=rmax; r++)
    {
      tempX.SetElement(r, 1, x(r, c));
      tempY.SetElement(r, 1, y(r, c));
      tempZ.SetElement(r, 1, z(r, c));
    }
    
    // Spline the temporary arrays
    clSpline splineV(0, tempX, tempY, tempZ);

    // Calculate the first derivatives
    dMatrix tempXYZ = splineV.GetFirstDerivatives();

    // Copy back into appropriate matrices
    {
      for(int r=1; r<=rmax; r++)
      {
        xv.SetElement(r, c, tempXYZ(r, 1));
        yv.SetElement(r, c, tempXYZ(r, 2));
        zv.SetElement(r, c, tempXYZ(r, 3));
      }
    }
  }
}

void clCarpet::CalcCrossDerivatives(void)
{
  // Find matrix dimensions
  int rmax = x.GetNumberRows();
  int cmax = x.GetNumberColumns();

  // Set all matrix dimensions; U in columns; V in rows
  xuv.SetNumberRows(rmax); xuv.SetNumberColumns(cmax);
  yuv.SetNumberRows(rmax); yuv.SetNumberColumns(cmax);
  zuv.SetNumberRows(rmax); zuv.SetNumberColumns(cmax);

  // Set up temporary matrices
  dMatrix tempX(rmax, 1);
  dMatrix tempY(rmax, 1);
  dMatrix tempZ(rmax, 1);

  // Spline v-nodes = spline columns
  for(int c=1; c<=cmax; c++)
  {
    // Fill temporary matrices
    for(int r=1; r<=rmax; r++)
    {
      tempX.SetElement(r, 1, xu(r, c));
      tempY.SetElement(r, 1, yu(r, c));
      tempZ.SetElement(r, 1, zu(r, c));
    }
    
    // Spline the temporary arrays
    clSpline splineUV(0, tempX, tempY, tempZ);

    // Calculate the first derivatives
    dMatrix tempXYZ = splineUV.GetFirstDerivatives();

    // Copy back into appropriate matrices
    {
      for(int r=1; r<=rmax; r++)
      {
        xuv.SetElement(r, c, tempXYZ(r, 1));
        yuv.SetElement(r, c, tempXYZ(r, 2));
        zuv.SetElement(r, c, tempXYZ(r, 3));
      }
    }
  }
}


/*
**----------------
** Public methods
**----------------
*/

/*
** Contructors and Destructor
*/
clCarpet::clCarpet(const int number, const dMatrix &pointsX, const dMatrix &pointsY, const dMatrix &pointsZ)
{
  initialised = 0;

  SetNumberID(number);
  SetCoordinates(pointsX, pointsY, pointsZ);
}

clCarpet::clCarpet(const clCarpet &source)
{
  initialised = 0;

  Assign(source);
}

clCarpet::~clCarpet(void)
{
  //cerr << "DESTROYING CARPET: " << GetNumberID() << endl;
}

/*
** Methods to query the member variables
*/
int clCarpet::GetNumberID(void) const
{
  return(numberID);
}

void clCarpet::GetCoordinates(dMatrix &xCopy, dMatrix &yCopy, dMatrix &zCopy) const
{
  xCopy = x;
  yCopy = y;
  zCopy = z;
}

void clCarpet::GetFirstDerivativesU(dMatrix &xuCopy, dMatrix &yuCopy, dMatrix &zuCopy)
{
  if(!initialised)
  {
    Initialise();
  }

  xuCopy = xu;
  yuCopy = yu;
  zuCopy = zu;
}

void clCarpet::GetFirstDerivativesV(dMatrix &xvCopy, dMatrix &yvCopy, dMatrix &zvCopy)
{
  if(!initialised)
  {
    Initialise();
  }

  xvCopy = xv;
  yvCopy = yv;
  zvCopy = zv;
}

void clCarpet::GetCrossDerivatives(dMatrix &xuvCopy, dMatrix &yuvCopy, dMatrix &zuvCopy)
{
  if(!initialised)
  {
    Initialise();
  }

  xuvCopy = xuv;
  yuvCopy = yuv;
  zuvCopy = zuv;
}

/*
** Methods for operations
*/
clCarpet& clCarpet::Assign(const clCarpet &source)
{
  SetNumberID(source.GetNumberID());

  source.GetCoordinates(x, y, z);

  return(*this);
}

clCarpet& clCarpet::operator=(const clCarpet &source)
{
  return(Assign(source));
}
