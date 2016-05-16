/**********************************************
*** Body file for definition of class clPatch
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <math.h>
#include <stdio.h>

#include "exception.h"
#include "matrix.h"
#include "patch.h"

using namespace std;

/*
** ---------------
** Private methods
** ---------------
*/

void clPatch::Initialise(void)
{
  if(!initialised && GetBlendType()!='U')
  {
    initialised = 1;

    BlendIt();
    CalculateExtremes();
  }
}

void clPatch::BlendIt(void)
{
  // Check for valid blending type
  if (GetBlendType() == 'H')
  {
    Hermite();
  }
  else if (GetBlendType() == 'B')
  {
    Bezier();
  }
  else
  {
    throw clException("clPatch", "BlendIt", "Unknown type for blending functions.");
  }
}

void clPatch::Hermite(void)
{
  // Define the Basis Matrices
  dMatrix MU(4, 4);
  MU.SetElement(1, 1,  2); MU.SetElement(1, 2, -2); MU.SetElement(1, 3,  1); MU.SetElement(1, 4,  1);
  MU.SetElement(2, 1, -3); MU.SetElement(2, 2,  3); MU.SetElement(2, 3, -2); MU.SetElement(2, 4, -1);
  MU.SetElement(3, 1,  0); MU.SetElement(3, 2,  0); MU.SetElement(3, 3,  1); MU.SetElement(3, 4,  0);
  MU.SetElement(4, 1,  1); MU.SetElement(4, 2,  0); MU.SetElement(4, 3,  0); MU.SetElement(4, 4,  0);

  dMatrix MV(MU);
  MV.Transpose();

  // Multiply the Geometry Matrices with the Basis Matrices
  QX = GX;
  QY = GY;
  QZ = GZ;
  QX.PostMultiply(MV); QX.PreMultiply(MU);
  QY.PostMultiply(MV); QY.PreMultiply(MU);
  QZ.PostMultiply(MV); QZ.PreMultiply(MU);
}

void clPatch::Bezier(void)
{
  // Define the Basis Matrices
  dMatrix MU(4, 4);
  MU.SetElement(1, 1, -1); MU.SetElement(1, 2,  3); MU.SetElement(1, 3, -3); MU.SetElement(1, 4,  1);
  MU.SetElement(2, 1,  3); MU.SetElement(2, 2, -6); MU.SetElement(2, 3,  3); MU.SetElement(2, 4,  0);
  MU.SetElement(3, 1, -3); MU.SetElement(3, 2,  3); MU.SetElement(3, 3,  0); MU.SetElement(3, 4,  0);
  MU.SetElement(4, 1,  1); MU.SetElement(4, 2,  0); MU.SetElement(4, 3,  0); MU.SetElement(4, 4,  0);

  dMatrix MV(MU);
  MV.Transpose();

  // Multiply the Geometry Matrices with the Basis Matrices
  QX = GX;
  QY = GY;
  QZ = GZ;
  QX.PostMultiply(MV); QX.PreMultiply(MU);
  QY.PostMultiply(MV); QY.PreMultiply(MU);
  QZ.PostMultiply(MV); QZ.PreMultiply(MU);
}

void clPatch::CalculateExtremes(void)
{
  dMatrix coord;

  // Calculates the extremes on basis of the specified cornerpoints

  coord = GetCoordinates(0.0, 0.0);
  xMin = xMax = coord(1,1);
  yMin = yMax = coord(1,2);
  zMin = zMax = coord(1,3);

  coord = GetCoordinates(1.0, 0.0);
  xMin = (xMin < coord(1,1)) ? xMin : coord(1,1);  xMax = (xMax > coord(1,1)) ? xMax : coord(1,1);
  yMin = (yMin < coord(1,2)) ? yMin : coord(1,2);  yMax = (yMax > coord(1,2)) ? yMax : coord(1,2);
  zMin = (zMin < coord(1,3)) ? zMin : coord(1,3);  zMax = (zMax > coord(1,3)) ? zMax : coord(1,3);

  coord = GetCoordinates(0.0, 1.0);
  xMin = (xMin < coord(1,1)) ? xMin : coord(1,1);  xMax = (xMax > coord(1,1)) ? xMax : coord(1,1);
  yMin = (yMin < coord(1,2)) ? yMin : coord(1,2);  yMax = (yMax > coord(1,2)) ? yMax : coord(1,2);
  zMin = (zMin < coord(1,3)) ? zMin : coord(1,3);  zMax = (zMax > coord(1,3)) ? zMax : coord(1,3);

  coord = GetCoordinates(1.0, 1.0);
  xMin = (xMin < coord(1,1)) ? xMin : coord(1,1);  xMax = (xMax > coord(1,1)) ? xMax : coord(1,1);
  yMin = (yMin < coord(1,2)) ? yMin : coord(1,2);  yMax = (yMax > coord(1,2)) ? yMax : coord(1,2);
  zMin = (zMin < coord(1,3)) ? zMin : coord(1,3);  zMax = (zMax > coord(1,3)) ? zMax : coord(1,3);
}

