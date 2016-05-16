/*****************************************
*** Definition of class clParentFrame
***
*** Body File
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <wx/wx.h>
#include <wx/treectrl.h>

#include <string>

//#include "mywx.h"
#include "canvas.h"
#include "exceptiontree.h"
#include "geometry.h"
#include "geometryframe.h"
#include "geometryreader.h"
#include "geometrywriter.h"
#include "gui.h"
#include "macro.h"
#include "parentframe.h"
#include "surfacegrid.h"
#include "tetrahedron.h"
#include "treeitemdata.h"
#include "volumegridframe.h"

/*
** Constructor
*/
clParentFrame::clParentFrame(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, const long style):
                             clMainFrame(parent, id, title, pos, size, style)
{
  geometryFrame    = NULL;
  surfaceGridFrame = NULL;
  volumeGridFrame  = NULL;

  geometry    = NULL;
  surfaceGrid = NULL;
  volumeGrid  = NULL;

  // Give it an icon
#ifdef __WXMSW__
  SetIcon(wxIcon("child_icn"));
#else
  SetIcon(wxIcon(_T("gridLab_xpm")));
#endif

  Report("Welcome to gridLab\n\n");
}

clParentFrame::~clParentFrame(void)
{
    if(geometry)    delete geometry;
    if(surfaceGrid) delete surfaceGrid;
    if(volumeGrid)  delete volumeGrid;

    // The following frames are automatically deleted by the AUIManager
//    if(geometryFrame)    delete geometryFrame;
//    if(surfaceGridFrame) delete surfaceGridFrame;
//    if(volumeGridFrame)  delete volumeGridFrame;
}

