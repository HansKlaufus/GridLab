/****************************************************
*** Body file for definition of class clNode3D
***
*** J.L. Klaufus - Cranfield 1999
**/

#include "globals.h"
#include "exceptiontree.h"
#include "node3d.h"

clNode3D::clNode3D(void) : clNode()
{
  try
  {
    SetObjectType(otNode3D);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode3D::clNode3D");
    throw EX;
  }
}

clNode3D::clNode3D(const int ID, const dMatrix &m) : clNode(ID, m)
{
  try
  {
    SetObjectType(otNode3D);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::clNode");
    throw EX;
  }
}

clNode3D::clNode3D(const int ID, const double x, const double y, const double z)
{
  try
  {
    SetObjectType(otNode3D);
    SetNodeID(ID);
    SetCoordinates(x, y, z);
    ResetHaloNode();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode3D::clNode3D");
    throw EX;
  }
}

clNode3D::clNode3D(const clNode3D &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode3D::clNode3D");
    throw EX;
  }
}

clNode3D::~clNode3D(void)
{
  //cerr << "DESTROYING NODE: " << GetNodeID() << endl;
}

/*
** Query methods
*/

dMatrix clNode3D::SetCoordinates(const double x, const double y, const double z)
{
  try
  {
    dMatrix temp(3,1);

    temp.SetElement(1,1,x);
    temp.SetElement(2,1,y);
    temp.SetElement(3,1,z);

    SetCoordinates(temp);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode3D::SetCoordinates");
    throw EX;
  }

  return(coord);
}

double clNode3D::x(void) const
{
  return(coord(1,1));
}

double clNode3D::y(void) const
{
  return(coord(2,1));
}

double clNode3D::z(void) const
{
  return(coord(3,1));
}

/*
** Methods for operations
*/

clNode3D& clNode3D::Assign(const clNode3D &source)
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
    EX.AddMethodToTree("clNode3D::Assign");
    throw EX;
  }

  return(*this);
}

clNode3D& clNode3D::operator=(const clNode3D &source)
{
  return(Assign(source));
}