/*
** --------------
** Public methods
** --------------
*/


/*
** Constructors and Destructor
*/
clPatch::clPatch(void)
{
  SetNumberID(0);
  SetBlendType('U');

  initialised  = 0;
  xMin         = 0.0;
  xMax         = 0.0;
  yMin         = 0.0;
  yMax         = 0.0;
  zMin         = 0.0;
  zMax         = 0.0;
}

clPatch::clPatch(const int number,
                 const dMatrix &X, const dMatrix &XU, const dMatrix &XV, const dMatrix &XUV,
                 const dMatrix &Y, const dMatrix &YU, const dMatrix &YV, const dMatrix &YUV,
                 const dMatrix &Z, const dMatrix &ZU, const dMatrix &ZV, const dMatrix &ZUV, const char type)
{
  SetNumberID(number);
  SetGX(X, XU, XV, XUV);
  SetGY(Y, YU, YV, YUV);
  SetGZ(Z, ZU, ZV, ZUV);
  SetBlendType(type);

  initialised  = 0;
  xMin         = 0.0;
  xMax         = 0.0;
  yMin         = 0.0;
  yMax         = 0.0;
  zMin         = 0.0;
  zMax         = 0.0;
}

clPatch::clPatch(const int number,
                 const dMatrix &GGX, const dMatrix &GGY, const dMatrix &GGZ, const char type)
{
  SetNumberID(number);
  SetGX(GGX);
  SetGY(GGY);
  SetGZ(GGZ);
  SetBlendType(type);

  initialised  = 0;
  xMin         = 0.0;
  xMax         = 0.0;
  yMin         = 0.0;
  yMax         = 0.0;
  zMin         = 0.0;
  zMax         = 0.0;
}

clPatch::clPatch(const clPatch &source)
{
  Assign(source);
}

clPatch::~clPatch(void)
{
  //cerr << "DESTROYING PATCH " << GetNumberID() << endl;
}


/*
** Methods to set and query member variables
*/

/*
** Query initialisation-flag
*/
int clPatch::GetInitialised(void) const
{
  return(initialised);
}

/*
** Set and Query the ID variables
*/
int clPatch::SetNumberID(const int number)
{
  numberID = number;

  return (numberID);
}

int clPatch::GetNumberID(void) const
{
  return (numberID);
}

/*
** Set and Query matrix GX: Geometrical Constraint Matrix in x-direction
*/
void clPatch::SetGX(const dMatrix &GGX)
{
  int rmax = GGX.GetNumberRows();
  int cmax = GGX.GetNumberColumns();

  if(rmax != 4 || cmax != 4)
  {
    throw clException("clPatch", "SetGX", "Wrong dimensions of matrix GGX.");
  }
  else
  {
    GX.Assign(GGX);
  }
}

void clPatch::SetGX(const dMatrix &X, const dMatrix &XU, const dMatrix &XV, const dMatrix &XUV)
{
  // Set dimensions of matrix G
  GX.SetNumberRows(4);
  GX.SetNumberColumns(4);

  // Check dimensions of matrices
  if((X.GetNumberRows()   == 2 && X.GetNumberColumns()   == 2) &&
     (XU.GetNumberRows()  == 2 && XU.GetNumberColumns()  == 2) &&
     (XV.GetNumberRows()  == 2 && XV.GetNumberColumns()  == 2) &&
     (XUV.GetNumberRows() == 2 && XUV.GetNumberColumns() == 2))
  {
    GX.SetElement(1, 1, X.GetElement(1, 1));
    GX.SetElement(1, 2, X.GetElement(1, 2));
    GX.SetElement(2, 1, X.GetElement(2, 1));
    GX.SetElement(2, 2, X.GetElement(2, 2));

    GX.SetElement(1, 3, XV.GetElement(1, 1));
    GX.SetElement(1, 4, XV.GetElement(1, 2));
    GX.SetElement(2, 3, XV.GetElement(2, 1));
    GX.SetElement(2, 4, XV.GetElement(2, 2));

    GX.SetElement(3, 1, XU.GetElement(1, 1));
    GX.SetElement(3, 2, XU.GetElement(1, 2));
    GX.SetElement(4, 1, XU.GetElement(2, 1));
    GX.SetElement(4, 2, XU.GetElement(2, 2));

    GX.SetElement(3, 3, XUV.GetElement(1, 1));
    GX.SetElement(3, 4, XUV.GetElement(1, 2));
    GX.SetElement(4, 3, XUV.GetElement(2, 1));
    GX.SetElement(4, 4, XUV.GetElement(2, 2));
  }
  else
  {
    throw clException("clPatch", "SetGX", "Wrong dimensions of matrices X, XU, XV or XUV.");
  }
}

