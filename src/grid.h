/*********************************************************
*** Header file for declaration of class tpGrid
***
*** J.L. Klaufus - Haarlem 2009
**/

#ifndef GRID_H
#define GRID_H

#include <fstream>
#include <string>
#include <string.h>
#include <vector>

#include "matrix.h"
#include "indexedlist.h"

using namespace std;

template<class NTYPE, class ETYPE>
class tpGrid
{
private:
  enObjectType          objectType;
  int                   numberID;
  string                name;

  vector<ETYPE*>        theElementList;
  vector<NTYPE*>        theNodeList;

  int                   SetNumberID(const int);

protected:
  void                  SetObjectType(enObjectType type) {objectType = type;};

  int                   AddElement(ETYPE*);
  void                  DeleteInferiorElements(void);
  void                  StitchElements(void);

  int                   AddNode(NTYPE*);
  void                  DeleteNode(const int);
  void                  DeleteDuplicateNodes(void);

  bool                  IsEqualFace(ETYPE*, const int, ETYPE*, const int);

  bool                  CheckElementNodes(void) const;
  bool                  CheckElementConnections(void) const;
  bool                  CheckNodeConnections(void) const;
  bool                  CheckElementParentGrid(void) const;

  void                  DumpNodes(fstream&) const;
  void                  DumpElements(fstream&) const;

public:
  tpGrid(void);
  tpGrid(const int, string = "noName");
  tpGrid(const tpGrid&);
  ~tpGrid(void);

  enObjectType          GetObjectType(void) const {return (objectType);};
  int                   GetNumberID(void) const;

  void                  SetName(const string);
  string                GetName(void);

  NTYPE                 GetNode(const int) const;
  NTYPE*                GetNodePointer(const int);
  int                   GetNodeIndex(const int) const;
  int                   GetNumberNodes(void) const;
  int                   GetLastNodeID(void);
  int                   GetNewNodeID(void);

  ETYPE                 GetElement(const int) const;
  ETYPE*                GetElementPointer(const int);
  int                   GetElementIndex(const int) const;
  int                   GetNumberElements(void) const;
  int                   GetLastElementID(void);
  int                   GetNewElementID(void);
  void                  DeleteElement(const int);

  double                GetMaxCoordinate(const int) const;
  double                GetMinCoordinate(const int) const;

  void                  SetBoundaryCells(void);
  bool                  CheckGrid(void) const;
  double                GetArea(void);
  double                GetVolume(void);

  void                  DumpGrid(void) const;
  void                  DumpGrid(const string) const;
  void                  DumpGrid(const char*) const;
};