/*
** Methods for managing the children
*/
void clParentFrame::CreateGeometryFrame(wxString name)
{
  try
  {
    // Create a new child frame, containing a canvas
    geometryFrame = new clGeometryFrame(noteBook, name, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, geometry);
    if(!noteBook->AddPage(geometryFrame, name, true))
    {
      clExceptionTree *EX = new clExceptionTree("clParentFrame", "CreateGeometryFrame", "Failed to add geometryFrame to noteBook");
      throw(EX);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::CreateGeometryFrame");
    throw(EX);
  }
}

void clParentFrame::CreateSurfaceGridFrame(wxString name)
{
  try
  {
    // Create a new child frame, containing a canvas
    surfaceGridFrame = new clSurfaceGridFrame(noteBook, name, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, surfaceGrid);
    if(!noteBook->AddPage(surfaceGridFrame, name, true))
    {
      clExceptionTree *EX = new clExceptionTree("clParentFrame", "CreateSurfaceGridFrame", "Failed to add surfaceGridFrame to noteBook");
      throw(EX);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::CreateVolumeGridFrame");
    throw(EX);
  }
}

void clParentFrame::CreateVolumeGridFrame(wxString name)
{
  try
  {
    // Create a new child frame, containing a canvas
    volumeGridFrame = new clVolumeGridFrame(noteBook, name, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, volumeGrid);
    if(!noteBook->AddPage(volumeGridFrame, name, true))
    {
      clExceptionTree *EX = new clExceptionTree("clParentFrame", "CreateVolumeGridFrame", "Failed to add volumeGridFrame to noteBook");
      throw(EX);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::CreateVolumeGridFrame");
    throw(EX);
  }
}

void clParentFrame::CloseFile(void)
{
  try
  {
    if(geometry)    delete geometry;
    if(surfaceGrid) delete surfaceGrid;
    if(volumeGrid)  delete volumeGrid;

    geometry    = NULL;
    surfaceGrid = NULL;
    volumeGrid  = NULL;

    if(geometryFrame)    geometryFrame->Close();
    if(surfaceGridFrame) surfaceGridFrame->Close();
    if(volumeGridFrame)  volumeGridFrame->Close();

    geometryFrame    = NULL;
    surfaceGridFrame = NULL;
    volumeGridFrame  = NULL;

    treeControl->DeleteAllItems();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::CloseFile");
    throw(EX);
  }
}

void clParentFrame::Report(std::string vString)
{
  wxString vMessage(vString.c_str(), wxConvUTF8);
  Report(vMessage);
}

void clParentFrame::Report(wxString message)
{
  try
  {
    textWindow->AppendText(message);
    wxSafeYield();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::Report");
    throw(EX);
  }
}

void clParentFrame::CreateTree(wxString fileName)
{
  try
  {
    clTreeItemData *rootItemData  = new clTreeItemData(0, itRoot);
    wxTreeItemId   rootID         = treeControl->AddRoot(fileName, -1, -1, rootItemData);

    clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
    wxTreeItemId   dummyID        = treeControl->AppendItem(rootID, _T("DUMMY"), -1, -1, dummyItemData);
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::CreateGeometryTree");
    throw(EX);
  }
}

void clParentFrame::OnExpandTree(wxTreeEvent& event)
{
  try
  {
    wxTreeItemId    itemID   = event.GetItem();
    clTreeItemData *itemData = (clTreeItemData*)(treeControl->GetItemData(itemID));
    enItemType      itemType = itemData->GetItemType();

    if(itemType == itDummy)
    {
      clExceptionTree *EX = new clExceptionTree("clParentFrame", "OnExpandTree", "Item does not have any children");
      throw(EX);
    }
    else
    {
      // Determine the type of the item
      wxTreeItemId    theLastChildID = treeControl->GetLastChild(itemID);
      clTreeItemData *itemData       = (clTreeItemData*)(treeControl->GetItemData(theLastChildID));

      if(itemData->GetItemType() == itDummy)
      {
        // Delete the dummy entry
        treeControl->DeleteChildren(itemID);

        // Determine the type of the item
        clTreeItemData *itemData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
        enItemType      itemType  = itemData->GetItemType();

        switch (itemType)
        {
          case itRoot:
          {
            if(geometry)
            {
              // Add geometry
              int             numberID = geometry->GetNumberID();
              clTreeItemData *itemData = new clTreeItemData(numberID, itGeometry);

              wxString itemText = wxString::Format(_T("Geometry (ID = %d)"), numberID);
              wxTreeItemId    geoID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry in order to display a '+' in fromt of the item.
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(geoID, _T("DUMMY"), -1, -1, dummyItemData);
            }

            if(surfaceGrid)
            {
              // Add surfaceGrid
              int             numberID = surfaceGrid->GetNumberID();
              clTreeItemData *itemData = new clTreeItemData(numberID, itSurfaceGrid);

              wxString        itemText = wxString::Format(_T("SurfaceGrid (ID = %d)"), numberID);
              wxTreeItemId    surfID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry in order to display a '+' in fromt of the item.
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(surfID, _T("DUMMY"), -1, -1, dummyItemData);
            }

            if(volumeGrid)
            {
              // Add volumeGrid
              int             numberID = volumeGrid->GetNumberID();
              clTreeItemData *itemData = new clTreeItemData(numberID, itVolumeGrid);

              wxString itemText = wxString::Format(_T("VolumeGrid (ID = %d)"), numberID);
              wxTreeItemId    volID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry in order to display a '+' in fromt of the item.
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId   dummyID        = treeControl->AppendItem(volID, _T("DUMMY"), -1, -1, dummyItemData);
            }
            break;
          }

          case itGeometry:
          {
            // Add surfaces
            for(int s=0; s<geometry->GetNumberSurfaces(); s++)
            {
              clSurface      *theSurface = geometry->GetSurfacePointer(s);
              int             numberID   = theSurface->GetNumberID();
              clTreeItemData *itemData   = new clTreeItemData(numberID, s, itSurface);

              wxString itemText = wxString::Format(_T("Surface (ID = %d)"), numberID);
              wxTreeItemId    surfID     = treeControl->AppendItem(itemID,itemText, -1, -1, itemData);

              // Add dummy entry
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(surfID, _T("DUMMY"), -1, -1, dummyItemData);
            }
            break;
          }

          case itSurface:
          {
            // Get surface
            clTreeItemData *itemData     = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             surfaceEntry = itemData->GetItemIndex();
            clSurface      *surface      = geometry->GetSurfacePointer(surfaceEntry);

            // Add patches
            for(int p=0; p<surface->GetNumberPatches(); p++)
            {
              clPatch        *thePatch   = surface->GetPatchPointer(p);
              int             numberID   = thePatch->GetNumberID();
              clTreeItemData *itemData   = new clTreeItemData(numberID, p, itPatch);

              wxString itemText = wxString::Format(_T("Patch (ID = %d)"), numberID);
              wxTreeItemId    patchID     = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(patchID, _T("DUMMY"), -1, -1, dummyItemData);
            }
            break;
          }

          case itPatch:
          {
            // Add points
            for(int p=0; p<4; p++)
            {
              clTreeItemData *itemData = new clTreeItemData(p, p, itPoint);

              wxString itemText = wxString::Format(_T("Point (ID = %d)"), p);
              wxTreeItemId    pointID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(pointID, _T("DUMMY"), -1, -1, dummyItemData);
            }
            break;
          }

          case itPoint:
          {
            // Get point
            clTreeItemData *pointData   = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             pointEntry  = pointData->GetItemIndex();

            // Get patch
            wxTreeItemId    patchID     = treeControl->GetItemParent(itemID);
            clTreeItemData *patchData   = (clTreeItemData*)(treeControl->GetItemData(patchID));
            int             patchEntry  = patchData->GetItemIndex();

            // Get surface
            wxTreeItemId    surfaceID    = treeControl->GetItemParent(patchID);
            clTreeItemData *surfaceData  = (clTreeItemData*)(treeControl->GetItemData(surfaceID));
            int             surfaceEntry = surfaceData->GetItemIndex();

            clSurface      *surface     = geometry->GetSurfacePointer(surfaceEntry);
            clPatch        *patch       = surface->GetPatchPointer(patchEntry);

            // Add coordinates
            double u, v;

            switch(pointEntry)
            {
              case 0:
                u=0.0;
                v=0.0;
                break;

              case 1:
                u=1.0;
                v=0.0;
                break;

              case 2:
                u=0.0;
                v=1.0;
                break;

              case 3:
                u=1.0;
                v=1.0;
                break;

              default:
                clExceptionTree *EX = new clExceptionTree("clParentFrame", "OnExpandTree", "Invalid Point Entry.");
                throw(EX);
                break;
            }

            wxString itemText;
            tpMatrix<double> coord = patch->GetCoordinates(u, v);

            itemText = wxString::Format(_T("X = %f"), coord(1,1));
            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);

            itemText = wxString::Format(_T("Y = %f"), coord(1,2));
            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);

            itemText = wxString::Format(_T("Z = %f"), coord(1,3));
            clTreeItemData *zData = new clTreeItemData(2, 2, itValue);
            wxTreeItemId    zID   = treeControl->AppendItem(itemID, itemText, -1, -1, zData);

            break;
          }

          case itVolumeGrid:
          {
            // Add elements
            for(int e=0; e<volumeGrid->GetNumberElements(); e++)
            {
              clTetrahedron theElement = volumeGrid->GetElement(e);
              if(!theElement.IsHaloCell())
              {
                int             numberID     = theElement.GetElementID();
                clTreeItemData *itemData     = new clTreeItemData(numberID, e, itElement3D);

                wxString itemText = wxString::Format(_T("Element (ID = %d)"), numberID);
                wxTreeItemId    elementID    = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

                // Add dummy entry
                clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
                wxTreeItemId    dummyID       = treeControl->AppendItem(elementID, _T("DUMMY"), -1, -1, dummyItemData);
              }
            }

            // Add nodes
            for(int n=0; n<volumeGrid->GetNumberNodes(); n++)
            {
              clNode3D        theNode    = volumeGrid->GetNode(n);
              int             numberID   = theNode.GetNodeID();
              clTreeItemData *itemData   = new clTreeItemData(numberID, n, itNode3D);

              wxString itemText = wxString::Format(_T("Node (ID = %d)"), numberID);
              wxTreeItemId    nodeID     = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(nodeID, _T("DUMMY"), -1, -1, dummyItemData);
            }
            break;
          }

          case itSurfaceGrid:
          {
            // Add elements
            for(int e=0; e<surfaceGrid->GetNumberElements(); e++)
            {
              clTriangle *theElement = surfaceGrid->GetElementPointer(e);
              if(!theElement->IsHaloCell())
              {
                int             numberID     = theElement->GetElementID();
                clTreeItemData *itemData     = new clTreeItemData(numberID, e, itElement2D);

                wxString itemText = wxString::Format(_T("Element (ID = %d)"), numberID);
                wxTreeItemId    elementID    = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

                // Add dummy entry
                clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
                wxTreeItemId    dummyID       = treeControl->AppendItem(elementID, _T("DUMMY"), -1, -1, dummyItemData);
              }
            }

            // Add nodes
//            for(int n=0; n<surfaceGrid->GetNumberNodes(); n++)
//            {
//              clNode3D       *theNode    = surfaceGrid->GetNodePointer(n);
//              int             numberID   = theNode->GetNumberID();
//              clTreeItemData *itemData   = new clTreeItemData(numberID, n, itNode3D);
//
//              wxString        itemText   = wxString::Format("Node (ID = %d)", numberID);
//              wxTreeItemId    nodeID     = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
//
//              // Add dummy entry
//              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
//              wxTreeItemId    dummyID       = treeControl->AppendItem(nodeID, "DUMMY", -1, -1, dummyItemData);
//            }
            break;
          }

          case itElement2D:
          {
            // Get the element
            clTreeItemData *elementData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             elementIndex = elementData->GetItemIndex();
            clTriangle     *theElement   = surfaceGrid->GetElementPointer(elementIndex);

            // Add faces
            for(int f=0; f<theElement->GetNumberFaces(); f++)
            {
              clTreeItemData *itemData = new clTreeItemData(f, f, itFace3D);

              wxString itemText = wxString::Format(_T("Face (IN = %d)"), f);
              wxTreeItemId faceID = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId    dummyID       = treeControl->AppendItem(faceID, _T("DUMMY"), -1, -1, dummyItemData);
            }

            // Add node IDs
            for(int n=0; n<theElement->GetNumberNodes(); n++)
            {
              int             theNodeID = theElement->GetNodeID(n);
              clTreeItemData *itemData  = new clTreeItemData(theNodeID, n, itValue);

              wxString     itemText = wxString::Format(_T("Node (ID = %d)"), theNodeID);
              wxTreeItemId nodeID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
            }

            // Add radius
            double          theRadius  = theElement->GetRadiusCircumcircle();
            clTreeItemData *radiusData = new clTreeItemData(0, 0, itValue);

            wxString itemText;
            itemText = wxString::Format(_T("Radius = %f"), theRadius);
            wxTreeItemId radiusID = treeControl->AppendItem(itemID, itemText, -1, -1, radiusData);

            // Add area
            double          theArea  = theElement->GetArea();
            clTreeItemData *areaData = new clTreeItemData(0, 0, itValue);

            itemText = wxString::Format(_T("Area = %f"), theArea);
            wxTreeItemId volumeID = treeControl->AppendItem(itemID, itemText, -1, -1, areaData);

            // Add circumcentre
            clTreeItemData *circumCentreData = new clTreeItemData(0, 0, itCircumCentre2D);
            wxTreeItemId    circumCentreID   = treeControl->AppendItem(itemID, _T("CircumCentre"), -1, -1, circumCentreData);

            // Add dummy entry
            clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
            wxTreeItemId    dummyID       = treeControl->AppendItem(circumCentreID, _T("DUMMY"), -1, -1, dummyItemData);

            // Add centroid
            clTreeItemData *centroidData = new clTreeItemData(0, 0, itCentroid2D);
            wxTreeItemId    centroidID   = treeControl->AppendItem(itemID, _T("Centroid"), -1, -1, centroidData);

            // Add dummy entry
            clTreeItemData *dummyItemData2 = new clTreeItemData(0, itDummy);
            wxTreeItemId    dummyID2       = treeControl->AppendItem(centroidID, _T("DUMMY"), -1, -1, dummyItemData2);

            break;
          }

          case itElement3D:
          {
            // Get the element
            clTreeItemData *elementData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             elementIndex = elementData->GetItemIndex();
            clTetrahedron  *theElement   = volumeGrid->GetElementPointer(elementIndex);

            // Add faces
            for(int f=0; f<theElement->GetNumberFaces(); f++)
            {
              clTreeItemData *itemData = new clTreeItemData(f, f, itFace3D);

              wxString itemText = wxString::Format(_T("Face (IN = %d)"), f);
              wxTreeItemId faceID = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

              // Add dummy entry
              clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
              wxTreeItemId   dummyID        = treeControl->AppendItem(faceID, _T("DUMMY"), -1, -1, dummyItemData);
            }

            // Add node IDs
            for(int n=0; n<theElement->GetNumberNodes(); n++)
            {
              int theNodeID = theElement->GetNodeID(n);
              clTreeItemData *itemData = new clTreeItemData(theNodeID, n, itValue);

              wxString itemText = wxString::Format(_T("Node (ID = %d)"), theNodeID);
              wxTreeItemId nodeID = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
            }

            // Add radius
            double          theRadius  = theElement->GetRadiusCircumsphere();
            clTreeItemData *radiusData = new clTreeItemData(0, 0, itValue);

            wxString itemText;
            itemText = wxString::Format(_T("Radius = %f"), theRadius);
            wxTreeItemId radiusID = treeControl->AppendItem(itemID, itemText, -1, -1, radiusData);

            // Add volume
            double          theVolume  = theElement->GetVolume();
            clTreeItemData *volumeData = new clTreeItemData(0, 0, itValue);

            itemText = wxString::Format(_T("Volume = %f"), theVolume);
            wxTreeItemId volumeID = treeControl->AppendItem(itemID, itemText, -1, -1, volumeData);

            // Add circumcentre
            clTreeItemData *circumCentreData = new clTreeItemData(0, 0, itCircumCentre2D);
            wxTreeItemId    circumCentreID   = treeControl->AppendItem(itemID, _T("CircumCentre"), -1, -1, circumCentreData);

            // Add dummy entry
            clTreeItemData *dummyItemData = new clTreeItemData(0, itDummy);
            wxTreeItemId   dummyID        = treeControl->AppendItem(circumCentreID, _T("DUMMY"), -1, -1, dummyItemData);

            // Add centroid
            clTreeItemData *centroidData = new clTreeItemData(0, 0, itCentroid2D);
            wxTreeItemId    centroidID   = treeControl->AppendItem(itemID, _T("Centroid"), -1, -1, centroidData);

            // Add dummy entry
            clTreeItemData *dummyItemData2 = new clTreeItemData(0, itDummy);
            wxTreeItemId   dummyID2        = treeControl->AppendItem(centroidID, _T("DUMMY"), -1, -1, dummyItemData2);

            break;
          }

          case itFace2D:
          {
            // Get the element
            wxTreeItemId    theElementID    = treeControl->GetItemParent(itemID);
            clTreeItemData *theElementData  = (clTreeItemData*)(treeControl->GetItemData(theElementID));
            int             theElementIndex = theElementData->GetItemIndex();
            clTriangle     *theElement      = surfaceGrid->GetElementPointer(theElementIndex);

            // Get the face
            clTreeItemData *theFaceData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             theFaceIndex = theFaceData->GetItemIndex();

            // Add neighbour ID
            int             theNeighbourID = theElement->GetNeighbourID(theFaceIndex);
            clTreeItemData *itemData       = new clTreeItemData(theNeighbourID, theFaceIndex, itNeighbour);

            wxString itemText      = wxString::Format(_T("Neighbour (ID = %d)"), theNeighbourID);
            wxTreeItemId newItemID = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

            // Add node IDs
            int faceNodes[theElement->GetNumberFaceNodes()];
            theElement->GetFaceNodes(theFaceIndex, faceNodes);
            for(int n=0; n<theElement->GetNumberFaceNodes(); n++)
            {
              int             theNodeID = faceNodes[n];
              clTreeItemData *itemData  = new clTreeItemData(theNodeID, n, itValue);

              wxString     itemText = wxString::Format(_T("Node (IN = %d)"), theNodeID);
              wxTreeItemId nodeID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
            }
            break;
          }

          case itFace3D:
          {
            // Get the element
            wxTreeItemId    theElementID    = treeControl->GetItemParent(itemID);
            clTreeItemData *theElementData  = (clTreeItemData*)(treeControl->GetItemData(theElementID));
            int             theElementIndex = theElementData->GetItemIndex();
            clTetrahedron   theElement      = volumeGrid->GetElement(theElementIndex);

            // Get the face
            clTreeItemData *theFaceData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             theFaceIndex = theFaceData->GetItemIndex();

            // Add neighbour ID
            int             theNeighbourID = theElement.GetNeighbourID(theFaceIndex);
            clTreeItemData *itemData       = new clTreeItemData(theNeighbourID, theFaceIndex, itNeighbour);

            wxString itemText = wxString::Format(_T("Neighbour (ID = %d)"), theNeighbourID);
            wxTreeItemId newItemID = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);

            // Add node IDs
            int faceNodes[theElement.GetNumberFaceNodes()];
            theElement.GetFaceNodes(theFaceIndex, faceNodes);
            for(int n=0; n<theElement.GetNumberFaceNodes(); n++)
            {
              int             theNodeID = faceNodes[n];
              clTreeItemData *itemData  = new clTreeItemData(theNodeID, n, itValue);

              wxString itemText = wxString::Format(_T("Node (IN = %d)"), theNodeID);
              wxTreeItemId nodeID = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
            }
            break;
          }

          case itNode2D:
          {
//            // Get the node
//            clTreeItemData *theNodeData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
//            int             theNodeIndex = theNodeData->GetItemIndex();
//            clNode2D       *theNode      = surfaceGrid->GetNodePointer(theNodeIndex);
//
//            wxString itemText;
//            tpMatrix<double> coord = theNode->GetCoordinates();
//
//            itemText = wxString::Format("X = %f", coord(1,1));
//            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
//            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);
//
//            itemText = wxString::Format("Y = %f", coord(2,1));
//            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
//            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);
//
//            // Add element IDs
//            for(int e=0; e<theNode->GetNumberElements(); e++)
//            {
//              int             theElementID = theNode->GetElementID(e);
//              clTreeItemData *itemData     = new clTreeItemData(theElementID, e, itValue);
//
//              wxString     itemText = wxString::Format("Element (ID = %d)", theElementID);
//              wxTreeItemId nodeID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
//            }
//            break;
          }

          case itNode3D:
          {
            // Get the node
            clTreeItemData *theNodeData  = (clTreeItemData*)(treeControl->GetItemData(itemID));
            int             theNodeIndex = theNodeData->GetItemIndex();
            clNode3D       *theNode      = volumeGrid->GetNodePointer(theNodeIndex);

            wxString itemText;
            tpMatrix<double> coord = theNode->GetCoordinates();

            itemText = wxString::Format(_T("X = %f"), coord(1,1));
            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);

            itemText = wxString::Format(_T("Y = %f"), coord(2,1));
            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);

            itemText = wxString::Format(_T("Z = %f"), coord(3,1));
            clTreeItemData *zData = new clTreeItemData(2, 2, itValue);
            wxTreeItemId    zID   = treeControl->AppendItem(itemID, itemText, -1, -1, zData);

            // Add element IDs
            for(int e=0; e<theNode->GetNumberElements(); e++)
            {
              int             theElementID = theNode->GetElementID(e);
              clTreeItemData *itemData  = new clTreeItemData(theElementID, e, itValue);

              wxString     itemText = wxString::Format(_T("Element (ID = %d)"), theElementID);
              wxTreeItemId nodeID   = treeControl->AppendItem(itemID, itemText, -1, -1, itemData);
            }
            break;
          }

          case itCircumCentre2D:
          {
            // Get the element
            wxTreeItemId      theElementID    = treeControl->GetItemParent(itemID);
            clTreeItemData   *theElementData  = (clTreeItemData*)(treeControl->GetItemData(theElementID));
            int               theElementIndex = theElementData->GetItemIndex();
            clTriangle       *theElement      = surfaceGrid->GetElementPointer(theElementIndex);
            clNode2D          circumCentre    = theElement->GetCircumCentre();

            wxString itemText;

            itemText = wxString::Format(_T("X = %f"), circumCentre.x());
            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);

            itemText = wxString::Format(_T("Y = %f"), circumCentre.y());
            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);

            break;
          }

          case itCircumCentre3D:
          {
            // Get the element
            wxTreeItemId      theElementID    = treeControl->GetItemParent(itemID);
            clTreeItemData   *theElementData  = (clTreeItemData*)(treeControl->GetItemData(theElementID));
            int               theElementIndex = theElementData->GetItemIndex();
            clTetrahedron    *theElement      = volumeGrid->GetElementPointer(theElementIndex);
            clNode3D          circumCentre    = theElement->GetCircumCentre();

            wxString itemText;

            itemText = wxString::Format(_T("X = %f"), circumCentre.x());
            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);

            itemText = wxString::Format(_T("Y = %f"), circumCentre.y());
            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);

            itemText = wxString::Format(_T("Z = %f"), circumCentre.z());
            clTreeItemData *zData = new clTreeItemData(2, 2, itValue);
            wxTreeItemId    zID   = treeControl->AppendItem(itemID, itemText, -1, -1, zData);

            break;
          }

          case itCentroid2D:
          {
            // Get the element
            wxTreeItemId      theElementID    = treeControl->GetItemParent(itemID);
            clTreeItemData   *theElementData  = (clTreeItemData*)(treeControl->GetItemData(theElementID));
            int               theElementIndex = theElementData->GetItemIndex();
            clTriangle       *theElement      = surfaceGrid->GetElementPointer(theElementIndex);
            clNode2D          centroid        = theElement->GetCentroid();

            wxString itemText;

            itemText = wxString::Format(_T("X = %f"), centroid.x());
            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);

            itemText = wxString::Format(_T("Y = %f"), centroid.y());
            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);

            break;
          }

          case itCentroid3D:
          {
            // Get the element
            wxTreeItemId      theElementID    = treeControl->GetItemParent(itemID);
            clTreeItemData   *theElementData  = (clTreeItemData*)(treeControl->GetItemData(theElementID));
            int               theElementIndex = theElementData->GetItemIndex();
            clTetrahedron    *theElement      = volumeGrid->GetElementPointer(theElementIndex);
            clNode3D          centroid        = theElement->GetCentroid();

            wxString itemText;

            itemText = wxString::Format(_T("X = %f"), centroid.x());
            clTreeItemData *xData = new clTreeItemData(0, 0, itValue);
            wxTreeItemId    xID   = treeControl->AppendItem(itemID, itemText, -1, -1, xData);

            itemText = wxString::Format(_T("Y = %f"), centroid.y());
            clTreeItemData *yData = new clTreeItemData(1, 1, itValue);
            wxTreeItemId    yID   = treeControl->AppendItem(itemID, itemText, -1, -1, yData);

            itemText = wxString::Format(_T("Z = %f"), centroid.z());
            clTreeItemData *zData = new clTreeItemData(2, 2, itValue);
            wxTreeItemId    zID   = treeControl->AppendItem(itemID, itemText, -1, -1, zData);

            break;
          }

          default:
          {
            clExceptionTree *EX = new clExceptionTree("clParentFrame", "OnExpandTree", "Unknown item type.");
            throw(EX);

            break;
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    std::string vString = "";

    EX.AddMethodToTree("clParentFrame::OnExpandTree");
    vString = EX.PrintError(vString);
    Report(vString);
  }
}

