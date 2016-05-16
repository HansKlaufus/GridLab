/*************************************************
*** Body file for definition of class clElement
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>
#include <sstream>

#include "element.h"
#include "exception.h"
#include "matrix.h"

clElement::clElement(void)
{
  try
  {
    SetElementID(0);
    ResetInitialised();
    ResetHaloCell();
    ResetBoundaryCell();

    SetNumberNodes(0);
    SetNumberFaces(0);
    SetNumberFaceNodes(0);

//    SetArea(-1);
//    SetVolume(-1);

    // Reset all references to nodes and faces
    nodeIDArray.assign(0, -1);
    neighbourIDArray.assign(0, -1);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::clElement");
    throw EX;
  }
}

clElement::clElement(const int elementID, const int nNodes, const int nFaces, const int nFaceNodes)
{
  try
  {
    SetElementID(elementID);
    ResetInitialised();
    ResetHaloCell();
    ResetBoundaryCell();

    SetNumberNodes(nNodes);
    SetNumberFaces(nFaces);
    SetNumberFaceNodes(nFaceNodes);

//    SetArea(-1);
//    SetVolume(-1);

    // Reset all references to nodes and faces
    nodeIDArray.assign(nNodes, -1);
    neighbourIDArray.assign(nFaces, -1);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::clElement");
    throw EX;
  }
}

clElement::clElement(const int nNodes, const int nFaces, const int nFaceNodes)
{
  try
  {
    SetElementID(0);
    ResetInitialised();
    ResetHaloCell();
    ResetBoundaryCell();

    SetNumberNodes(nNodes);
    SetNumberFaces(nFaces);
    SetNumberFaceNodes(nFaceNodes);

//    SetArea(-1);
//    SetVolume(-1);

    // Reset all references to nodes and faces
    nodeIDArray.assign(nNodes, -1);
    neighbourIDArray.assign(nFaces, -1);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::clElement");
    throw EX;
  }
}

clElement::clElement(const clElement &source)
{
  try
  {
    Assign(source);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::clElement");
    throw EX;
  }
}

clElement::~clElement(void)
{
  //cerr << "DESTROYING ELEMENT: " << GetElementID() << endl;
}

int clElement::SetElementID(const int number)
{
  elementID = number;

  return(elementID);
}

int clElement::SetNumberNodes(const int number)
{
  numberNodes = number;

  return(numberNodes);
}

int clElement::SetNumberFaces(const int number)
{
  numberFaces = number;

  return(numberFaces);
}

int clElement::SetNumberFaceNodes(const int number)
{
  numberFaceNodes = number;

  return(numberFaceNodes);
}

inline int clElement::GetElementID(void) const
{
  return(elementID);
}

inline int clElement::GetNumberNodes(void) const
{
  return(numberNodes);
}

inline int clElement::GetNumberFaces(void) const
{
  return(numberFaces);
}

inline int clElement::GetNumberFaceNodes(void) const
{
  return(numberFaceNodes);
}

int clElement::GetNodeID(const int index) const
{
  /*
  ** Returns the node ID given the node index
  */

  if(index<0 || index>=numberNodes)
  {
    throw clException("clElement", "GetNodeID", "Invalid index specification.");
  }

  return(nodeIDArray[index]);
}

int clElement::GetNeighbourID(const int index) const
{
  /*
  ** Returns the neighbour ID given a face index
  */

  if(index<0 || index>=numberFaces)
  {
    throw clException("clElement", "GetNeighbourID", "Invalid index specification.");
  }

  return(neighbourIDArray[index]);
}

int clElement::SetNodeID(const int nodeIndex, const int nodeID)
{
  /*
  ** Sets the node ID for a given node index
  */

  if(nodeIndex<0 || nodeIndex>=numberNodes)
  {
    throw clException("clElement", "SetNodeID", "Invalid index specified.");
  }

  // Set the node
  nodeIDArray[nodeIndex] = nodeID;

  return(numberNodes);
}

int clElement::SetNeighbourID(const int faceIndex, const int neighbourID)
{
  /*
  ** Sets the neighbour ID for a given face index
  */

  if(faceIndex<0 || faceIndex>=numberFaces)
  {
    throw clException("clElement", "SetNeighbourID", "Invalid index specified.");
  }

  // Set the face
  neighbourIDArray[faceIndex] = neighbourID;

  return(numberFaces);
}

void clElement::DeleteNodeRef(const int nodeIndex)
{
  /*
  ** Sets the node ID to -1 given a node index
  */

  try
  {
    nodeIDArray[nodeIndex] = -1;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::DeleteNodeRef");
    throw EX;
  }
}

void clElement::DeleteNeighbourRef(const int faceIndex)
{
  /*
  ** Sets the face ID to -1 given a face index
  */

  try
  {
    neighbourIDArray[faceIndex] = -1;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::DeleteNeighbourRef");
    throw EX;
  }
}