dMatrix clPatch::GetGX(void) const
{
  return (GX);
}

/*
** Set and Query matrix GY: Geometrical Constraint Matrix in y-direction
*/
void clPatch::SetGY(const dMatrix &GGY)
{
  int rmax = GGY.GetNumberRows();
  int cmax = GGY.GetNumberColumns();

  if(rmax != 4 || cmax != 4)
  {
    throw clException("clPatch", "SetGY", "Wrong dimensions of matrix GGY.");
  }
  else
  {
    GY.Assign(GGY);
  }
}

void clPatch::SetGY(const dMatrix &Y, const dMatrix &YU, const dMatrix &YV, const dMatrix &YUV)
{
  // Set dimensions of matrix G
  GY.SetNumberRows(4);
  GY.SetNumberColumns(4);

  // Check dimensions of matrices
  if((Y.GetNumberRows()   == 2 && Y.GetNumberColumns()   == 2) &&
     (YU.GetNumberRows()  == 2 && YU.GetNumberColumns()  == 2) &&
     (YV.GetNumberRows()  == 2 && YV.GetNumberColumns()  == 2) &&
     (YUV.GetNumberRows() == 2 && YUV.GetNumberColumns() == 2))
  {
    GY.SetElement(1, 1, Y.GetElement(1, 1));
    GY.SetElement(1, 2, Y.GetElement(1, 2));
    GY.SetElement(2, 1, Y.GetElement(2, 1));
    GY.SetElement(2, 2, Y.GetElement(2, 2));

    GY.SetElement(1, 3, YV.GetElement(1, 1));
    GY.SetElement(1, 4, YV.GetElement(1, 2));
    GY.SetElement(2, 3, YV.GetElement(2, 1));
    GY.SetElement(2, 4, YV.GetElement(2, 2));

    GY.SetElement(3, 1, YU.GetElement(1, 1));
    GY.SetElement(3, 2, YU.GetElement(1, 2));
    GY.SetElement(4, 1, YU.GetElement(2, 1));
    GY.SetElement(4, 2, YU.GetElement(2, 2));

    GY.SetElement(3, 3, YUV.GetElement(1, 1));
    GY.SetElement(3, 4, YUV.GetElement(1, 2));
    GY.SetElement(4, 3, YUV.GetElement(2, 1));
    GY.SetElement(4, 4, YUV.GetElement(2, 2));
  }
  else
  {
    throw clException("clPatch", "SetGY", "Wrong dimensions of matrices Y, YU, YV or YUV.");
  }
}

dMatrix clPatch::GetGY(void) const
{
  return (GY);
}

/*
** Set and Query matrix GZ: Geometrical Constraint Matrix in z-direction.
*/
void clPatch::SetGZ(const dMatrix &GGZ)
{
  int rmax = GGZ.GetNumberRows();
  int cmax = GGZ.GetNumberColumns();

  if(rmax != 4 || cmax != 4)
  {
    throw clException("clPatch", "SetGZ", "Wrong dimensions of matrix GGZ.");
  }
  else
  {
    GZ.Assign(GGZ);
  }
}

void clPatch::SetGZ(const dMatrix &Z, const dMatrix &ZU, const dMatrix &ZV, const dMatrix &ZUV)
{
  // Set dimensions of matrix G
  GZ.SetNumberRows(4);
  GZ.SetNumberColumns(4);

  // Check dimensions of matrices
  if((Z.GetNumberRows()   == 2 && Z.GetNumberColumns()   == 2) &&
     (ZU.GetNumberRows()  == 2 && ZU.GetNumberColumns()  == 2) &&
     (ZV.GetNumberRows()  == 2 && ZV.GetNumberColumns()  == 2) &&
     (ZUV.GetNumberRows() == 2 && ZUV.GetNumberColumns() == 2))
  {
    GZ.SetElement(1, 1, Z.GetElement(1, 1));
    GZ.SetElement(1, 2, Z.GetElement(1, 2));
    GZ.SetElement(2, 1, Z.GetElement(2, 1));
    GZ.SetElement(2, 2, Z.GetElement(2, 2));

    GZ.SetElement(1, 3, ZV.GetElement(1, 1));
    GZ.SetElement(1, 4, ZV.GetElement(1, 2));
    GZ.SetElement(2, 3, ZV.GetElement(2, 1));
    GZ.SetElement(2, 4, ZV.GetElement(2, 2));

    GZ.SetElement(3, 1, ZU.GetElement(1, 1));
    GZ.SetElement(3, 2, ZU.GetElement(1, 2));
    GZ.SetElement(4, 1, ZU.GetElement(2, 1));
    GZ.SetElement(4, 2, ZU.GetElement(2, 2));

    GZ.SetElement(3, 3, ZUV.GetElement(1, 1));
    GZ.SetElement(3, 4, ZUV.GetElement(1, 2));
    GZ.SetElement(4, 3, ZUV.GetElement(2, 1));
    GZ.SetElement(4, 4, ZUV.GetElement(2, 2));
  }
  else
  {
    throw clException("clPatch", "SetGZ", "Wrong dimensions of matrices Z, ZU, ZV or ZUV.");
  }
}