void clParentFrame::OnSelectionChanged(wxTreeEvent& event)
{
  try
  {
    int highLightObjects[3];

    wxTreeItemId   itemID = event.GetItem();
    clTreeItemData *itemData     = (clTreeItemData*)(treeControl->GetItemData(itemID));
    enItemType     itemType      = itemData->GetItemType();
    int            itemIndex     = itemData->GetItemIndex();

    switch (itemType)
    {
      case itElement3D:
      {
        highLightObjects[0] = itemIndex;
        highLightObjects[1] = -1;
        highLightObjects[2] = -1;

        volumeGridFrame->SetHighLight(hlElement, highLightObjects);
        volumeGridFrame->RedrawCanvas();
        break;
      }

      case itFace3D:
      {
        wxTreeItemId   theParentID = treeControl->GetItemParent(itemID);
        clTreeItemData *parentData = (clTreeItemData*)(treeControl->GetItemData(theParentID));
        int            parentIndex = parentData->GetItemIndex();

        highLightObjects[0] = parentIndex;
        highLightObjects[1] = itemIndex;
        highLightObjects[2] = -1;

        volumeGridFrame->SetHighLight(hlFace, highLightObjects);
        volumeGridFrame->RedrawCanvas();
        break;
      }

      case itNeighbour:
      {
        int elementID    = itemData->GetItemID();
        int elementIndex = volumeGrid->GetElementIndex(elementID);

        highLightObjects[0] = elementIndex;
        highLightObjects[1] = -1;
        highLightObjects[2] = -1;

        volumeGridFrame->SetHighLight(hlElement, highLightObjects);
        volumeGridFrame->RedrawCanvas();
        break;
      }

      default:
      {
        highLightObjects[0] = -1;
        highLightObjects[1] = -1;
        highLightObjects[2] = -1;

        if(volumeGridFrame)
        {
          volumeGridFrame->SetHighLight(hlNone, highLightObjects);
          volumeGridFrame->RedrawCanvas();
        }
        break;
      }
    }
  }

  catch(clExceptionTree EX)
  {
    std::string vString = NULL;

    EX.AddMethodToTree("clParentFrame::OnSelectionChanged");
    vString = EX.PrintError(vString);
    Report(vString);
  }
}