/******************************************************
*** Body file for definition of class tpGrid
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <fstream>
#include <stdlib.h>

#include "exceptiontree.h"
#include "grid.h"
#include "matrix.h"

using namespace std;

template<class NTYPE, class ETYPE>
tpGrid<NTYPE, ETYPE>::tpGrid(void)
{
  try
  {
    SetObjectType(otGrid);
    SetNumberID(0);
    SetName("noName");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::tpGrid");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
tpGrid<NTYPE, ETYPE>::tpGrid(const int ID, const string theName)
{
  try
  {
    SetObjectType(otGrid);
    SetNumberID(ID);
    SetName(theName);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::tpGrid");
    throw EX;
  }
}

//template<class NTYPE, class ETYPE>
//tpGrid<NTYPE, ETYPE>::tpGrid(const tpGrid &source)
//{
//  try
//  {
//  }
//
//  catch(clExceptionTree EX)
//  {
//    EX.AddMethodToTree("tpGrid::CreateGrid");
//    throw EX;
//  }
//}

template<class NTYPE, class ETYPE>
tpGrid<NTYPE, ETYPE>::~tpGrid(void)
{
  //cerr << "DESTROYING GRID: " << GetNumberID() << endl;
  for(unsigned int e=0; e<theElementList.size(); e++) delete theElementList[e];
  for(unsigned int n=0; n<theNodeList.size(); n++)    delete theNodeList[n]; //TODO: clNode3D has non-virtual destructor - CHANGE
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::SetNumberID(const int ID)
{
  numberID = ID;

  return(numberID);
}

template<class NTYPE, class ETYPE>
string tpGrid<NTYPE, ETYPE>::GetName(void)
{
  return (name);
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::SetName(const string theName)
{
  name = theName;
}

template<class NTYPE, class ETYPE>
bool tpGrid<NTYPE, ETYPE>::CheckGrid(void) const
{
  bool ret=true;

  try
  {
    // Check initial grid
    ret = ret && CheckElementNodes();
    ret = ret && CheckElementConnections();
    ret = ret && CheckNodeConnections();
    ret = ret && CheckElementParentGrid();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::CheckGrid");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpGrid<NTYPE, ETYPE>::CheckElementNodes(void) const
{
  bool ret=true;

  try
  {
    // Loop through all elements
    for(unsigned int e=0; e<theElementList.size(); e++)
    {
      ETYPE *theElement = theElementList[e];
      if(theElement->IsInferiorElement())
      {
        ret=false;

        char message[MESLEN];
        sprintf(message, "Error in grid.\n\tInferior element:\n\tELEMENT ID\t= %d", theElement->GetElementID());
        throw clExceptionTree("tpGrid", "CheckElementNodes", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::CheckElementNodes");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpGrid<NTYPE, ETYPE>::CheckElementConnections(void) const
{
  bool ret=true;

  try
  {
    // Loop through all elements
    for(unsigned int e=0; e<theElementList.size(); e++)
    {
      ETYPE* activeElement     = theElementList[e];
      int    activeID          = activeElement->GetElementID();
      int    numberConnections = 0;

      // Loop through faces and find element that is referred to
      for(int activeFace=0; activeFace<activeElement->GetNumberFaces(); activeFace++)
      {
        int passiveID = activeElement->GetNeighbourID(activeFace);

        if(passiveID!=-1)
        {
          int    passiveIndex   = GetElementIndex(passiveID);
          ETYPE* passiveElement = theElementList[passiveIndex];

          // Check if passiveElement also refers to activeElement
          int  passiveFace = 0;
          bool succes      = false;
          while(passiveFace<passiveElement->GetNumberFaces() && !succes)
          {
            if(passiveElement->GetNeighbourID(passiveFace) == activeID)
            {
              succes = true;
              numberConnections++;
            }

            passiveFace++;
          }

          if(!succes)
          {
            ret=false;

            // passiveElement does NOT refer to activeElement
            char message[MESLEN];
            sprintf(message, "Error in grid.\n    %d.%d is one-way connected to %d, ", activeID, activeFace, passiveID);
            throw clExceptionTree("tpGrid", "CheckElementConnections", message);
          }
        }
        else
        {
          // Passive element is halo cell; do not check
          numberConnections++;
        }
      }

      if(numberConnections<activeElement->GetNumberFaces())
      {
        ret=false;

        char message[MESLEN];
        sprintf(message, "Error in grid; could not find connection.\n    ELEMENTID = %d", activeID);
        throw clExceptionTree("tpGrid", "CheckElementConnections", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::CheckElementConnections");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpGrid<NTYPE, ETYPE>::CheckNodeConnections(void) const
{
  bool ret=true;

  try
  {
    // Loop through all nodes
    for(unsigned int n=0; n<theNodeList.size(); n++)
    {
      NTYPE *node           = theNodeList[n];
      int    numberElements = node->GetNumberElements();
      if(numberElements == 0)
      {
        ret=false;

        char message[MESLEN];
        sprintf(message, "Error in grid; floating node detected.\n    NODEID = %d", node->GetNodeID());
        throw clExceptionTree("tpGrid", "CheckNodeConnections", message);
      }
      else
      {
        // Loop through all elements that the node refers to
        for(int e=0; e<numberElements; e++)
        {
          int    elementID    = node->GetElementID(e);
          int    elementIndex = GetElementIndex(elementID);
          ETYPE *element      = theElementList[elementIndex];

          int    numberNodes  = element->GetNumberNodes();
          int    nn=0;

          // Search in element for a reference to this node
          bool found = false;
          while(nn<numberNodes && !found)
          {
            found = (element->GetNodeID(nn) == node->GetNodeID());
            nn++;
          }

          if(!found)
          {
            ret=false;

            char message[MESLEN];
            sprintf(message, "Error in grid; no bi-directional reference between node and element.\n    NODEID = %d\n    ELEMENTID = %d", node->GetNodeID(), element->GetElementID());
            throw clExceptionTree("tpGrid", "CheckNodeConnections", message);
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::CheckNodeConnections");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpGrid<NTYPE, ETYPE>::CheckElementParentGrid(void) const
{
  int ret=true;

  try
  {
    // Loop through all elements
    for(unsigned int e=0; e<theElementList.size(); e++)
    {
      ETYPE *element = theElementList[e];
      if(!element->GetParentGrid())
      {
        ret=false;

        char message[MESLEN];
        sprintf(message, "Error in grid; element has no Parent Grid.\nPerform binding to Parent Grid prior to CheckGrid.\n    ELEMENTID = %d", element->GetElementID());
        throw clExceptionTree("tpGrid", "CheckElementParentGrid", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::CheckElementParentGrid");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetNumberID(void) const
{
  return(numberID);
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetNumberElements(void) const
{
  return(theElementList.size());
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetNumberNodes(void) const
{
  return(theNodeList.size());
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetLastElementID(void)
{
  int lastElementID = 0;

  if(!theElementList.empty())
  {
    int    index       = theElementList.size()-1;
    ETYPE *lastElement = theElementList[index];
    lastElementID      = lastElement->GetElementID();
  }

  return(lastElementID);
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetNewElementID(void)
{
  return(GetLastElementID()+1);
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetLastNodeID(void)
{
  int lastNodeID = 0;

  if(!theNodeList.empty())
  {
    int    index    = theNodeList.size()-1;
    NTYPE *lastNode = theNodeList[index];
    lastNodeID      = lastNode->GetNodeID();
  }

  return(lastNodeID);
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetNewNodeID(void)
{
  return(GetLastNodeID()+1);
}

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetElementIndex(const int theElementID) const
{
  int index = -1;

  try
  {
    int min = 0;
    int max = theElementList.size()-1;

    bool found = false;
    while(!found && min<=max)
    {
      int mid = (min + max)/2;

      int compareID = theElementList[mid]->GetElementID();
      if(theElementID < compareID)
      {
        max = mid-1;
      }
      else if(theElementID > compareID)
      {
        min = mid+1;
      }
      else //(theElementID == compareID)
      {
        found = true;
        index = mid;
      }
    }

    if(!found)
    {
      char message[MESLEN];
      sprintf(message, "Could not find element.\n    ELEMENTID = %d", theElementID);
      throw clExceptionTree("tpGrid", "GetElementIndex", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::GetElementIndex");
    throw EX;
  }

  return(index);
}

//template<class NTYPE, class ETYPE>
//int tpGrid<NTYPE, ETYPE>::GetElementIndex(const int theElementID) const
//{
//  int index = 0;
//
//  try
//  {
//    int numberElements = theElementList.size();
//
//    while((index<numberElements) && (theElementList[index]->GetElementID()!=theElementID))
//    {
//      index++;
//    }
//
//    if(index==numberElements)
//    {
//      char message[MESLEN];
//      sprintf(message, "Could not find element.\n    ELEMENTID = %d", theElementID);
//      throw clExceptionTree("tpGrid", "GetElementIndex", message);
//    }
//  }
//
//  catch(clExceptionTree EX)
//  {
//    EX.AddMethodToTree("tpGrid::GetElementIndex");
//    throw EX;
//  }
//
//  return(index);
//}
//

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::GetNodeIndex(const int theNodeID) const
{
  //
  // Returns the nodeIndex for a given nodeID, using the grid System
  //

  int index = -1;

  try
  {
    int min = 0;
    int max = theNodeList.size()-1;

    bool found = false;
    while(!found && min<=max)
    {
      int mid = (min + max)/2;

      int compareID = theNodeList[mid]->GetNodeID();
      if(theNodeID < compareID)
      {
        max = mid-1;
      }
      else if(theNodeID > compareID)
      {
        min = mid+1;
      }
      else //(theNodeID == compareID)
      {
        found = true;
        index = mid;
      }
    }

    if(!found)
    {
      char message[MESLEN];
      sprintf(message, "Could not find node.\n    NODEID = %d", theNodeID);
      throw clExceptionTree("tpGrid", "GetNodeIndex", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::GetNodeIndex");
    throw EX;
  }

  return(index);
}

//template<class NTYPE, class ETYPE>
//int tpGrid<NTYPE, ETYPE>::GetNodeIndex(const int theNodeID) const
//{
//  //
//  // Returns the nodeIndex for a given nodeID, using the grid System
//  //
//
//  int index = 0;
//
//  try
//  {
//    int numberNodes=theNodeList.size();
//
//    while(index<numberNodes && theNodeList[index]->GetNodeID()!=theNodeID)
//    {
//      index++;
//    }
//
//    if(index==numberNodes)
//    {
//      char message[MESLEN];
//      sprintf(message, "Could not find node.\n    NODEID = %d", theNodeID);
//      throw clExceptionTree("tpGrid", "GetNodeIndex", message);
//    }
//  }
//
//  catch(clExceptionTree EX)
//  {
//    EX.AddMethodToTree("tpGrid::GetNodeIndex");
//    throw EX;
//  }
//
//  return(index);
//}
//
template<class NTYPE, class ETYPE>
ETYPE tpGrid<NTYPE, ETYPE>::GetElement(const int index) const
{
  return(*theElementList[index]);
}

template<class NTYPE, class ETYPE>
ETYPE* tpGrid<NTYPE, ETYPE>::GetElementPointer(const int index)
{
  return(theElementList[index]);
}

template<class NTYPE, class ETYPE>
NTYPE tpGrid<NTYPE, ETYPE>::GetNode(const int index) const
{
  return(*theNodeList[index]);
}

template<class NTYPE, class ETYPE>
NTYPE* tpGrid<NTYPE, ETYPE>::GetNodePointer(const int index)
{
  return(theNodeList[index]);
}

/***************************************************************
***
***   Performing Node Characteristic Operation
***
**/

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::AddNode(NTYPE *theNode)
{
  //
  // IN : theNode   = the new node
  // OUT: newNodeID = ID of new node
  //

  // TODO: newNodeID is not really assigned to theNode

  int newNodeID = 1;

  try
  {
    if(!theNodeList.empty())
    {
      newNodeID = this->GetNewNodeID();
    }

    theNodeList.push_back(theNode);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::AddNode");
    throw EX;
  }

  return(newNodeID);
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DeleteNode(const int nodeID)
{
  //
  // IN : nodeID = ID of node to be deleted
  // OUT: -
  //

  try
  {
    int    nodeIndex = GetNodeIndex(nodeID);
    NTYPE *node      = theNodeList[nodeIndex];

    // Delete the elements this node refers to
    while(node->GetNumberElements()>0)
    {
      int elementID = node->GetElementID(0);
      this->DeleteElement(elementID);
    }

    // Delete the node itself
    theNodeList.erase(theNodeList.begin() + nodeIndex);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DeleteNode");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DeleteDuplicateNodes(void)
{
  try
  {
    cout << "Deleting duplicate nodes..." << endl;

    int oldProgress = 0;
    int newProgress = 0;

    for(unsigned int n=0; n<theNodeList.size()-1; n++)
    {
      // Calculate and Print the progress
      newProgress = n*100.0/theNodeList.size();
      if((newProgress - oldProgress)>0.5)
      {
          cout << "Progress: " << newProgress << "%" << endl;
          oldProgress = newProgress;
      }

      // Continue with next node
      NTYPE *masterNode   = theNodeList[n];
      int    masterNodeID = masterNode->GetNodeID();

      for(unsigned int nn=n+1; nn<theNodeList.size(); nn++)
      {
        NTYPE *slaveNode   = theNodeList[nn];
        int    slaveNodeID = slaveNode->GetNodeID();

        tpMatrix<double> coords1 = masterNode->GetCoordinates();
        tpMatrix<double> coords2 = slaveNode->GetCoordinates();

        bool sameNodes = true;
        for(int r=0; r<coords1.GetNumberRows(); r++)
        {
          sameNodes = sameNodes && (fabs(coords1.GetElement(r+1,1) - coords2.GetElement(r+1,1))<PRECISION);
        }

        if(sameNodes)
        {
          // Replace references to duplicate node
          while(slaveNode->GetNumberElements()>0)
          {
            int    elementID    = slaveNode->GetElementID(0);
            int    elementIndex = GetElementIndex(elementID);
            ETYPE *element      = GetElementPointer(elementIndex);
            int    nodeIndex    = element->GetNodeIndex(slaveNodeID);

            element->SetNodeID(nodeIndex, masterNodeID); // Replace references to duplicate node
            masterNode->AddElementID(elementID);         // Add elementID to node
            slaveNode->DeleteElementRefByID(elementID);  // Delete reference to elementID

            cout << "Replacing node " << slaveNodeID << " by node " << masterNodeID << " for element " << elementID << endl;
          }

          DeleteNode(slaveNodeID);  // Delete Node
          nn--;                     // Reset nn
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DeleteDuplicateNodes");
    throw EX;
  }
}

/***************************************************************
***
***  Perforing Element Characteristic Operations
***
**/

template<class NTYPE, class ETYPE>
int tpGrid<NTYPE, ETYPE>::AddElement(ETYPE *theElement)
{
  //
  // IN : theElement   = the new element
  // OUT: newElementID = ID of new element
  //

  // TODO: newElementID is not really assigned to theElement

  int newElementID = 1;

  try
  {
    if(!theElementList.empty()>0)
    {
      newElementID = this->GetNewElementID();
      theElement->SetElementID(newElementID);
    }

    theElementList.push_back(theElement);

    // TODO: This is suspicious code! similarly is NOT done for AddNode....
    // Add elementID to the nodes of this new element
    for(int n=0; n<theElement->GetNumberNodes(); n++)
    {
      int theNodeID = theElement->GetNodeID(n);
      if(theNodeID!=-1)
      {
        int    theNodeIndex = this->GetNodeIndex(theNodeID);
        NTYPE *theNode      = this->GetNodePointer(theNodeIndex);
        theNode->AddElementID(theElement->GetElementID());
      }

      // TODO: Do similarly for neighbouring elements?
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::AddElement");
    throw EX;
  }

  return(newElementID);
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DeleteElement(const int theElementID)
{
  //
  // IN : theElementID = ID of element to be deleted
  // OUT: -
  //

  try
  {
    int    theElementIndex = GetElementIndex(theElementID);
    ETYPE *theElement      = theElementList[theElementIndex];

    // Delete the references from the nodes to this element
    for(int n=0; n<theElement->GetNumberNodes(); n++)
    {
      int theNodeID = theElement->GetNodeID(n);

      if(theNodeID!=-1)
      {
        int    theNodeIndex = GetNodeIndex(theNodeID);
        NTYPE *theNode      = theNodeList[theNodeIndex];

        theNode->DeleteElementRefByID(theElementID);
      }
    }

    // Delete the references from the neighbour elements to this element
    for(int e=0; e<theElement->GetNumberFaces(); e++)
    {
      // Get neighbour from element
      int theNeighbourID = theElement->GetNeighbourID(e);

      // Check for halo cells
      if(theNeighbourID!=-1)
      {
        int    theNeighbourIndex = GetElementIndex(theNeighbourID);
        ETYPE* theNeighbour      = theElementList[theNeighbourIndex];

        // Search in neighbour for element and delete
        for(int f=0; f<theNeighbour->GetNumberFaces(); f++)
        {
          if(theNeighbour->GetNeighbourID(f)==theElementID)
          {
            theNeighbour->DeleteNeighbourRef(f); //->SetNeighbourID(f, -1);
          }
        }
      }
    }

    // Delete the element itself
    theElementList.erase(theElementList.begin() + theElementIndex);

    delete theElement;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DeleteElement");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DeleteInferiorElements(void)
{
  try
  {
    for(unsigned int e=0; e<theElementList.size(); e++)
    {
      ETYPE *element = theElementList[e];
      if (element->IsInferiorElement())
      {
        DeleteElement(element->GetElementID());
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DeleteInferiorElements");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::StitchElements(void)
{
  //
  // Creates references to neighbouring elements and vice versa
  //

  try
  {
    cout << "Stitching elements..." << endl;

    int oldProgress = 0;
    int newProgress = 0;

    int numberElements = theElementList.size();

    // Loop through all elements
    for(int fe=0; fe<numberElements-1; fe++)
    {
      // Calculate and Print the progress
      newProgress = fe*100.0/theNodeList.size();
      if((newProgress - oldProgress)>0.5)
      {
          cout << "Progress: " << newProgress << "%" << endl;
          oldProgress = newProgress;
      }

      // Start with next element
      ETYPE *firstElement = theElementList[fe];

      // Check if firstElement has reference to -1
      for(int ff=0; ff<firstElement->GetNumberFaces(); ff++)
      {
        // Check if current face has no neighbour
        if(firstElement->GetNeighbourID(ff)<0)
        {
          bool connected = false;

          // Loop through all other elements
          for(int se=fe+1; !connected && se<numberElements; se++)
          {
            ETYPE *secondElement = theElementList[se];

            // Check if secondElement has reference to -1
            for(int sf=0; !connected && sf<secondElement->GetNumberFaces(); sf++)
            {
              if(secondElement->GetNeighbourID(sf)<0)
              {
                // Check if both faces are identical
                if(IsEqualFace(firstElement, ff, secondElement,sf))
                {
                  // Create reference from firstElement to secondElement and vice versa
                  firstElement->SetNeighbourID(ff, secondElement->GetElementID());
                  secondElement->SetNeighbourID(sf, firstElement->GetElementID());
                  connected = true;
//                  printf("Connecting element %d.%d to %d.%d\n", firstElement->GetElementID(), ff, secondElement->GetElementID(),sf);
                }
              }
            }
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::StitchElements");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
bool tpGrid<NTYPE, ETYPE>::IsEqualFace(ETYPE *masterElement, const int masterFaceIndex, ETYPE *slaveElement, const int slaveFaceIndex)
{
  //
  // IN : masterNodeIDs = array containing the ID numbers of the nodes of the master face
  //      slaveNodeIDs  = array containing the ID numbers of the nodes of the slave face
  // OUT: ret = TRUE if current master face is equal to slave face
  //            FALSE if current master face is not equal to slave face
  //

  bool ret = false;

  try
  {
    int numberConnectedNodes = 0;

    for(int i=0; i<masterElement->GetNumberFaceNodes(); i++)
    {
      int masterNodeIndex = masterElement->GetFaceNode(masterFaceIndex, i);
      int masterNodeID    = masterElement->GetNodeID(masterNodeIndex);
      for(int j=0; j<slaveElement->GetNumberFaceNodes(); j++)
      {
        int slaveNodeIndex = slaveElement->GetFaceNode(slaveFaceIndex, j);
        int slaveNodeID    = slaveElement->GetNodeID(slaveNodeIndex);

        if(masterNodeID==slaveNodeID) numberConnectedNodes++;
      }
    }

    if(numberConnectedNodes== masterElement->GetNumberFaceNodes() && numberConnectedNodes== slaveElement->GetNumberFaceNodes())
    {
      ret=true;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::IsEqualFace");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DumpGrid(void) const
{
  try
  {
    DumpGrid("grid.csv");
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DumpGrid");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DumpGrid(const string fileName) const
{
  try
  {
    DumpGrid(fileName.c_str());
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DumpGrid");
    throw EX;
  }
}


template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DumpGrid(const char* fileName) const
{
  try
  {
    fstream fout;
    fout.open(fileName, ios::out);
    fout.precision(3);
    if (fout.good())
    {
      DumpNodes(fout);
      DumpElements(fout);
      fout.close();
    }
    else
    {
      throw clExceptionTree("tpGrid", "DumpGrid", "Could not open dump-file.");
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DumpGrid");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DumpNodes(fstream &fout) const
{
  try
  {
    // Dump all nodes with the element-references
    fout << "NODE ID;COORDS;ELEMENTS;" << endl;
    for(unsigned int n=0; n<theNodeList.size(); n++)
    {
      NTYPE *node  = theNodeList[n];
      fout << node->GetNodeID() << ";";

      tpMatrix<double> coord = node->GetCoordinates();
      for(int r=0; r<coord.GetNumberRows(); r++)
      {
        fout << coord(r+1, 1) << ";";
      }

      for(int e=0; e<node->GetNumberElements(); e++)
      {
        fout << node->GetElementID(e) << ";";
      }
      fout << endl;
    }
    fout << endl;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DumpNodes");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::DumpElements(fstream &fout) const
{
  try
  {
    // Dump all elements with neighbour-references and node-references
    fout << "ELEMENT ID;NEIGHBOUR REFERENCES;;;;NODE REFERENCES;;;" << endl;
    for(unsigned int e=0; e<theElementList.size(); e++)
    {
      ETYPE *element = theElementList[e];
      fout << element->GetElementID() << ";";

      for(int nb=0; nb<element->GetNumberFaces(); nb++)
      {
        fout << element->GetNeighbourID(nb) << ";";
      }

      for(int no=0; no<element->GetNumberNodes(); no++)
      {
        fout << element->GetNodeID(no) << ";";
      }
      fout << endl;
    }
    fout << endl;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::DumpElements");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
double tpGrid<NTYPE, ETYPE>::GetMaxCoordinate(const int coordType) const
{
  tpMatrix<double> coords;
  double           coordValue;

  try
  {
    coords     = theNodeList[0]->GetCoordinates();
    coordValue = coords(coordType, 1);

    for(unsigned int n=1; n<theNodeList.size(); n++)
    {
      coords     = theNodeList[n]->GetCoordinates();
      if (coords(coordType, 1) > coordValue)
      {
        coordValue = coords(coordType, 1);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::GetMaxCoordinate");
    throw EX;
  }

  return (coordValue);
}

template<class NTYPE, class ETYPE>
double tpGrid<NTYPE, ETYPE>::GetMinCoordinate(const int coordType) const
{
  tpMatrix<double> coords;
  double           coordValue;

  try
  {
    coords     = theNodeList[0]->GetCoordinates();
    coordValue = coords(coordType, 1);

    for(unsigned int n=1; n<theNodeList.size(); n++)
    {
      coords     = theNodeList[n]->GetCoordinates();
      if (coords(coordType, 1) < coordValue)
      {
        coordValue = coords(coordType, 1);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::GetMaxCoordinate");
    throw EX;
  }

  return (coordValue);
}

template<class NTYPE, class ETYPE>
void tpGrid<NTYPE, ETYPE>::SetBoundaryCells(void)
{
  try
  {
    for(int e=0; e<GetNumberElements(); e++)
    {
      ETYPE *element = GetElementPointer(e);
      if(element->IsHaloCell())
      {
        for(int f=0; f<element->GetNumberFaces(); f++)
        {
          int neighbourID = element->GetNeighbourID(f);
          if(neighbourID != -1)
          {
            int neighbourIndex = GetElementIndex(neighbourID);
            ETYPE *neighbour = GetElementPointer(neighbourIndex);
            neighbour->SetBoundaryCell();
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::SetBoundaryCells");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
double tpGrid<NTYPE, ETYPE>::GetArea(void)
{
  double totalArea = 0.0f;

  try
  {
    for(int e=0; e<GetNumberElements(); e++)
    {
      ETYPE *element = GetElementPointer(e);
      if(!element->IsHaloCell() && !element->IsBoundaryCell())
      {
        totalArea += element->GetArea();
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::GetArea");
    throw EX;
  }

  return (totalArea);
}

template<class NTYPE, class ETYPE>
double tpGrid<NTYPE, ETYPE>::GetVolume(void)
{
  double totalVolume = 0.0f;

  try
  {
    for(int e=0; e<GetNumberElements(); e++)
    {
      ETYPE *element = GetElementPointer(e);
      if(!element->IsHaloCell() && !element->IsBoundaryCell())
      {
        totalVolume += element->GetVolume();
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpGrid::GetVolume");
    throw EX;
  }

  return (totalVolume);
}

#endif