dMatrix clPatch::GetGZ(void) const
{
  return (GZ);
}

/*
** Set and Query blending function type
*/
char clPatch::SetBlendType(const char type)
{
  // Check for valid blending type
  if (type == 'U')
  {
    blendType = type;
  }
  else if (type == 'H')
  {
    blendType = type;
  }
  else if (type == 'B')
  {
    blendType = type;
  }
  else
  {
    throw clException("clPatch", "SetBlendType", "Unknown type for blending functions.");
  }

  return (blendType);
}

char clPatch::GetBlendType(void) const
{
  return (blendType);
}


/*
** method to convert Hermite patches to Bezier patches
*/
void clPatch::HermiteToBezier(void)
{
  if (GetBlendType() != 'H')
  {
    throw clException("clPatch", "HermiteToBezier", "Patch type not Hermite.");
  }

  // Define the Conversion Matrix
  dMatrix MHB(4, 4);
  MHB.SetElement(1, 1, 1); MHB.SetElement(1, 2, 0); MHB.SetElement(1, 3, 0.00); MHB.SetElement(1, 4,  0.00);
  MHB.SetElement(2, 1, 1); MHB.SetElement(2, 2, 0); MHB.SetElement(2, 3, 0.33); MHB.SetElement(2, 4,  0.00);
  MHB.SetElement(3, 1, 0); MHB.SetElement(3, 2, 1); MHB.SetElement(3, 3, 0.00); MHB.SetElement(3, 4, -0.33);
  MHB.SetElement(4, 1, 0); MHB.SetElement(4, 2, 1); MHB.SetElement(4, 3, 0.00); MHB.SetElement(4, 4,  0.00);

  // Convert th Geometry Matrices
  GX.PreMultiply(MHB);
  GY.PreMultiply(MHB);
  GZ.PreMultiply(MHB);

  // Set blendType
  SetBlendType('B');

  // Force reinitialisation
  initialised = 0;
}

/*
** Method for calculating the normal vector to the patch
*/
dMatrix clPatch::GetNormal(const double u, const double v)
{
  // Check if already blended
  if(!initialised)
  {
    Initialise();
  }

  dMatrix FU(1, 4);
  FU.SetElement(1, 1, u*u*u);
  FU.SetElement(1, 2, u*u);
  FU.SetElement(1, 3, u);
  FU.SetElement(1, 4, 1);

  dMatrix dFU(1, 4);
  FU.SetElement(1, 1, 3*u*u);
  FU.SetElement(1, 2, 2*u);
  FU.SetElement(1, 3, 1);
  FU.SetElement(1, 4, 0);

  dMatrix FV(4, 1);
  FV.SetElement(1, 1, v*v*v);
  FV.SetElement(2, 1, v*v);
  FV.SetElement(3, 1, v);
  FV.SetElement(4, 1, 1);

  dMatrix dFV(4, 1);
  FV.SetElement(1, 1, 3*v*v);
  FV.SetElement(2, 1, 2*v);
  FV.SetElement(3, 1, 1);
  FV.SetElement(4, 1, 0);

  // Calculate tangent vector in u direction
  dMatrix x(QX), y(QY), z(QZ);
  x.PostMultiply(FV); x.PreMultiply(dFU);
  y.PostMultiply(FV); y.PreMultiply(dFU);
  z.PostMultiply(FV); z.PreMultiply(dFU);

  double a1 = x.GetElement(1, 1);
  double a2 = y.GetElement(1, 1);
  double a3 = z.GetElement(1, 1);

  // Calculate tangent vector in v direction
  x.PostMultiply(dFV); x.PreMultiply(FU);
  y.PostMultiply(dFV); y.PreMultiply(FU);
  z.PostMultiply(dFV); z.PreMultiply(FU);

  double b1 = x.GetElement(1, 1);
  double b2 = y.GetElement(1, 1);
  double b3 = z.GetElement(1, 1);

  // Fill the normal vector with the cross-product of 2 tangent vectors
  dMatrix normal(3, 1);
  normal.SetElement(1, 1, a2*b3 - a3*b2);
  normal.SetElement(1, 1, a3*b1 - a1*b3);
  normal.SetElement(1, 1, a1*b2 - a2*b1);

  return (normal);
}