/*
** Events
*/
void clParentFrame::OnNewFile(wxCommandEvent& event)
{
  CloseFile();

  Report("Sorry; facilities for new geometry not implemented yet...\n");
}

void clParentFrame::OnOpenFile(wxCommandEvent& event)
{
  try
  {
    wxString fileName = wxFileSelector(_T("Open file"), NULL, NULL, _T("*.geo"), _T("Geometry files (*.geo)|*.geo|GTS files (*.gts)|*.gts|Wavefront OBJ files (*.obj)|*.obj|Dump files (*.dmp)|*.dmp|All files (*)|*"), wxOPEN, this, -1, -1);
    if(!fileName.IsEmpty())
    {
      CloseFile();

      // Return message in textWindow
      wxString message;
      message = _T("Reading file ") + wxFileNameFromPath(fileName) + _T("...\n");
      Report(message);

      wxString fileExtention = fileName.Mid(fileName.Find('.', TRUE));
      if(fileExtention.CmpNoCase(_T(".geo"))==0)
      {
        geometry = new clGeometry();
        clGeometryReader reader((const char*)fileName.mb_str(), *geometry);

        Report("Building Geometry Frame...\n");
        CreateGeometryFrame(_T("Geometry"));

        Report("Building Tree...\n");
        CreateTree(wxFileNameFromPath(fileName));
      }
      else if(fileExtention.CmpNoCase(_T(".gts"))==0)
      {
        surfaceGrid = new clSurfaceGrid(0);
        surfaceGrid->ReadGTSGrid(fileName.mb_str());

        Report("Building Surface Frame...\n");
        CreateSurfaceGridFrame(_T("Surface Grid"));
      }
      else if(fileExtention.CmpNoCase(_T(".obj"))==0)
      {
        surfaceGrid = new clSurfaceGrid(0);
        surfaceGrid->ReadOBJGrid(fileName.mb_str());

        Report("Building Surface Frame...\n");
        CreateSurfaceGridFrame(_T("Surface Grid"));
      }
      else if(fileExtention.CmpNoCase(_T(".dmp"))==0)
      {
      }
    }

    Report("Done.\n\n");
  }

  catch(clExceptionTree EX)
  {
    std::string vString = NULL;

    EX.AddMethodToTree("clParentFrame::OnOpenFile");
    vString = EX.PrintError(vString);
    Report(vString);
  }
}

