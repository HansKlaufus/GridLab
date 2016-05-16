/*********************************************************
*** Header file for declaration of class tpUnstructured
***
*** J.L. Klaufus - Haarlem 2009
**/

#ifndef UNSTRUCTURED_H
#define UNSTRUCTURED_H

#include <algorithm>
#include <time.h>
#include <vector>

#include "globals.h"
#include "grid.h"

using namespace std;

template<class NTYPE, class ETYPE>
class tpUnstructured : public tpGrid<NTYPE, ETYPE>
{
private:
  int                   FindFaceIndex(const ETYPE, const int);
  int                   FindEnclosingElement(const NTYPE*);
  void                  FindAffectedElements_Simple(const int, vector<int>&, vector<int>&, vector<int>&);
  void                  FindAffectedElements_Advanced(const int, NTYPE*, vector<int>&, vector<int>&, vector<int>&);
  void                  ConnectNewElements(vector<int>&);
  bool                  FindFaceConnection(const int, const int, int&, int&);
  bool                  IsDuplicateNode(const NTYPE*);

  int                   CorrectInSphereCriterion(void);

  bool                  CheckElementVolumes(void);
  bool                  CheckInSphereCriterion(void);
  bool                  CheckTotalVolume(void);

public:
  tpUnstructured(void);
  tpUnstructured(const int);
  tpUnstructured(const tpUnstructured&);
  virtual ~tpUnstructured(void);

  tpMatrix<double>      GetNodeCoordinates(const NTYPE) const;

  virtual ETYPE        *CreateElement(void) = 0;

  int                   InsertNode(NTYPE*, bool);
  int                   CorrectGrid(void);
  bool                  CheckGrid(void);
  bool                  DeleteInitialGrid(void);
  void                  RandomCentroidInsertion(const int);
};


/******************************************************
*** Body file for definition of class tpUnstructured
***
*** J.L. Klaufus - Haarlem 2009
**/

#include <fstream>
#include <stdlib.h>
//#include <time.h>

#include "exceptiontree.h"
#include "globals.h"
#include "matrix.h"
#include "stack.h"

using namespace std;

