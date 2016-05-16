/*******************************************************************
*** Header file for declaration and definition of template MATRIX
***
*** J.L. Klaufus - Cranfield 1998
**/


#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <math.h>

#include "globals.h"
#include "exceptiontree.h"

template <class MTYPE>
class tpMatrix
{
protected:
  int       numberRows, numberColumns;
  MTYPE     *elements;

  void      ResetElements(void);

public:
  tpMatrix(void);
  tpMatrix(const int, const int);
  tpMatrix(const tpMatrix<MTYPE>&);
  ~tpMatrix(void);

  int                SetNumberRows(const int);
  int                SetNumberColumns(const int);
  MTYPE              SetElement(const int, const int, const MTYPE);
  int                GetNumberRows(void) const;
  int                GetNumberColumns(void) const;
  MTYPE              GetElement(const int, const int) const;

  tpMatrix<MTYPE>&   Zero(void);
  tpMatrix<MTYPE>&   One(void);
  tpMatrix<MTYPE>&   Identity(void);

  MTYPE              LU_Decomp(tpMatrix<int>&);
  tpMatrix<MTYPE>&   LU_Subst(tpMatrix<MTYPE>&, tpMatrix<int>&);
  tpMatrix<MTYPE>&   Solve(tpMatrix<MTYPE>&);
  int                IsSingular(void);

  int                IsTri(void);
  tpMatrix<MTYPE>&   MakeTri(void);
  tpMatrix<MTYPE>&   SolveTri(tpMatrix<MTYPE>&);

  tpMatrix<MTYPE>&   SubMatrix(const int, const int);
  tpMatrix<MTYPE>&   MirrorRows(void);
  tpMatrix<MTYPE>&   MirrorColumns(void);
  tpMatrix<MTYPE>&   Transpose(void);
  tpMatrix<MTYPE>&   Inverse(void);