void clParentFrame::OnSaveFile(wxCommandEvent& event)
{
  wxString fileName = wxFileSelector(_T("Save file"), NULL, NULL, _T("geo"), _T("*.geo"), wxSAVE, this, -1, -1);
  if(!fileName.IsEmpty())
  {
    // Return message in textWindow
    wxString message;
    message = _T("Writing file ") + wxFileNameFromPath(fileName) + _T("...\n");
    Report(message);

//    clWriter writer(fileName, foGNU, geometry);

    Report("Done.\n\n");
  }
}

void clParentFrame::OnPrint(wxCommandEvent& event)
{
  Report("Sorry; printing facilities not implemented yet...\n");
}

void clParentFrame::OnPrintSetup(wxCommandEvent& event)
{
  Report("Sorry; printing facilities not implemented yet...\n");
}

void clParentFrame::OnPageSetup(wxCommandEvent& event)
{
  Report("Sorry; printing facilities not implemented yet...\n");
}

void clParentFrame::OnPreview(wxCommandEvent& event)
{
  Report("Sorry; printing facilities not implemented yet...\n");
}

void clParentFrame::OnCloseFile(wxCommandEvent& event)
{
  CloseFile();
}

void clParentFrame::OnQuit(wxCommandEvent& event)
{
  CloseFile();
  Close(TRUE);
}