template<class NTYPE, class ETYPE>
tpUnstructured<NTYPE, ETYPE>::tpUnstructured(void) : tpGrid<NTYPE, ETYPE>()
{
  try
  {
    this->SetObjectType(otUnstructured);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::tpUnstructured");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
tpUnstructured<NTYPE, ETYPE>::tpUnstructured(const int ID) : tpGrid<NTYPE, ETYPE>(ID)
{
  try
  {
    this->SetObjectType(otUnstructured);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::tpUnstructured");
    throw EX;
  }
}

//template<class NTYPE, class ETYPE>
//tpUnstructured<NTYPE, ETYPE>::tpUnstructured(const tpUnstructured &source)
//{
//  try
//  {
//  }
//
//  catch(clExceptionTree EX)
//  {
//    EX.AddMethodToTree("tpUnstructured::tpUnstructured");
//    throw EX;
//  }
//}

template<class NTYPE, class ETYPE>
tpUnstructured<NTYPE, ETYPE>::~tpUnstructured(void)
{
  //cerr << "DESTROYING GRID: " << GetNumberID() << endl;
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::DeleteInitialGrid()
{
  // NOTE: It might be best never to delete the Initial Grid
  //       Since if this is done, then in the extreme case, nodes can be added afterwards in elements that have <4 neighbours...

  bool success = true;

  try
  {
    // Delete the initial nodes
    for(int nodeIndex=0; nodeIndex<this->GetNumberNodes(); nodeIndex++)
    {
      NTYPE *node = this->GetNodePointer(nodeIndex);

      if(node->IsHaloNode())
      {
        int nodeID = node->GetNodeID();
        this->DeleteNode(nodeID);
        nodeIndex--;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::DeleteInitialGrid");
    throw EX;
  }

  return(success);
}

template<class NTYPE, class ETYPE>
int tpUnstructured<NTYPE, ETYPE>::FindEnclosingElement(const NTYPE *node)
{
  //
  // IN : pointer to new node
  // OUT: elementID = ID of the element enclosing the new node
  //

  int    elementID = -1;
  int    elementIndex;
  ETYPE *element = NULL;

  try
  {
    //Start with last element
    int returnID         = this->GetLastElementID();
    int endlessLoopCheck = -1;

    while(elementID != returnID)
    {
      if(endlessLoopCheck == returnID)
      {
        // Programme is hanging in an endless loop
        char message[MESLEN];
        sprintf(message, "Programme hanging in an ENDLESS LOOP\n    1ST ELEMENTID = %d\n    2ND ELEMENTID = %d", endlessLoopCheck, elementID);
        throw clExceptionTree("tpUnstructured", "FindEnclosingElement", message);
      }

      endlessLoopCheck = elementID;
      elementID        = returnID;
      elementIndex     = this->GetElementIndex(elementID);
      element          = this->GetElementPointer(elementIndex);

//      cout << " " << elementID;

      if(element->IsHaloCell())
      {
        // Current element is a halo cell, search for neighbour
        int f=0;
        while(f<element->GetNumberFaces() && element->GetNeighbourID(f)==-1)
        {
          f++;
        }

        if(f>=element->GetNumberFaces())
        {
          char message[MESLEN];
          sprintf(message, "Could not find valid neighbour for halo cell.\n    ELEMENTID = %d", element->GetElementID());
          throw clExceptionTree("tpUnstructured", "FindEnclosingElement", message);
        }

        // Return ID of one of the neighbouring tetrahedrons
        returnID = element->GetNeighbourID(f);
      }
      else
      {
        // Check if node is in current element
        returnID = element->IsNodeInElement(node);
      }
    }

    if(element->IsHaloCell())
    {
      char string[25];
      sprintf(string, "Found element is HaloCell\n ID = %d", elementID);
      throw clExceptionTree("tpUnstructured", "FindEnclosingElement", string);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::FindEnclosingElement");
    throw EX;
  }

//  cout << endl;

  return(elementID);
}

template<class NTYPE, class ETYPE>
void tpUnstructured<NTYPE, ETYPE>::FindAffectedElements_Simple(const int theElementID, vector<int> &deleteList, vector<int> &neighbourList, vector<int> &neighbourFaceList)
{
  //
  //  IN : theElementID      = the element which encloses the new node
  //  OUT: deleteList        = list containing the ID's of all affected elements
  //       neighbourList     = list containing the ID's of all new neighbour elements
  //       neighbourFaceList = list containing the entries of the affected neighbour faces.
  //

  try
  {
    // Retrieve the element
    int    theElementIndex = this->GetElementIndex(theElementID);
    ETYPE *theElement      = this->GetElementPointer(theElementIndex);

    // Add element to deleteList
    deleteList.push_back(theElementID);

    // Push current neighbours of element on stack for checking
    for(int f=0; f<theElement->GetNumberFaces(); f++)
    {
      int    theNeighbourID    = theElement->GetNeighbourID(f);
      int    theNeighbourIndex = this->GetElementIndex(theNeighbourID);
      ETYPE* theNeighbour      = this->GetElementPointer(theNeighbourIndex);

      neighbourList.push_back(theNeighbourID);

      for(int ff=0; ff<theNeighbour->GetNumberFaces(); ff++)
      {
        if(theNeighbour->GetNeighbourID(ff)==theElementID)
        {
          neighbourFaceList.push_back(ff);
        }
      }
    }

  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::FindAffectedElements_Simple");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
void tpUnstructured<NTYPE, ETYPE>::FindAffectedElements_Advanced(const int theStartID, NTYPE *theNode, vector<int> &theDeleteList, vector<int> &theNeighbourList, vector<int> &theFaceList)
{
  //
  //  IN : theElementID      = the element which encloses the new node
  //       theNode           = pointer to the new node; for checking the In-Sphere Criterion
  //  OUT: theDeleteList     = list containing the ID's of all affected elements
  //       theNeighbourList  = list containing the ID's of all new neighbour elements
  //       theFaceList       = list containg the indices of the affected neighbour faces (in neighbour system).
  //

  try
  {
    tpStack<int> searchStack;
    tpStack<int> faceStack;

    faceStack.Push(0);
    searchStack.Push(theStartID);
    while(!searchStack.IsEmpty())
    {
      // Pop from stack until stack is empty
      int theFaceIndex = faceStack.Pop();
      int theElementID = searchStack.Pop();

      // Create pointer to the popped element
      int    theElementIndex = this->GetElementIndex(theElementID);
      ETYPE *theElement      = this->GetElementPointer(theElementIndex);

      // Check if the node falls within the circum-sphere of the element
//      if(!theElement->IsHaloCell() && theElement->InSphereCriterion(theNode))
      if(!theElement->IsHaloCell() && ((theElement->GetElementID() == theStartID) || theElement->InSphereCriterion(theNode)))
      {
        // Check if element was already added to deleteList
        vector<int>::const_iterator it = find(theDeleteList.begin(), theDeleteList.end(), theElementID);
        if(it == theDeleteList.end())
        {
          // Not yet added to deleteList; add element to deleteList
          theDeleteList.push_back(theElementID);

          // Push current neighbours of element on stack for checking for In-Sphere criterion
          for(int n=0; n<theElement->GetNumberFaces(); n++)
          {
            int theNeighbourID = theElement->GetNeighbourID(n);
            if(theNeighbourID!=-1)
            {
              int    theNeighbourIndex = this->GetElementIndex(theNeighbourID);
              ETYPE *theNeighbour      = this->GetElementPointer(theNeighbourIndex);

              searchStack.Push(theNeighbourID);
              faceStack.Push(theNeighbour->FindFaceIndex(theElementID));
            }
          }
        }
      }
      else
      {
        // This is an unaffected element (either haloCell or node does not fulfill inSphereCriterion)
        // Check if not already added to lists
        vector<int>::const_iterator it = find(theNeighbourList.begin(), theNeighbourList.end(), theElementID);
        if((it == theNeighbourList.end()) || (*it != theFaceIndex))
        {
          // Add to neighbourList
          theNeighbourList.push_back(theElementID);
          theFaceList.push_back(theFaceIndex);
        }
        else
        {
          // Strange, seems two elements fight for the same neighbour and same neighbour face
          char message[MESLEN];
          sprintf(message, "Duplicate entry in theNeighbourList/theFaceList.\n    ELEMENTID = %d\n    FACEENTRY = %d", theElementID, theFaceIndex);
          throw clExceptionTree("tpUnstructured", "FindAffectedElements", message);
        }
      }
    }

    // Perform a check
    if(theDeleteList.empty())
    {
      char message[MESLEN];
      sprintf(message, "Empty deleteList.\n    ELEMENTID = %d", theStartID);
      throw clExceptionTree("tpUnstructured", "FindAffectedElements", message);
    }

    if(theNeighbourList.empty())
    {
      char message[MESLEN];
      sprintf(message, "Empty neighbourList.\n    ELEMENTID = %d", theStartID);
      throw clExceptionTree("tpUnstructured", "FindAffectedElements", message);
    }

    if(theFaceList.empty())
    {
      char message[MESLEN];
      sprintf(message, "Empty faceList.\n    ELEMENTID = %d", theStartID);
      throw clExceptionTree("tpUnstructured", "FindAffectedElements", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::FindAffectedElements");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
int tpUnstructured<NTYPE, ETYPE>::InsertNode(NTYPE *theNode, bool checkForDuplicateNodes = true)
{
  //
  // IN    : theNode   = the new node that must be inserted
  // RETURN: -1 = error
  //          0 = duplicate node; node not added
  //          1 = node creates invalid element; node not added
  //          2 = success; node added
  //

  int theNodeID   = -1;
  int returnValue = 2;

  try
  {
    bool isDuplicateNode = true;
    if(checkForDuplicateNodes)
    {
      isDuplicateNode = IsDuplicateNode(theNode);
    }

    if(isDuplicateNode)
    {
      returnValue = 0;
    }
    else
    {
      vector<int> theDeleteList;    // ID's of all elements to be deleted
      vector<int> theNeighbourList; // ID's of all new neighbour elements
      vector<int> theFaceList;      // Indices of affected neighbour faces

      // Find the affected elements
      int theElementID = FindEnclosingElement(theNode);
      FindAffectedElements_Advanced(theElementID, theNode, theDeleteList, theNeighbourList, theFaceList);

#ifdef __DEBUG__
      cout << endl << "-------------------" << endl;
      cout << "Inserting Node: " << theNode->GetNodeID() << endl;
      cout << "Primary suspected element: " << theElementID << endl;

      cout << "Number of elements to be deleted: " << theDeleteList.size() << endl;
      cout << "Elements are:" << endl;
      for(unsigned int i=0; i<theDeleteList.size(); i++) cout << theDeleteList[i] << " ";
      cout << endl << endl;

      cout << "Number of neighbour elements: " << theNeighbourList.size() << endl;
      cout << "Elements are:" << endl;
      for(unsigned int j=0; j<theNeighbourList.size(); j++) cout << theNeighbourList[j] << " ";
      cout << endl << endl;

      cout << "Faces are:" << endl;
      for(unsigned int k=0; k<theFaceList.size(); k++) cout << theFaceList[k] << " ";
      cout << endl << endl;
#endif

      //
      // Add the new node
      //
      theNodeID = theNode->GetNodeID();
      if(this->AddNode(theNode) != theNodeID)
      {
        cerr << "Warning: NodeID changed while inserting node..." << endl;
        returnValue = -1;
      }

      //
      // Connect to the neighbourNodes, create the new elements
      //
      vector<int> theNewElementIDs;
      for(unsigned int n=0; n<theNeighbourList.size(); n++)
      {
        int     theNeighbourID    = theNeighbourList[n];
        int     theNeighbourIndex = this->GetElementIndex(theNeighbourID);
        ETYPE  *theNeighbour      = this->GetElementPointer(theNeighbourIndex);

        ETYPE  *theElement(CreateElement());
        int     theNumberFaceNodes = theElement->GetNumberFaceNodes();

        int     theFaceNodes[theNumberFaceNodes];
        int     theNeighbourFaceIndex = theFaceList[n];
        theNeighbour->GetFaceNodes(theNeighbourFaceIndex, theFaceNodes);

        // Nodes can already be resolved, so fill
        theElement->SetNodeID(0, theNodeID);            // The first node is set to the new node
        for(int fn=1; fn<=theNumberFaceNodes; fn++)     // The remaining nodes on the face opposite of the new node are equal to the nodes of the neighbour face, but in reverse order
        {
          theElement->SetNodeID(fn, theNeighbour->GetNodeID(theFaceNodes[theNumberFaceNodes-fn]));
        }

        // Add the new element
        int theElementID = this->AddElement(theElement);
        theNewElementIDs.push_back(theElementID);
      }

      //
      // Validate elements
      //
      bool validElements = true;
      for(unsigned int e=0; validElements && (e<theNewElementIDs.size()); e++)
      {
        int    theElementID    = theNewElementIDs[e];
        int    theElementIndex = this->GetElementIndex(theElementID);
        ETYPE *theElement      = this->GetElementPointer(theElementIndex);

        validElements = validElements && theElement->IsValidElement();
      }

      if(!validElements)
      {
        // Undo node insertion
        this->DeleteNode(theNodeID); // Also deletes elements just created
        returnValue = 1;
      }
      else
      {
        //
        // Connect to the neighbourElements
        //
        for(unsigned int n=0; n<theNeighbourList.size(); n++)
        {
          int     theNeighbourID    = theNeighbourList[n];
          int     theNeighbourIndex = this->GetElementIndex(theNeighbourID);
          ETYPE  *theNeighbour      = this->GetElementPointer(theNeighbourIndex);

          int     theElementID      = theNewElementIDs[n];
          int     theElementIndex   = this->GetElementIndex(theElementID);
          ETYPE  *theElement        = this->GetElementPointer(theElementIndex);

          // Only 1 neighbour element is known, so rest is set to -1
          theElement->SetNeighbourID(0, theNeighbourID);
          for(int nn=1; nn<theElement->GetNumberFaceNodes(); nn++)
          {
            theElement->SetNeighbourID(nn, -1);
          }

          // Make neighbour refer to the new element
          int theNeighbourFaceIndex = theFaceList[n];
          theNeighbour->SetNeighbourID(theNeighbourFaceIndex, theElementID);

#ifdef __DEBUG__
          cout << "Connecting " << theElementID << ".0 to element " << theNeighbourID << "." << theNeighbourFaceIndex << endl;
#endif
        }

        //
        // Connect to the new Elements
        //
        ConnectNewElements(theNewElementIDs);

        //
        // Delete all indicated elements
        //
        while(!theDeleteList.empty())
        {
          this->DeleteElement(*theDeleteList.begin());
          theDeleteList.erase(theDeleteList.begin());
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::InsertNode");
    throw EX;
  }

  return(returnValue);
}

template<class NTYPE, class ETYPE>
void tpUnstructured<NTYPE, ETYPE>::ConnectNewElements(vector<int> &newElementIDs)
{
  //
  // IN : newElementIDs = pointer to list containing all newly created element
  // OUT: -
  //

  try
  {
    while(!newElementIDs.empty())
    {
      int    activeID      = *newElementIDs.begin();
      int    activeIndex   = this->GetElementIndex(activeID);
      ETYPE* activeElement = this->GetElementPointer(activeIndex);

      // Clear from being haloCell
      activeElement->ResetHaloCell();

      // Check the number of already connected faces
//      cout << endl;

      int numberConnected = 0;
      for(int f=0; f<activeElement->GetNumberFaces() ; f++)
      {
        if(activeElement->GetNeighbourID(f)!=-1)
        {
#ifdef __DEBUG__
          cout << activeID << "." << f << " connected to " << activeElement->GetNeighbourID(f) << "     \t[EXISTING CONNECTION]" << endl;
#endif

          numberConnected++;
        }
      }

      // Delete element from list; not required anymore
      newElementIDs.erase(newElementIDs.begin());

      // Loop through remaining list to find neighbour
      unsigned int  i      = 0;
      bool          succes = false;
      while(i<newElementIDs.size() && numberConnected<activeElement->GetNumberFaces())
      {
        int    passiveID      = newElementIDs[i];
        int    passiveIndex   = this->GetElementIndex(passiveID);
        ETYPE *passiveElement = this->GetElementPointer(passiveIndex);

        // Find neighbour face connnection
        int activeFace  = 0;
        int passiveFace = 0;
        succes = FindFaceConnection(activeID, passiveID, activeFace, passiveFace);

        if(succes && activeElement->GetNeighbourID(activeFace)==-1)
        {
          // Establish the connection
          activeElement->SetNeighbourID(activeFace, passiveID);
          passiveElement->SetNeighbourID(passiveFace, activeID);

#ifdef __DEBUG__
          cout << activeID << "." << activeFace << " connected to " << passiveID << "." << passiveFace << "     \t[NEW CONNECTION]" << endl;
#endif

          numberConnected++;
        }

        i++;
      }

      if(numberConnected<activeElement->GetNumberFaces())
      {
        char message[MESLEN];
#ifdef __DEBUG__
        for(int i=0; i<activeElement->GetNumberFaces(); i++) cout << "FACE " << i<< " CONNECTED TO " << activeElement->GetNeighbourID(i) << endl;
#endif

        sprintf(message, "Could not establish a connection.\n    ELEMENTID = %d", activeID);
        throw clExceptionTree("tpUnstructured", "ConnectNewElements", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::ConnectNewElements");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::FindFaceConnection(const int masterID, const int slaveID, int &masterFaceIndex, int &slaveFaceIndex)
{
  //
  // IN : masterID   = ID of master element
  //      slaveID    = ID of slave element
  // OUT: masterFace = face entry of master element referring to slave element
  //      slaveFace  = face entry of slave element referring to master element
  // OUT: ret        = TRUE if a connection was found
  //                   FALSE if no connection was found
  //

  bool ret = false;

  try
  {
    // Find the element entries
    int masterIndex = this->GetElementIndex(masterID);
    int slaveIndex  = this->GetElementIndex(slaveID);

    // Retrieve the elements
    ETYPE *masterElement = this->GetElementPointer(masterIndex);
    ETYPE *slaveElement  = this->GetElementPointer(slaveIndex);

    // Check on number of face nodes
    if(slaveFaceIndex>=slaveElement->GetNumberFaces() || masterFaceIndex>=masterElement->GetNumberFaces())
    {
      char message[MESLEN];
      sprintf(message, "Invalid face(s) found.\n    MASTERFACE = %d\n    SLAVEFACE  = %d", masterFaceIndex, slaveFaceIndex);
      throw clException("tpUnstructured", "FindFaceConnection", message);
    }

    // Loop through all faces of the master element
    masterFaceIndex=0;
    while(masterFaceIndex<masterElement->GetNumberFaces() && !ret)
    {
      // Loop through all faces of the slave element
      slaveFaceIndex=0;
      while(slaveFaceIndex<slaveElement->GetNumberFaces() && !ret)
      {
        // Check if the master face is equal to the slave face
        ret = this->IsEqualFace(masterElement, masterFaceIndex, slaveElement, slaveFaceIndex);

        slaveFaceIndex++;
      }

      masterFaceIndex++;
    }
    slaveFaceIndex--;
    masterFaceIndex--;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::FindFaceConnection");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
int tpUnstructured<NTYPE, ETYPE>::CorrectGrid(void)
{
  // TODO: Correct Grid adds nodes outside the cube in cube.geo...
  int numberCorrections = 0;

  try
  {
    numberCorrections += CorrectInSphereCriterion();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::CorrectGrid");
    throw EX;
  }

  return numberCorrections;
}

template<class NTYPE, class ETYPE>
int tpUnstructured<NTYPE, ETYPE>::CorrectInSphereCriterion(void)
{
  int numberCorrections = 0;

  try
  {
    int e = 0;
    while(e<this->GetNumberElements())
    {
      ETYPE* element = this->GetElementPointer(e);

      // Loop through all nodes
      for(int n=0; n<this->GetNumberNodes(); n++)
      {
        NTYPE* node = this->GetNodePointer(n);

//        cout << "Checking node " << node->GetNodeID() << " against element " << element->GetElementID() << endl;

        bool repairElement = true;
        repairElement = repairElement && !element->IsHaloCell();
        repairElement = repairElement && !element->IsElementNode(node->GetNodeID());
        repairElement = repairElement &&  element->InSphereCriterion(node);

        if(repairElement)
        {
          NTYPE    centroid  = element->GetCentroid();
          NTYPE   *lastNode  = this->GetNodePointer(this->GetNumberNodes()-1);
          int      newNodeID = lastNode->GetNodeID()+1;

          NTYPE    newNode(newNodeID, centroid.GetCoordinates());
          InsertNode(newNode);

          // Debugging
//          cout << "Corrected element: " << element->GetElementID() << endl;

          // Increment counter
          numberCorrections++;

          // Restart at beginning
          e = -1;
          n = this->GetNumberNodes();
        }
      }

      e++;
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::CorrectInSphereCriterion");
    throw EX;
  }

  return numberCorrections;
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::CheckGrid(void)
{
  bool ret=true;

  try
  {
    // Check initial grid
    ret = ret && tpGrid<NTYPE, ETYPE>::CheckGrid();
    ret = ret && this->CheckElementVolumes();
    ret = ret && this->CheckInSphereCriterion();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::CheckGrid");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::CheckElementVolumes(void)
{
  bool ret=true;

  try
  {
    // Loop through all elements
    for(int e=0; e<this->GetNumberElements(); e++)
    {
      ETYPE* theElement = this->GetElementPointer(e);

      if(!theElement->IsHaloCell() && theElement->GetVolume()<=PRECISION)
      {
        ret=false;

        char message[MESLEN];
        sprintf(message, "Detected element with zero or negative volume.\n    ELEMENTID = %d", theElement->GetElementID());
        throw clExceptionTree("tpUnstructured", "CheckElementVolumes", message);
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::CheckElementVolumes");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::CheckInSphereCriterion(void)
{
  bool ret=true;

  try
  {
    // Loop through all elements
    for(int e=0; e<this->GetNumberElements(); e++)
    {
      ETYPE* element = this->GetElementPointer(e);

      // Check if element is not haloCell
      if(!element->IsHaloCell())
      {
        // Loop through all nodes
        for(int n=0; n<this->GetNumberNodes(); n++)
        {
          NTYPE* node = this->GetNodePointer(n);

          // Check node is not haloNode
          if(!node->IsHaloNode())
          {
            // Check node is not part of element
            int  i             = 0;
            bool isElementNode = false;
            while(i<element->GetNumberNodes() && !isElementNode)
            {
              if(element->GetNodeID(i) == node->GetNodeID())
              {
                isElementNode = true;
              }

              i++;
            }

            if(!isElementNode && element->InSphereCriterion(node))
            {
              ret=false;

              char message[MESLEN];
              sprintf(message, "Detected node breaching in-sphere criterion.\n    ELEMENTID = %d\n    NODEID      = %d", element->GetElementID(), node->GetNodeID());
              throw clExceptionTree("tpUnstructured", "CheckInSphereCriterion", message);
            }
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::CheckInSphereCriterion");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::CheckTotalVolume(void)
{
  bool   ret=true;
  double totalVolume = 0.0;

  try
  {
    for(int e=0; e<this->GetNumberElements(); e++)
    {
      ETYPE *element = this->GetElementPointer(e);
      if(!element->IsHaloCell())
      {
        totalVolume = totalVolume + element->GetVolume();
      }
    }

    // TODO: The total Volume is dependend on the size of the initial grid
    if(fabs(totalVolume - (double)(64)) > PRECISION)
    {
      ret=false;

      char message[MESLEN];
      sprintf(message, "Total volume incorrect.\n    VOLUME = %f [64.00]", totalVolume);
      throw clExceptionTree("clUnstructured3D", "CheckTotalVolume", message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::CheckTotalVolume");
    throw EX;
  }

  return(ret);
}

template<class NTYPE, class ETYPE>
void tpUnstructured<NTYPE, ETYPE>::RandomCentroidInsertion(const int numberInsertions)
{
  try
  {
    srand((unsigned)time(NULL));
    //srand((unsigned)0);

    for(int i=0; i<numberInsertions; i++)
    {
      // Select a random tetrahedron - but not a halo tetrahedron
      int index = rand() % this->GetNumberElements();

      ETYPE *theElement = this->GetElementPointer(index);
      if(!theElement->IsHaloCell())
      {
        NTYPE  centroid  = theElement->GetCentroid();
        NTYPE *lastNode  = this->GetNodePointer(this->GetNumberNodes()-1);
        int    newNodeID = lastNode->GetNodeID()+1;

        NTYPE *newNode = new NTYPE(newNodeID, centroid.GetCoordinates());
        this->InsertNode(newNode);
      }
      else
      {
        // Retry randomisation
        i--;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::RandomCentroidInsertion");
    throw EX;
  }
}

template<class NTYPE, class ETYPE>
bool tpUnstructured<NTYPE, ETYPE>::IsDuplicateNode(const NTYPE *nodeA)
{
  bool isDuplicateNode = false;
  try
  {
    for(int n=0; n<this->GetNumberNodes() && !isDuplicateNode; n++)
    {
      NTYPE *nodeB = this->GetNodePointer(n);
      isDuplicateNode = nodeA->IsEqual(*nodeB);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("tpUnstructured::IsDuplicateNode");
    throw EX;
  }

  return (isDuplicateNode);
}

#endif
