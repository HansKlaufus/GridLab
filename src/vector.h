/*******************************************************************
*** Header file for declaration and definition of template tpVector
***
*** J.L. Klaufus - Haarlem 2010
**/


#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#include "globals.h"
#include "exceptiontree.h"
#include "matrix.h"

template <class VTYPE>
class tpVector : public tpMatrix<VTYPE>
{
private:

public:
  tpVector(void);
  tpVector(const int);
  tpVector(const tpVector<VTYPE>&);
  ~tpVector(void);

  int                SetNumberColumns(const int);
  VTYPE              SetElement(const int, const VTYPE);
  VTYPE              GetElement(const int) const;

  double             Length(void);
  void               Normalise(void);
  tpVector<VTYPE>&   CrossProduct(const tpVector<VTYPE>&);

//  int                operator==(const tpVector<VTYPE>&) const;
//  int                operator!=(const tpVector<VTYPE>&) const;
//  VTYPE              operator()(const int, const int) const;
//  tpVector<VTYPE>&   operator=(const tpVector<VTYPE>&);
//  tpVector<VTYPE>&   operator+(const tpVector<VTYPE>&);
//  tpVector<VTYPE>&   operator-(const tpVector<VTYPE>&);
//  tpVector<VTYPE>&   operator*(const VTYPE);
//  tpVector<VTYPE>&   operator*(const tpVector<VTYPE>&);
//  tpVector<VTYPE>&   operator/(const VTYPE);
//  tpVector<VTYPE>&   operator/(const tpVector<VTYPE>&);
};

//ostream&  operator<<(ostream&, const tpVector<dComplex>&);
//ostream&  operator<<(ostream&, const tpVector<double>&);
//ostream&  operator<<(ostream&, const tpVector<float>&);
//ostream&  operator<<(ostream&, const tpVector<int>&);


/*
** Templates have to be defined in the same files as
** they are declared and instantiated.
** Therefore everything in one headerfile.
*/


/*
** Constructors and Destructor
*/

template <class VTYPE>
tpVector<VTYPE>::tpVector(void) : tpMatrix<VTYPE>(3, 1)
{
}

template <class VTYPE>
tpVector<VTYPE>::tpVector(const int r) : tpMatrix<VTYPE>(r, 1)
{
}

template <class VTYPE>
tpVector<VTYPE>::tpVector(const tpVector<VTYPE> &source)
{
  Assign(source);
}

template <class VTYPE>
tpVector<VTYPE>::~tpVector(void)
{
}


template <class VTYPE>
int tpVector<VTYPE>::SetNumberColumns(const int c)
{
  throw clExceptionTree("tpVector", "SetNumberColumns", "Vector is Matrix with one column; can not set number columns");
}

template <class VTYPE>
VTYPE tpVector<VTYPE>::SetElement(const int r, const VTYPE v)
{
  return(tpMatrix<VTYPE>::SetElement(r, 1, v));
}


template <class VTYPE>
VTYPE tpVector<VTYPE>::GetElement(const int r) const
{
  return(tpMatrix<VTYPE>::GetElement(r, 1));
}

template <class VTYPE>
double tpVector<VTYPE>::Length(void)
{
  double length = 0.0f;

  int rmax = this->GetNumberRows();
  for(int r=0; r<rmax; r++)
  {
    length += this->GetElement(r+1)*this->GetElement(r+1);
  }

  return (sqrt(length));
}

template <class VTYPE>
void tpVector<VTYPE>::Normalise(void)
{
  double length = this->Length();

  int rmax = this->GetNumberRows();
  for(int r=0; r<rmax; r++)
  {
    this->SetElement(r+1, 1, this->GetElement(r+1, 1)/length);
  }
}

template <class VTYPE>
tpVector<VTYPE>& tpVector<VTYPE>::CrossProduct(const tpVector<VTYPE> &v)
{
  // v is the first matrix, *THIS the second
  int nr1 = v.GetNumberRows();
  int nr2 = this->GetNumberRows();

  if (nr1 != 3)
  {
    throw clExceptionTree("tpVector", "CrossProduct", "CrossProduct only defined in 3D.");
  }
  else if (nr1 == nr2)
  {
    tpVector<VTYPE> temp(nr1);

    temp.SetElement(1,  v.GetElement(2)*this->GetElement(3) - v.GetElement(3)*this->GetElement(2));
    temp.SetElement(2, -v.GetElement(1)*this->GetElement(3) + v.GetElement(3)*this->GetElement(1));
    temp.SetElement(3,  v.GetElement(1)*this->GetElement(2) - v.GetElement(2)*this->GetElement(1));

    Assign(temp);
  }
  else
  {
    throw clExceptionTree("Vector", "CrossProduct", "Dimensions of vectors do not match.");
  }

  return(*this);
}

/*
** Redefinition of operators
*/




#endif