void clParentFrame::OnSurfaceGrid(wxCommandEvent& event)
{
  try
  {
    if(!surfaceGrid)
    {
      Report("Creating Surface Grid...\n");
      surfaceGrid = new clSurfaceGrid(0);

      Report("Building SurfaceGrid Frame...\n");
      CreateSurfaceGridFrame(_T("Surface Grid"));
    }
    else
    {

      for(int s=0; s<geometry->GetNumberSurfaces(); s++)
      {
        wxString message = wxString::Format(_T("[%3d%%] Applying grid to surface %d...\n"), 100*s/geometry->GetNumberSurfaces(), s);
        Report(message);

        clSurface *surface = geometry->GetSurfacePointer(s);
        surfaceGrid->CreateUnstructured(surface, 7, 7, dtUniform);
        surfaceGridFrame->RedrawCanvas();
      }

//      Report("Correcting grid...\n");
//      surfaceGrid->CorrectGrid();

      Report("Checking grid...\n");
      surfaceGrid->CheckGrid();
    }
  }

  catch(clExceptionTree EX)
  {
    std::string vString = "";

    EX.AddMethodToTree("clParentFrame::OnSurfaceGrid");
    vString = EX.PrintError(vString);
    Report(vString);
  }

  Report("Done.\n\n");
}