int clElement::GetNodeIndex(const int nodeID) const
{
  int nodeIndex = 0;

  try
  {
    while(nodeIndex<numberNodes && nodeIDArray[nodeIndex]!=nodeID)
    {
      nodeIndex++;
    }

    if(nodeIndex>=numberNodes)
    {
      char message[MESLEN];
      sprintf(message, "Could not find node.\n    NODEID   = %d\n", nodeID);
      throw clExceptionTree("clElement", "GetNodeIndex", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::GetNodeIndex");
    throw EX;
  }

   return(nodeIndex);
}

int clElement::GetFaceIndex(const int neighbourID) const
{
  int faceIndex = 0;

  try
  {
    while(faceIndex<numberFaces && neighbourIDArray[faceIndex]!=neighbourID)
    {
      faceIndex++;
    }

    if(neighbourIDArray[faceIndex]!=neighbourID)
    {
      char message[MESLEN];
      sprintf(message, "Could not find faceEntry pointing to neighbour.\n    ELEMENTID   = %d\n    NEIGHBOURID = %d\n", elementID, neighbourID);
      throw clExceptionTree("clElement", "GetFaceIndex", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::GetFaceIndex");
    throw EX;
  }

  return(faceIndex);
}

int clElement::FindFaceIndex(const int theNeighbourID) const
{
  int theFaceIndex = 0;

  try
  {
    while(theFaceIndex<numberFaces && neighbourIDArray[theFaceIndex]!=theNeighbourID)
    {
      theFaceIndex++;
    }

    if(neighbourIDArray[theFaceIndex]!=theNeighbourID)
    {
      char message[MESLEN];
      sprintf(message, "Could not find faceIndex pointing to neighbour.\n    ELEMENTID   = %d\n    NEIGHBOURID = %d\n", elementID, theNeighbourID);
      throw clExceptionTree("clElement", "FindFaceIndex", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clElement::FindFaceIndex");
    throw EX;
  }

  return(theFaceIndex);
}

bool clElement::IsElementNode(const int nodeID) const
{
  bool ret = false;

  vector<int>::const_iterator it = find(nodeIDArray.begin(), nodeIDArray.end(), nodeID);
  ret = (it != nodeIDArray.end());

  return(ret);
}

bool clElement::IsInferiorElement(void) const
{
  bool ret = false;

  // Loop through element faces
  for(int fn=0; fn<numberNodes-1; fn++)
  {
    int firstNodeID = nodeIDArray[fn];
    for(int sfn=fn+1; sfn<numberNodes; sfn++)
    {
      int secondNodeID = nodeIDArray[sfn];
      if(firstNodeID==secondNodeID)
      {
        ret=true;
      }
    }
  }

  return(ret);
}

void clElement::SetHaloCell(void)
{
  isHaloCell = true;
}

void clElement::ResetHaloCell(void)
{
  isHaloCell = false;
}

inline bool clElement::IsHaloCell(void) const
{
  return(isHaloCell);
}

void clElement::SetBoundaryCell(void)
{
  isBoundaryCell = true;
}

void clElement::ResetBoundaryCell(void)
{
  isBoundaryCell = false;
}

inline bool clElement::IsBoundaryCell(void) const
{
  return(isBoundaryCell);
}

void clElement::SetInitialised(void)
{
  isInitialised = true;
}

void clElement::ResetInitialised(void)
{
  isInitialised = false;
}

inline bool clElement::IsInitialised(void) const
{
  return(isInitialised);
}

void clElement::GetFaceNodes(const int face, int *faceNodes) const
{
}

clElement& clElement::Assign(const clElement &source)
{
  SetElementID(source.GetElementID());
  SetNumberNodes(source.GetNumberNodes());
  SetNumberFaces(source.GetNumberFaces());
  SetNumberFaceNodes(source.GetNumberFaceNodes());

  source.IsInitialised() ? SetInitialised() : ResetInitialised();
  source.IsHaloCell() ? SetHaloCell() : ResetHaloCell();
  source.IsBoundaryCell() ? SetBoundaryCell() : ResetBoundaryCell();

  for(int n=0; n<source.GetNumberNodes(); n++)
  {
    nodeIDArray[n] = source.GetNodeID(n);
  }

  for(int f=0; f<source.GetNumberFaces(); f++)
  {
    neighbourIDArray[f] = source.GetNeighbourID(f);
  }

  return(*this);
}

clElement& clElement::operator=(const clElement &source)
{
  return(Assign(source));
}

bool clElement::IsEqual(const clElement &source) const
{
  /*
  ** Does not evaluate 'elementID'
  */
  int ret = 0; //0 = not equal; 1 = equal

  for(int n=0; n<numberNodes; n++)
  {
    if(nodeIDArray[n] != source.GetNodeID(n))
    {
      ret = false;
    }
  }

  for(int f=0; f<numberFaces; f++)
  {
    if(neighbourIDArray[f] != source.GetNeighbourID(f))
    {
      ret = false;
    }
  }

  return(ret);
}

bool clElement::operator==(const clElement &source) const
{
  return(IsEqual(source));
}

bool clElement::operator!=(const clElement &source) const
{
  return(!IsEqual(source));
}

string clElement::GetElementDescription(void) const
{
  stringstream returnString;

  returnString << endl << "ElementID: " << elementID << endl;

  for(unsigned int n=0; n<nodeIDArray.size(); n++)
  {
    returnString << "NodeID [" << n << "]: " << nodeIDArray[n] << endl;
  }

  for(unsigned int n=0; n<neighbourIDArray.size(); n++)
  {
    returnString << "NeighbourID [" << n << "]: " << neighbourIDArray[n] << endl;
  }

  return (returnString.str());
}

double clElement::GetArea(void)
{
  return(area);
}

double clElement::SetArea(const double newArea)
{
  area = newArea;

  return(area);
}

double clElement::GetVolume(void)
{
  return(volume);
}

double clElement::SetVolume(const double newVolume)
{
  volume = newVolume;

  return(volume);
}

double clElement::GetMinimumAngle(void)
{
  return(minimumAngle);
}

double clElement::SetMinimumAngle(const double newAngle)
{
  minimumAngle = newAngle;

  return(minimumAngle);
}