/*
** Methods for querying the extremes
*/
double clPatch::GetMinX(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(xMin);
}

double clPatch::GetMaxX(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(xMax);
}

double clPatch::GetMinY(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(yMin);
}

double clPatch::GetMaxY(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(yMax);
}

double clPatch::GetMinZ(void)
{
  if(!initialised)
  {
    Initialise();
  }

  return(zMin);
}

double clPatch::GetMaxZ(void)
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
clPatch& clPatch::Assign(const clPatch &source)
{
  SetNumberID(source.GetNumberID());
  SetGX(source.GetGX());
  SetGY(source.GetGY());
  SetGZ(source.GetGZ());
  SetBlendType(source.GetBlendType());

  initialised = 0;

  return(*this);
}

clPatch& clPatch::operator=(const clPatch &source)
{
  return(Assign(source));
}

dMatrix clPatch::GetCoordinates(const double u, const double v)
{
  dMatrix uDummy(1, 1);
  dMatrix vDummy(1, 1);

  uDummy.SetElement(1, 1, u);
  vDummy.SetElement(1, 1, v);

  return(GetCoordinates(uDummy, vDummy));
}

dMatrix clPatch::operator()(const double u, const double v)
{
  return(GetCoordinates(u, v));
}

dMatrix clPatch::GetCoordinates(const dMatrix &u, const dMatrix &v)
{
  dMatrix coord;

  if((u.GetNumberColumns() != 1 || u.GetNumberRows() == 0) ||
     (v.GetNumberColumns() != 1 || v.GetNumberRows() != u.GetNumberRows()))
  {
    throw clException("clPatch", "GetCoordinates", "Invalid dimesions for matrices u and/or v.");
  }
  else
  {
    // Check if already blended
    if(!initialised)
    {
      Initialise();
    }

    coord.SetNumberRows(u.GetNumberRows());
    coord.SetNumberColumns(3);

    // Do for all specified points
    for(int i=1; i<=u.GetNumberRows(); i++)
    {
      double uu = u(i, 1);
      double vv = v(i, 1);

      // Truncate uu and vv upon PRECISION
      if(fabs(uu - 0.0f) < PRECISION)      uu = 0.0f;
      else if(fabs(uu - 1.0f) < PRECISION) uu = 1.0f;

      if(fabs(vv - 0.0f) < PRECISION)      vv = 0.0f;
      else if(fabs(vv - 1.0f) < PRECISION) vv = 1.0f;

      if(uu < 0.0f || uu > 1.0f || vv < 0.0f || vv > 1.0f)
      {
        cerr << uu-(double)(0) << " " << vv-(double)(1) << endl;
        char message[MESLEN];
        sprintf(message, "Invalid value for u and/or v.\n  U = %g\n  V = %g", uu, vv);
        throw clException("clPatch", "GetCoordinates", message);
      }
      else
      {
        dMatrix FU(1, 4);
        FU.SetElement(1, 1, uu*uu*uu);
        FU.SetElement(1, 2, uu*uu);
        FU.SetElement(1, 3, uu);
        FU.SetElement(1, 4, 1);

        dMatrix FV(4, 1);
        FV.SetElement(1, 1, vv*vv*vv);
        FV.SetElement(2, 1, vv*vv);
        FV.SetElement(3, 1, vv);
        FV.SetElement(4, 1, 1);

        // Calculate the co-ordinates
        dMatrix x(QX), y(QY), z(QZ);
        x.PostMultiply(FV); x.PreMultiply(FU);
        y.PostMultiply(FV); y.PreMultiply(FU);
        z.PostMultiply(FV); z.PreMultiply(FU);

        // Copy the co-ordinates into the column vector coord
        coord.SetElement(i, 1, x(1, 1));
        coord.SetElement(i, 2, y(1, 1));
        coord.SetElement(i, 3, z(1, 1));
      }
    }
  }

  return (coord);
}

dMatrix clPatch::operator()(const dMatrix &u, const dMatrix &v)
{
  return(GetCoordinates(u, v));
}