void clParentFrame::OnBoundaryGrid(wxCommandEvent& event)
{
  try
  {
    Report("Boundary Grid not implemented yet\n");
  }

  catch(clExceptionTree EX)
  {
    std::string vString = "";

    EX.AddMethodToTree("clParentFrame::OnBoundaryGrid");
    vString = EX.PrintError(vString);
    Report(vString);
  }

  Report("Done.\n\n");
}

void clParentFrame::OnVolumeGrid(wxCommandEvent& event)
{
  try
  {
    if(!volumeGrid)
    {
      double minX = geometry->GetMinX();
      double maxX = geometry->GetMaxX();
      double minY = geometry->GetMinY();
      double maxY = geometry->GetMaxY();
      double minZ = geometry->GetMinZ();
      double maxZ = geometry->GetMaxZ();

      double deltaX = maxX - minX;
      double deltaY = maxY - minY;
      double deltaZ = maxZ - minZ;

      minX = minX - deltaX/2;
      maxX = maxX + deltaX/2;
      minY = minY - deltaY/2;
      maxY = maxY + deltaY/2;
      minZ = minZ - deltaZ/2;
      maxZ = maxZ + deltaZ/2;

      volumeGrid = new clUnstructured3D(minX, maxX, minY, maxY, minZ, maxZ);

      Report("Building VolumeGrid Frame...\n");
      CreateVolumeGridFrame(_T("Volume Grid"));
    }
    else
    {
      if(!surfaceGrid)
      {
        Report("Performing random centroid insertion...\n");
        volumeGrid->RandomCentroidInsertion(50);
      }
      else
      {
        Report("Inserting surfaceGrid nodes...\n");
        for(int n=0; n<surfaceGrid->GetNumberNodes(); n++)
//        static int n =-1;
//        n++;
        {
          clNode3D *surfaceNode = surfaceGrid->GetNodePointer(n);
          dMatrix   coord3d     = surfaceNode->GetCoordinates();
          clNode3D *newNode     = new clNode3D(volumeGrid->GetNewNodeID(), coord3d);

          wxString message = wxString::Format(_T("[%3d%%] Inserting NODE ID = %d (%f, %f, %f)\n"), 100*n/surfaceGrid->GetNumberNodes(), newNode->GetNodeID(), newNode->x(), newNode->y(), newNode->z());
          Report(message);

          volumeGrid->InsertNode(newNode);

          // Redraw canvas each 5% of the total number of nodes
          if(n%(int)(0.05*surfaceGrid->GetNumberNodes())==0) volumeGridFrame->RedrawCanvas();
        }
      }

//      Report("Correcting grid...\n");
//      char message[MESLEN];
//      sprintf(message, "Number of corrections performed: %d\n", volumeGrid->CorrectGrid());
//      Report(message);

      // NOTE: It might be best never to delete the Initial Grid
      //       Since if this is done, then in the extreme case, nodes can be added afterwards in elements that have <4 neighbours...
      Report("Deleting initial grid...\n");
      volumeGrid->DeleteInitialGrid();

      Report("Verifying grid...\n");
      volumeGrid->CheckGrid();

      volumeGridFrame->RedrawCanvas();
    }
  }

  catch(clExceptionTree EX)
  {
    std::string vString = NULL;

    EX.AddMethodToTree("clParentFrame::OnVolumeGrid");
    vString = EX.PrintError(vString);
    Report(vString);
  }

  Report("Done.\n\n");
}

