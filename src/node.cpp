/****************************************************
*** Body file for definition of class clNode
***
*** J.L. Klaufus - Haarlem 2009
**/

#include <algorithm>
#include <sstream>
#include <vector>

#include "globals.h"
#include "exceptiontree.h"
#include "node.h"

clNode::clNode(void)
{
  try
  {
    SetObjectType(otNode);
    SetNodeID(0);
    ResetHaloNode();
    ResetBoundaryNode();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::clNode");
    throw EX;
  }
}

clNode::clNode(const int ID, const dMatrix &m)
{
  try
  {
    SetObjectType(otNode);
    SetNodeID(ID);
    ResetHaloNode();
    ResetBoundaryNode();
    SetCoordinates(m);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::clNode");
    throw EX;
  }
}

clNode::clNode(const clNode &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::clNode");
    throw EX;
  }
}

clNode::~clNode(void)
{
  //cerr << "DESTROYING NODE: " << GetNodeID() << endl;
}

int clNode::SetNodeID(const int number)
{
  nodeID = number;

  return(nodeID);
}

inline int clNode::GetNodeID(void) const
{
  return(nodeID);
}

void clNode::SetHaloNode(void)
{
  isHaloNode = true;
}

void clNode::ResetHaloNode(void)
{
  isHaloNode = false;
}

inline bool clNode::IsHaloNode(void) const
{
  return(isHaloNode);
}

void clNode::SetBoundaryNode(void)
{
  isBoundaryNode = true;
}

void clNode::ResetBoundaryNode(void)
{
  isBoundaryNode = false;
}

inline bool clNode::IsBoundaryNode(void) const
{
  return(isBoundaryNode);
}

dMatrix clNode::GetCoordinates(void) const
{
  return(coord);
}

dMatrix clNode::SetCoordinates(const dMatrix &m)
{
  try
  {
    dMatrix dummy = m;

    if(dummy.GetNumberRows() < dummy.GetNumberColumns())
    {
      dummy.Transpose();
      coord = dummy;
    }
    else
    {
      coord = dummy;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::SetCoordinates");
    throw EX;
  }

  return(coord);
}

int clNode::GetElementID(const int index) const
{
  /*
  ** Returns the element ID given the index
  */

  int elementID = -1;

  try
  {
    elementID = elementIDList[index];
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::GetElementID");
    throw EX;
  }

  return(elementID);
}

int clNode::AddElementID(const int elementID)
{
  /*
  ** Sets the element ID for a given face index
  */

  try
  {
    elementIDList.push_back(elementID);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::AddElementID");
    throw EX;
  }

  return(elementIDList.size());
}

void clNode::DeleteElementRefByIndex(const int index)
{
  try
  {
    elementIDList.erase(elementIDList.begin() + index);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::DeleteElementRefByIndex");
    throw EX;
  }
}

void clNode::DeleteElementRefByID(const int ID)
{
  try
  {
    std::vector<int>::iterator it = std::find(elementIDList.begin(), elementIDList.end(), ID);
    elementIDList.erase(it);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::DeleteElementRefByID");
    throw EX;
  }
}

inline int clNode::GetNumberElements(void) const
{
  return(elementIDList.size());
}

bool clNode::IsElementNode(const int elementID) const
{
  bool ret = false;

  std::vector<int>::const_iterator it = std::find(elementIDList.begin(), elementIDList.end(), elementID);
  ret = (it != elementIDList.end());

  return(ret);
}

clNode& clNode::Assign(const clNode &source)
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

    elementIDList.clear();

    for(int e=0; e<source.GetNumberElements(); e++)
    {
      AddElementID(source.GetElementID(e));
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::Assign");
    throw EX;
  }

  return(*this);
}

clNode& clNode::operator=(const clNode &source)
{
  return(Assign(source));
}

bool clNode::IsEqual(const clNode &source) const
{
  bool isEqual = true; // 0 = not equal; 1 = equal

  try
  {
    /*
    ** Does not check for nodeID
    */

    dMatrix coordA = GetCoordinates();
    dMatrix coordB = source.GetCoordinates();

    if(coordA.GetNumberRows() == coordB.GetNumberRows())
    {
      for(int r=0; r<coordA.GetNumberRows() && isEqual; r++)
      {
        if(fabs(coordA(r+1, 1) - coordB(r+1, 1)) > CLIPRANGE)
        {
          isEqual = isEqual && false;
        }
      }
    }

//    if(isEqual)
//    {
//      cout << "IDENTICAL NODES: [" << GetNodeID() << ", " << source.GetNodeID() << "]" << endl;
//    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clNode::IsEqual");
    throw EX;
  }

  return(isEqual);
}

bool clNode::operator==(const clNode &source) const
{
  return(IsEqual(source));
}

bool clNode::operator!=(const clNode &source) const
{
  return(!IsEqual(source));
}

std::string clNode::GetNodeDescription(void) const
{
  std::stringstream returnString;

  returnString << "NodeID: "<< nodeID << std::endl;

  for(int r=0; r<coord.GetNumberRows(); r++)
  {
    returnString << "     X[" << r << "]: " << coord(r+1, 1) << std::endl;
  }

  return (returnString.str());
}
