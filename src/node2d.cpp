/****************************************************
*** Body file for definition of class clNode2D
***
*** J.L. Klaufus - Haarlem 2009
**/

#include "exceptiontree.h"
#include "node2d.h"

clNode2D::clNode2D(void) : clNode()
{
  try
  {
    SetObjectType(otNode2D);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode2D::clNode2D");
    throw EX;
  }
}

clNode2D::clNode2D(const int ID, const double x, const double y)
{
  try
  {
    SetObjectType(otNode2D);
    SetNodeID(ID);
    SetCoordinates(x, y);
    ResetHaloNode();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode2D::clNode2D");
    throw EX;
  }
}

clNode2D::clNode2D(const int ID, const dMatrix &m) : clNode(ID, m)
{
  try
  {
    SetObjectType(otNode2D);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode2D::clNode2D");
    throw EX;
  }
}

clNode2D::clNode2D(const clNode2D &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode2D::clNode2D");
    throw EX;
  }
}

clNode2D::~clNode2D(void)
{
  //cerr << "DESTROYING NODE: " << GetNodeID() << endl;
}

/*
** Query methods
*/

dMatrix clNode2D::SetCoordinates(const double x, const double y)
{
  try
  {
    dMatrix temp(2,1);

    temp.SetElement(1,1,x);
    temp.SetElement(2,1,y);

    SetCoordinates(temp);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode2D::SetCoordinates");
    throw EX;
  }

  return(coord);
}

double clNode2D::x(void) const
{
  return(coord(1,1));
}

double clNode2D::y(void) const
{
  return(coord(2,1));
}

/*
** Methods for operations
*/

clNode2D& clNode2D::Assign(const clNode2D &source)
{
  try
  {
    SetNodeID(source.GetNodeID());
    coord = source.GetCoordinates();

    if (source.IsHaloNode())
    {
      SetHaloNode();
    }
    else
    {
      ResetHaloNode();
    }

    if (source.IsBoundaryNode())
    {
      SetBoundaryNode();
    }
    else
    {
      ResetBoundaryNode();
    }

    while(GetNumberElements()>0)
    {
      DeleteElementRefByIndex(0);
    }

    for(int e=0; e<source.GetNumberElements(); e++)
    {
      AddElementID(source.GetElementID(e));
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode2D::Assign");
    throw EX;
  }

  return(*this);
}

clNode2D& clNode2D::operator=(const clNode2D &source)
{
  return(Assign(source));
}