void clParentFrame::RandomNodeInsertion(const int numberInsertions)
{
  try
  {
    static int count = 0;

    //srand((unsigned)time(NULL));
    srand((unsigned)0);

    for(int i=0; i<numberInsertions; i++)
    {
      count++;

      wxString message = wxString::Format(_T("Performing insertion:\t%d\n"), count);
      Report(message);

      double x=rand();
      double y=rand();
      double z=rand();

      x/=RAND_MAX;
      y/=RAND_MAX;
      z/=RAND_MAX;

      message = wxString::Format(_T("Coordinates:\t\tX = %f, Y = %f, Z = %f\n"), x, y, z);
      Report(message);

      clNode3D *lastNode  = volumeGrid->GetNodePointer(volumeGrid->GetNumberNodes()-1);
      int       newNodeID = lastNode->GetNodeID()+1;

      clNode3D *newNode = new clNode3D(newNodeID, x, y, z);
      newNodeID = volumeGrid->InsertNode(newNode);

      message = wxString::Format(_T("New nodeID:\t\t%d\n"), newNodeID);
      Report(message);
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clParentFrame::RandomNodeInsertion");
    throw EX;
  }
}

void clParentFrame::RandomCircumCentreInsertion(const int numberInsertions)
{
  try
  {
    static int count = 0;

    //srand((unsigned)time(NULL));
    srand((unsigned)0);

    for(int i=0; i<numberInsertions; i++)
    {
      // Select a random tetrahedron - but not a halo tetrahedron
      int index = rand() % volumeGrid->GetNumberElements();

      clTetrahedron *theElement = volumeGrid->GetElementPointer(index);
      if(!theElement->IsHaloCell())
      {
        count++;

        wxString message = wxString::Format(_T("Performing insertion:\t%d\n"), count);
        Report(message);

        message = wxString::Format(_T("Using centre of element:\t%d\n"), theElement->GetElementID() );
        Report(message);
        cout << message << endl;

        clNode3D circumCentre = theElement->GetCircumCentre();

        message = wxString::Format(_T("Coordinates:\t\tX = %f, Y = %f, Z = %f\n"), circumCentre.x(), circumCentre.y(), circumCentre.z());
        Report(message);

        // Create a new node in the Centroid
        clNode3D *lastNode  = volumeGrid->GetNodePointer(volumeGrid->GetNumberNodes()-1);
        int       newNodeID = lastNode->GetNodeID()+1;

        clNode3D *newNode = new clNode3D(newNodeID, circumCentre.GetCoordinates());
        newNodeID = volumeGrid->InsertNode(newNode);

        message = wxString::Format(_T("New nodeID:\t\t%d\n"), newNodeID);
        Report(message);

//        char fileName[25];
//        sprintf(fileName, "volumeGrid_%d.csv\0", count);
//        message = wxString::Format("Dumping grid to:\t\t%s\n", fileName);
//        Report(message);
//        volumeGrid->DumpGrid(fileName);
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
    EX.AddMethodToTree("clParentFrame::RandomCircumCentreInsertion");
    throw EX;
  }
}

void clParentFrame::OnDumpGrid(wxCommandEvent& event)
{
  try
  {
    wxString pageName = noteBook->GetPageText(noteBook->GetSelection());

    if (pageName == wxT("Volume Grid"))
    {
      Report("Dumping Volume Grid...\n");
      volumeGrid->WriteGnuplotGrid();
      volumeGrid->DumpGrid();
    }
    else if (pageName == _T("Surface Grid"))
    {
      Report("Dumping Surface Grid...\n");
      surfaceGrid->WriteGnuplotGrid();
      surfaceGrid->DumpGrid();
    }
    else if (pageName == _T("Boundary Grid"))
    {
      Report("TODO: Dumping Boundary Grid...\n");
      // TODO: call boundaryGrid->DumpGrid();
    }
  }

  catch(clExceptionTree EX)
  {
    std::string vString = NULL;

    EX.AddMethodToTree("clParentFrame::OnDumpGrid");
    vString = EX.PrintError(vString);
    Report(vString);
  }

  Report("Done.\n\n");
}

void clParentFrame::OnAbout(wxCommandEvent& event)
{
  wxString vMessage1;
  vMessage1 += _T("Program:\t\t");
  vMessage1 += _T(APP_NAME);
  vMessage1 += _T("\n");
  vMessage1 += _T("Author:\t\t");
  vMessage1 += _T("J.L. Klaufus\n");
  vMessage1 += _T("Place:\t\t");
  vMessage1 += _T("University of Cranfield\n");
  vMessage1 += _T("Date:\t\t");
  vMessage1 += _T("september 1999\n\n");
  vMessage1 += _T("This program has been developped within the context of a final thesis\n");
  vMessage1 += _T("project in partial fullfilment of the requirements for the degree of\n");
  vMessage1 += _T("Master of Science in Aerospace Dynamics, option Computational Fluid Dynamics.\n");

  wxString vMessage2;
  vMessage2 += _T("About ");
  vMessage2 += _T(APP_NAME);

  wxMessageBox(vMessage1, vMessage2);
}

void clParentFrame::OnSize(wxSizeEvent& event)
{
  int w, h;
  GetClientSize(&w, &h);

  int xTextPosition = (int)(0);
  int yTextPosition = (int)((1-TEXT_HEIGHT)*h);
  int xTextSize     = w;
  int yTextSize     = (int)(TEXT_HEIGHT*h);
  textWindow->SetSize(xTextPosition, yTextPosition, xTextSize, yTextSize);

  int xTreePosition = (int)(0);
  int yTreePosition = (int)(0);
  int xTreeSize     = (int)(TREE_WIDTH*w);
  int yTreeSize     = (int)((1-TEXT_HEIGHT)*h-1);
  treeControl->SetSize(xTreePosition, yTreePosition, xTreeSize, yTreeSize);

  int xClientPosition = (int)(TREE_WIDTH*w+1);
  int yClientPosition = (int)(0);
  int xClientSize     = (int)((1-TREE_WIDTH)*w-1);
  int yClientSize     = (int)((1-TEXT_HEIGHT)*h-1);

//  GetClientWindow()->SetSize(xClientPosition, yClientPosition, xClientSize, yClientSize);
//  GetClientWindow()->SetClientSize(xClientSize, yClientSize);
}