  int                Compare(const tpMatrix<MTYPE>&) const;
  tpMatrix<MTYPE>&   Assign(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   Add(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   Subtract(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   Multiply(const MTYPE);
  tpMatrix<MTYPE>&   Divide(const MTYPE);
  tpMatrix<MTYPE>&   PreMultiply(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   PostMultiply(const tpMatrix<MTYPE>&);

  MTYPE              Minor(const int, const int);
  MTYPE              Cofactor(const int, const int);
  MTYPE              Determinant(void);
  tpMatrix<MTYPE>&   Adjoint(void);
  int                Rank(void);

  double             Length(void);
  tpMatrix<MTYPE>&   Normalise(void);

  int                operator==(const tpMatrix<MTYPE>&) const;
  int                operator!=(const tpMatrix<MTYPE>&) const;
  MTYPE              operator()(const int, const int) const;
  tpMatrix<MTYPE>&   operator=(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   operator+(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   operator-(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   operator*(const MTYPE);
  tpMatrix<MTYPE>&   operator*(const tpMatrix<MTYPE>&);
  tpMatrix<MTYPE>&   operator/(const MTYPE);
  tpMatrix<MTYPE>&   operator/(const tpMatrix<MTYPE>&);
};

//ostream&  operator<<(ostream&, const tpMatrix<dComplex>&);
//ostream&  operator<<(ostream&, const tpMatrix<double>&);
//ostream&  operator<<(ostream&, const tpMatrix<float>&);
//ostream&  operator<<(ostream&, const tpMatrix<int>&);


/*
** Templates have to be defined in the same files as
** they are declared and instantiated.
** Therefore everything in one headerfile.
*/


/*
** Constructors and Destructor
*/

template <class MTYPE>
tpMatrix<MTYPE>::tpMatrix(void)
{
  elements      = NULL;
  numberRows    = 3;
  numberColumns = 3;

  ResetElements();
  Zero();
}

template <class MTYPE>
tpMatrix<MTYPE>::tpMatrix(const int r, const int c=3)
{
  elements      = NULL;
  numberRows    = r;
  numberColumns = c;

  ResetElements();
  Zero();
}

template <class MTYPE>
tpMatrix<MTYPE>::tpMatrix(const tpMatrix<MTYPE> &source)
{
  int r = source.GetNumberRows();
  int c = source.GetNumberColumns();

  elements      = NULL;
  numberRows    = r;
  numberColumns = c;

  ResetElements();

  for (int rr=1; rr<=r; rr++)
  {
    for (int cc=1; cc<=c; cc++)
    {
      SetElement(rr, cc, source.GetElement(rr, cc));
    }
  }
}

template <class MTYPE>
tpMatrix<MTYPE>::~tpMatrix(void)
{
  if (elements)
  {
    delete [] elements;
  }
}


/*
** Initialisation methods (Private)
*/

template <class MTYPE>
void tpMatrix<MTYPE>::ResetElements(void)
{
  if (elements != NULL)
  {
    delete [] elements;
  }

  elements = new MTYPE[numberRows*numberColumns];
}


/*
** Methods to set membervariables
*/

template <class MTYPE>
int tpMatrix<MTYPE>::SetNumberRows(const int r)
{
  numberRows = r;
  ResetElements();
  return(numberRows);
}

template <class MTYPE>
int tpMatrix<MTYPE>::SetNumberColumns(const int c)
{
  numberColumns = c;
  ResetElements();
  return(numberColumns);
}

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::SetElement(const int r, const int c, const MTYPE v)
{
  if((r>0 && r<numberRows+1) && (c>0 && c<numberColumns+1))
  {
    elements[(r-1)*numberColumns+(c-1)] = v;
  }
  else
  {
    throw clExceptionTree("tpMatrix", "SetElement", "Invalid row or column index.");
  }

  return(elements[(r-1)*numberColumns+(c-1)]);
}


/*
** Methods to query membervariables
*/

template <class MTYPE>
int tpMatrix<MTYPE>::GetNumberRows(void) const
{
  return(numberRows);
}

template <class MTYPE>
int tpMatrix<MTYPE>::GetNumberColumns(void) const
{
  return(numberColumns);
}

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::GetElement(const int r, const int c) const
{
  MTYPE v;

  if((r>0 && r<numberRows+1) && (c>0 && c<numberColumns+1))
  {
    v = elements[(r-1)*numberColumns+(c-1)];
  }
  else
  {
    throw clExceptionTree("tpMatrix", "GetElement", "Invalid row or column index.");
  }

  return(v);
}


/*
** Methods for defining standard matrices
*/

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Zero(void)
{
  for (int r=1; r<=GetNumberRows(); r++)
  {
    for (int c=1; c<=GetNumberColumns(); c++)
    {
      SetElement(r, c, MTYPE(0));
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::One(void)
{
  for (int r=1; r<=GetNumberRows(); r++)
  {
    for (int c=1; c<=GetNumberColumns(); c++)
    {
      SetElement(r, c, MTYPE(1));
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Identity(void)
{
  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  if (rmax == cmax)
  {
    Zero();

    for (int r=1; r<=rmax; r++)
    {
      SetElement(r, r, MTYPE(1));
    }
  }

  return(*this);
}


/*
** Methods for LU-Decomposition
*/

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::LU_Decomp(tpMatrix<int> &index)
{
  /*
  ** A * x = b
  **
  ** Define: A = L * U
  **
  ** L = lower triangular matrix
  ** U = upper triangular matrix
  */

  int   N = GetNumberRows();
  int   iMax;
  MTYPE evenOrOdd;
  MTYPE tempValue;
  MTYPE largeValue;
  MTYPE sum;
  MTYPE *scale = new MTYPE[N+1];

  evenOrOdd=1;
  for(int i=1; i<=N; i++)
  {
    largeValue=(MTYPE)(0);

    for(int j=1; j<=N; j++)
    {
      tempValue=fabs(GetElement(i,j));

      if(tempValue>largeValue)
      {
        largeValue=tempValue;
      }
    }

    if(largeValue==(MTYPE)(0))
    {
      throw clExceptionTree("tpMatrix", "LU_Decomp", "Matrix is singular.");
    }

    scale[i]=(MTYPE)(1)/largeValue;
  }

  for(int j=1; j<=N; j++)
  {
    for(int i=1; i<j; i++)
    {
      sum=GetElement(i,j);

      for(int k=1; k<i; k++)
      {
        sum-=GetElement(i,k)*GetElement(k,j);
      }

      SetElement(i,j,sum);
    }

    largeValue=(MTYPE)(0);

    for(int ii=j; ii<=N; ii++)
    {
      sum=GetElement(ii,j);
      for(int k=1; k<j; k++)
      {
        sum-=GetElement(ii,k)*GetElement(k,j);
      }

      SetElement(ii,j,sum);

      tempValue=scale[ii]*fabs(sum);
      if(tempValue>=largeValue)
      {
        largeValue=tempValue;
        iMax=ii;
      }
    }

    if(j!=iMax)
    {
      for(int k=1; k<=N; k++)
      {
        tempValue=GetElement(iMax,k);
        SetElement(iMax,k,GetElement(j,k));
        SetElement(j,k,tempValue);
      }

      evenOrOdd=-evenOrOdd;
      scale[iMax]=scale[j];
    }

    index.SetElement(j,1,iMax);

    if(GetElement(j,j)==(MTYPE)(0))
    {
      SetElement(j,j,PRECISION);
    }

    if(j!=N)
    {
      tempValue=(MTYPE)(1)/GetElement(j,j);

      for(int i=j+1; i<=N; i++)
      {
        SetElement(i,j,tempValue*GetElement(i,j));
      }
    }
  }

  delete [] scale;

  return(evenOrOdd);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::LU_Subst(tpMatrix<MTYPE> &b, tpMatrix<int> &index)
{
  /*
  ** A * x = b
  **
  ** Define: A = L * U
  **
  ** L = lower triangular matrix
  ** U = upper triangular matrix
  */

  int           N = GetNumberRows();
  tpMatrix<MTYPE> y(N, 1);

  int ii=0;
  for(int i=1; i<=N; i++)
  {
    int   ip  = index.GetElement(i,1);
    MTYPE sum = b.GetElement(ip,1);
    b.SetElement(ip,1,b.GetElement(i,1));

    if(ii)
    {
      for(int j=ii; j<=i-1; j++)
      {
        sum-=GetElement(i,j)*b.GetElement(j,1);
      }
    }
    else if(sum)
    {
      ii=i;
    }

    b.SetElement(i,1,sum);
  }

  for(int k=N; k>=1; k--)
  {
    MTYPE sum=b.GetElement(k,1);

    for(int j=k+1; j<=N; j++)
    {
      sum-=GetElement(k,j)*b.GetElement(j,1);
    }

    b.SetElement(k,1,sum/GetElement(k,k));
  }

  return(b);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Solve(tpMatrix<MTYPE> &b)
{
  try
  {
    Inverse()*b;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpMatrix::Solve");
    throw EX;
  }

  return(*this);
}

template <class MTYPE>
int tpMatrix<MTYPE>::IsSingular(void)
{
  int ret=(0==-1);

  try
  {
    tpMatrix<MTYPE> M(*this);
    tpMatrix<int>   index(M.GetNumberRows(),1);

    M.LU_Decomp(index);
  }

  catch(clExceptionTree EX)
  {
    ret=(0==0);
  }

  return(ret);
}


/*
** Methods for tridiagonal matrices
*/

template <class MTYPE>
int tpMatrix<MTYPE>::IsTri(void)
{
  int  ret  = 1;
  int  rmax = GetNumberRows();
  int  cmax = GetNumberColumns();

  if (rmax != cmax)
  {
    ret = 0;
  }
  else
  {
    for(int r=1; r<=rmax; r++)
    {
      for(int c=1; c<=cmax; c++)
      {
        MTYPE value = GetElement(r, c);

        if((c == r-1 || c == r || c == r+1) && (value < PRECISION))
        {
          // Values on the diagonal must not be zero
          ret = 0;
        }
        else if(c != r-1 && c != r && c != r+1 && value > PRECISION)
        {
          // Values on the diagonal must be zero
          ret = 0;
        }
      }
    }
  }

  return (ret);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::MakeTri(void)
{
  // Check if matrix is tridiagonal
  if(!IsTri())
  {
    throw clExceptionTree("tpMatrix", "MakeTri", "Matrix is not tridiagonal");
  }
  else
  {
    // Copy original matrix
    tpMatrix<MTYPE> temp(*this);
    int rmax = GetNumberRows();

    // Redimension and refill original matrix
    SetNumberColumns(3);
    for(int r=1; r<=rmax; r++)
    {
      if(r==1)
      {
        SetElement(r, 1, (MTYPE) (0));
      }
      else
      {
        SetElement(r, 1, temp.GetElement(r, r-1));
      }

      SetElement(r, 2, temp.GetElement(r, r));

      if(r==rmax)
      {
        SetElement(r, 3, (MTYPE) (0));
      }
      else
      {
        SetElement(r, 3, temp.GetElement(r, r+1));
      }
    }
  }

  return (*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::SolveTri(tpMatrix<MTYPE> &vector)
{
  // Set up dummy matrix
  tpMatrix<MTYPE> dummy(*this);

  // Check for dimensions
  int rmax = dummy.GetNumberRows();
  int cmax = dummy.GetNumberColumns();

  int rvec = vector.GetNumberRows();
  int cvec = vector.GetNumberColumns();

  if (cmax != 3)
  {
    throw clExceptionTree("tpMatrix", "SolveTri", "Coefficient matrix not tridiagonal");
  }

  if (cvec != 1)
  {
    throw clExceptionTree("tpMatrix", "SolveTri", "Solution vector not in column format");
  }

  if (rmax != rvec)
  {
    throw clExceptionTree("tpMatrix", "SolveTri", "Dimensions do not match");
  }

  // Establish upper triangular matrix
  for(int r=2; r<=rmax; r++)
  {
    if(dummy.GetElement(r-1, 2)<PRECISION)
    {
      dummy.SetElement(r-1, 2, PRECISION);
      //throw clExceptionTree("tpMatrix", "SolveTri", "Division by zero (1).");
    }

    MTYPE rr = dummy.GetElement(r, 1)/dummy.GetElement(r-1, 2);
    dummy.SetElement(r, 2, dummy.GetElement(r, 2) - rr*dummy.GetElement(r-1, 3));
    vector.SetElement(r, 1, dummy.GetElement(r, 1) - rr*vector.GetElement(r-1, 1));
  }

  // Back substitution
  if(dummy.GetElement(rmax, 2)<PRECISION)
  {
    dummy.SetElement(rmax, 2, PRECISION);
    //throw clExceptionTree("tpMatrix", "SolveTri", "Division by zero (2).");
  }

  vector.SetElement(rmax, 1, vector.GetElement(rmax, 1)/dummy.GetElement(rmax, 2));
  {
    for(int r=rmax-1; r>=1; r--)
    {
      if(dummy.GetElement(r, 2)<PRECISION)
      {
        dummy.SetElement(r, 2, PRECISION);
        //throw clExceptionTree("tpMatrix", "SolveTri", "Division by zero (3).");
      }

      vector.SetElement(r, 1, (vector.GetElement(r, 1) - dummy.GetElement(r, 3)*vector.GetElement(r+1, 1))/dummy.GetElement(r, 2));
    }
  }

  return (vector);
}


/*
** Methods for matrix transformations
*/

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::SubMatrix(const int r, const int c)
{
  int rtemp, ctemp;

  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  // Extract TEMP from *THIS
  tpMatrix<MTYPE> temp(rmax-1, cmax-1);

  rtemp = 0;
  for (int rorg=1; rorg<=rmax; rorg++)
  {
    if (rorg != r)
    {
      rtemp++;
      ctemp = 0;

      for (int corg=1; corg<=cmax; corg++)
      {
        if (corg != c)
        {
          ctemp++;
          temp.SetElement(rtemp, ctemp, GetElement(rorg, corg));
        }
      }
    }
  }

  //Copy TEMP into *THIS
  Assign(temp);

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::MirrorRows(void)
{
  int           rmax = GetNumberRows();
  int           cmax = GetNumberColumns();
  tpMatrix<MTYPE> temp(*this);

  for(int r=1; r<=rmax; r++)
  {
    for(int c=1; c<=cmax; c++)
    {
      SetElement(rmax+1-r, c, temp.GetElement(r, c));
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::MirrorColumns(void)
{
  int           rmax = GetNumberRows();
  int           cmax = GetNumberColumns();
  tpMatrix<MTYPE> temp(*this);

  for(int c=1; c<=cmax; c++)
  {
    for(int r=1; r<=rmax; r++)
    {
      SetElement(r, cmax+1-c, temp.GetElement(r, c));
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Transpose(void)
{
  int           r, c;
  tpMatrix<MTYPE> temp(*this);

  // At this stage TEMP is equal to THIS

  // Switch ROWS and COLUMNS
  r = temp.GetNumberColumns();
  c = temp.GetNumberRows();

  // Redefine matrix (this)
  SetNumberRows(r);
  SetNumberColumns(c);

  for (int rr=1; rr<=r; rr++)
  {
    for (int cc=1; cc<=c; cc++)
    {
      SetElement(rr, cc, temp.GetElement(cc, rr));
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Inverse(void)
{
  try
  {
    int             N = GetNumberRows();
    tpMatrix<int>   index(N+1,1);
    tpMatrix<MTYPE> b(N, 1);
    tpMatrix<MTYPE> M(*this);

    LU_Decomp(index);

    for (int j=1; j<=N; j++)
    {
      for (int i=1; i<=N; i++)
      {
        b.SetElement(i, 1, MTYPE(0));
      }
      b.SetElement(j, 1, MTYPE(1));

      LU_Subst(b, index);

      for (int ii=1; ii<=N; ii++)
      {
        M.SetElement(ii, j, b.GetElement(ii, 1));
      }
    }

    Assign(M);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpMatrix::Inverse");
    throw EX;
  }

  return(*this);
}


/*
** Methods for matrix operations
*/

template <class MTYPE>
int tpMatrix<MTYPE>::Compare(const tpMatrix<MTYPE> &M) const
{
  int ret = 1; // Return value: 0 not equal; 1 equal
  int rmax = M.GetNumberRows();
  int cmax = M.GetNumberColumns();

  if ((GetNumberRows() == rmax) && (GetNumberColumns() == cmax))
  {
    for (int r=1; r<=rmax; r++)
    {
      for (int c=1; c<=cmax; c++)
      {
        if (fabs(GetElement(r, c)-M.GetElement(r, c)) > PRECISION)
        {
          ret = 0;
        }
      }
    }
  }
  else
  {
    ret = 0;
  }

  return(ret);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Assign(const tpMatrix<MTYPE> &m)
{
  int rmax = m.GetNumberRows();
  int cmax = m.GetNumberColumns();

  numberRows    = rmax;
  numberColumns = cmax;
  ResetElements();

  for (int r=1; r<=rmax; r++)
  {
    for (int c=1; c<=cmax; c++)
    {
      SetElement(r, c, m.GetElement(r, c));
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Add(const tpMatrix<MTYPE> &m)
{
  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  if ((m.GetNumberRows() == rmax) && (m.GetNumberColumns() == cmax))
  {
    for (int r=1; r<=rmax; r++)
    {
      for (int c=1; c<=cmax; c++)
      {
        SetElement(r, c, GetElement(r, c) + m.GetElement(r, c));
      }
    }
  }
  else
  {
    throw clExceptionTree("tpMatrix", "Add", "Dimensions of matrices do not match.");
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Subtract(const tpMatrix<MTYPE> &m)
{
  tpMatrix<MTYPE> temp(m);

  Add(temp.Multiply(MTYPE(-1.0)));

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Multiply(const MTYPE v)
{
  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  for (int r=1; r<=rmax; r++)
  {
    for (int c=1; c<=cmax; c++)
    {
      SetElement(r, c, GetElement(r, c) * v);
    }
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Divide(const MTYPE v)
{
  if (v != MTYPE(0))
  {
    Multiply(MTYPE(1)/v);
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::PreMultiply(const tpMatrix<MTYPE> &m)
{
  // M is the first matrix, *THIS the second
  int nr1 = m.GetNumberRows();
  int nc1 = m.GetNumberColumns();

  int nr2 = GetNumberRows();
  int nc2 = GetNumberColumns();

  if (nc1 == nr2)
  {
    tpMatrix<MTYPE> temp(nr1, nc2);

    for (int c2=1; c2<=nc2; c2++)
    {
      for (int r1=1; r1<=nr1; r1++)
      {
        MTYPE  res(0);

        for (int c1=1; c1<=nc1; c1++)
        {
          res = res + m.GetElement(r1, c1) * GetElement(c1, c2); //Remember c1 = r2
        }

        temp.SetElement(r1, c2, res);
      }
    }

    Assign(temp);
  }
  else
  {
    throw clExceptionTree("tpMatrix", "PreMultiply", "Dimensions of matrices do not match.");
  }

  return(*this);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::PostMultiply(const tpMatrix<MTYPE> &m)
{
  // *THIS is the first matrix, M the second
  int nr1 = GetNumberRows();
  int nc1 = GetNumberColumns();

  int nr2 = m.GetNumberRows();
  int nc2 = m.GetNumberColumns();

  if (nc1 == nr2)
  {
    tpMatrix<MTYPE> temp(nr1, nc2);

    for (int c2=1; c2<=nc2; c2++)
    {
      for (int r1=1; r1<=nr1; r1++)
      {
        MTYPE res(0);

        for (int c1=1; c1<=nc1; c1++)
        {
          res = res + GetElement(r1, c1) * m.GetElement(c1, c2); //Remember c1 = r2
        }

        temp.SetElement(r1, c2, res);
      }
    }

    Assign(temp);
  }
  else
  {
    throw clExceptionTree("tpMatrix", "PostMultiply", "Dimensions of matrices do not match.");
  }

  return(*this);
}

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::Minor(const int r, const int c)
{
  MTYPE ret(0);

  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  if (rmax == cmax && rmax > 1)
  {
    tpMatrix<MTYPE> temp(*this);

    temp.SubMatrix(r, c);
    ret = temp.Determinant();
  }

  return(ret);
}

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::Cofactor(const int r, const int c)
{
  int   fact = -1;
  MTYPE ret(0);

  for (int i=2; i<=r+c; i++)
  {
    fact = -fact;
  }

  ret = Minor(r, c) * fact;

  return(ret);
}

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::Determinant(void)
{
  MTYPE ret(1);
  int   rmax=GetNumberRows();
  int   cmax=GetNumberColumns();

  if (rmax == cmax)
  {
    tpMatrix<int>   index(rmax,1);
    tpMatrix<MTYPE> M(*this);

    ret=M.LU_Decomp(index);

    for (int i=1; i<=rmax; i++)
    {
      ret *= M.GetElement(i, i);
    }
  }

  return(ret);
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::Adjoint(void)
{
  tpMatrix<MTYPE> temp(*this);

  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  if (rmax == cmax && rmax > 1)
  {
    for (int r=1; r<=rmax; r++)
    {
      for (int c=1; c<=cmax; c++)
      {
        SetElement(r, c, temp.Cofactor(r, c));
      }
    }

    Transpose();
  }

  return(*this);
}

template <class MTYPE>
int tpMatrix<MTYPE>::Rank(void)
{
  int ret  = 0;
  int rmax = GetNumberRows();
  int cmax = GetNumberColumns();

  if (rmax == cmax)
  {
    if (rmax > 1)
    {
      if (this->Determinant() == MTYPE(0))
      {
        tpMatrix<MTYPE> temp(*this);

        temp.SubMatrix(1, 1);
        ret = temp.Rank();
      }
      else
      {
        ret = rmax;
      }
    }
    else
    {
      ret = 1;
    }
  }

  return(ret);
}

/*
** Redefinition of operators
*/

template <class MTYPE>
int tpMatrix<MTYPE>::operator==(const tpMatrix<MTYPE> &m) const
{
  return(Compare(m));
}

template <class MTYPE>
int tpMatrix<MTYPE>::operator!=(const tpMatrix<MTYPE> &m) const
{
  return(!Compare(m));
}

template <class MTYPE>
MTYPE tpMatrix<MTYPE>::operator()(const int r, const int c) const
{
  return(GetElement(r,c));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator=(const tpMatrix<MTYPE> &m)
{
  return(Assign(m));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator+(const tpMatrix<MTYPE> &m)
{
  return(Add(m));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator-(const tpMatrix<MTYPE> &m)
{
  return(Subtract(m));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator*(const MTYPE v)
{
  return(Multiply(v));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator*(const tpMatrix<MTYPE> &m)
{
  return(PostMultiply(m));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator/(const MTYPE v)
{
  return(Divide(v));
}

template <class MTYPE>
tpMatrix<MTYPE>& tpMatrix<MTYPE>::operator/(const tpMatrix<MTYPE> &m)
{
  tpMatrix<MTYPE> temp(m);

  PostMultiply(temp.Inverse());
  return(*this);
}


/*
** Overloading of the output operator (NOT memberfunctiontions of class MATRIX)
*/

/*
ostream& operator<<(ostream& os, const tpMatrix<Complex> &m)
{
  os << "\n";
  for (int r=1; r<=m.GetNumberRows(); r++)
  {
    os << "\n| ";
    for (int c=1; c<=m.GetNumberColumns(); c++)
    {
      os << m(r, c) << "   ";
    }
    os << " |";
  }
  os << "\n";

  return(os);
}

ostream& operator<<(ostream& os, const tpMatrix<double> &m)
{
  double v;

  os << "\n";
  for (int r=1; r<=m.GetNumberRows(); r++)
  {
    os << "\n| ";
    for (int c=1; c<=m.GetNumberColumns(); c++)
    {
      // Set Precision
      if (m(r, c) - int(m(r, c)) < PRECISION)
        v = double( int( m(r, c) ) );
      else if (double( int( m(r, c) ) + 1 ) - m(r, c) < PRECISION)
        v = double( int( m(r, c) + 1 ) );
      else
        v = m(r, c);

      os << v << "   ";
    }
    os << " |";
  }
  os << "\n";

  return(os);
}

ostream& operator<<(ostream& os, const tpMatrix<float> &m)
{
  float v;

  os << "\n";
  for (int r=1; r<=m.GetNumberRows(); r++)
  {
    os << "\n| ";
    for (int c=1; c<=m.GetNumberColumns(); c++)
    {
      // Set Precision
      if (m(r, c) - int(m(r, c)) < PRECISION)
        v = float( int( m(r, c) ) );
      else if (float( int( m(r, c) ) + 1 ) - m(r, c) < PRECISION)
        v = float( int( m(r, c) + 1 ) );
      else
        v = m(r, c);

      os << v << "   ";
    }
    os << " |";
  }
  os << "\n";

  return(os);
}

ostream& operator<<(ostream& os, const tpMatrix<int> &m)
{
  os << "\n";
  for (int r=1; r<=m.GetNumberRows(); r++)
  {
    os << "\n| ";
    for (int c=1; c<=m.GetNumberColumns(); c++)
    {
      os << m(r, c) << "   ";
    }
    os << " |";
  }
  os << "\n";

  return(os);
}
*/

#endif

