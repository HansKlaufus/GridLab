/*********************************************
*** Definition of class clVolumeGridFrame
***
*** Body File
***
*** J.L. Klaufus - Haarlem 2004
**/

#include <vector>
#include <wx/wx.h>

//#include "mywx.h"
#include "array.h"
#include "canvas.h"
#include "macro.h"
#include "matrix.h"
#include "node3d.h"
#include "parentframe.h"
#include "tetrahedron.h"
#include "unstructured3d.h"
#include "volumegridframe.h"

/*
** Definition of the Constructor and Destructor
*/
clVolumeGridFrame::clVolumeGridFrame(wxAuiNotebook *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style, clUnstructured3D *volGrid):
                            wxPanel(parent, wxID_ANY, pos, size, style, title)
{
  canvas               = NULL;
  volumeGrid           = volGrid;
  parentFrame          = parent;
  highLightObjectType  = hlNone;
  SetUpCanvas();
}

clVolumeGridFrame::~clVolumeGridFrame(void)
{
  if(canvas)           delete canvas;

// The following items are managed by parentFrame and should NOT be deleted
//  if(volumeGrid)       delete volumeGrid;
//  if(highLightObjects) delete highLightObjects;
}

/*
** Methods for drawing
*/
void clVolumeGridFrame::BuildVertexArray(std::vector<GLfloat> *vertexArray, std::vector<int> *highLightArray) const
{
  try
  {
    int numVertices = 0;

    // For each tetrahedron in the volumeGrid
    for(int theElementIndex=0; theElementIndex<volumeGrid->GetNumberElements(); theElementIndex++)
    {
      clTetrahedron *theElement = volumeGrid->GetElementPointer(theElementIndex);
      if(!theElement->IsHaloCell())
      {
        // For each face in the element
        for(int theFaceIndex=0; theFaceIndex<theElement->GetNumberFaces(); theFaceIndex++)
        {
          // Get the indices of the nodes
          int theFaceNodes[theElement->GetNumberFaceNodes()];
          theElement->GetFaceNodes(theFaceIndex, theFaceNodes);

          // For each node in the face
          for(int theFaceNodeIndex=0; theFaceNodeIndex<theElement->GetNumberFaceNodes(); theFaceNodeIndex++)
          {
            int       theNodeEntry = theFaceNodes[theFaceNodeIndex];
            int       theNodeID    = theElement->GetNodeID(theNodeEntry);
            int       theNodeIndex = volumeGrid->GetNodeIndex(theNodeID);
            clNode3D *theNode      = volumeGrid->GetNodePointer(theNodeIndex);

            // Retrieve the node from the volumeGrid
            vertexArray->push_back(theNode->x());
            vertexArray->push_back(theNode->y());
            vertexArray->push_back(theNode->z());

            // Set highlighting
            switch(highLightObjectType)
            {
              case hlElement:
                if(theElementIndex == highLightObjects[0])
                  highLightArray->push_back(numVertices);
                break;

              case hlFace:
                if((theElementIndex == highLightObjects[0]) && (theFaceIndex == highLightObjects[1]))
                  highLightArray->push_back(numVertices);
                break;

              case hlNode:
                //TODO: Add hlNode action
                break;

              case hlNone:
                //TODO: Add hlNone action
                break;
            }

            numVertices++;
          }
        }
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clUnstructured3D::BuildVertexArray");
    throw(EX);
  }
}

void clVolumeGridFrame::SetUpCanvas(void)
{
  try
  {
    GLfloat xMin = (GLfloat)(-20);
    GLfloat xMax = (GLfloat)(20);
    GLfloat yMin = (GLfloat)(-20);
    GLfloat yMax = (GLfloat)(20);
    GLfloat zMin = (GLfloat)(-20);
    GLfloat zMax = (GLfloat)(20);

    std::vector<GLfloat> *vertexArray    = new std::vector<GLfloat>;
    std::vector<int>     *highLightArray = new std::vector<int>;
    BuildVertexArray(vertexArray, highLightArray);

    /*
    ** Initialise the canvas
    */
    wxBoxSizer* sizer;
    sizer = new wxBoxSizer( wxHORIZONTAL );

    canvas = new clCanvas(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("clCanvas"), dsWireFrame, GL_TRIANGLES);
    sizer->Add( canvas, 1, wxEXPAND | wxALL, 5 );
    SetSizer(sizer);

    canvas->SetMaxima(xMin, xMax, yMin, yMax, zMin, zMax);
    canvas->SetRotations(-30.0f, 30.0f, 0.0f);
    canvas->SetTranslations( -0.5*(xMin+xMax), -0.5*(yMin+yMax), -0.5*(zMin+zMax));
    canvas->SetScaleFactor(1.0f);
    canvas->SetVertices(vertexArray, highLightArray);

    Show();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clVolumeGridFrame::SetUpCanvas");
    throw(EX);
  }
}

void clVolumeGridFrame::RedrawCanvas(void)
{
  try
  {
    std::vector<GLfloat> *vertexArray    = new std::vector<GLfloat>;
    std::vector<int>     *highLightArray = new std::vector<int>;
    BuildVertexArray(vertexArray, highLightArray);

    canvas->SetVertices(vertexArray, highLightArray);
    canvas->Refresh();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clVolumeGridFrame::RedrawCanvas");
    throw(EX);
  }
}

void clVolumeGridFrame::SetHighLight(enHighLight theHighlightedObject, int *theObjectEntries)
{
  try
  {
    highLightObjectType = theHighlightedObject;
    highLightObjects    = theObjectEntries;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clVolumeGridFrame::SetHighLight");
    throw(EX);
  }
}

/*
** methods for events
*/
void clVolumeGridFrame::OnClose(wxCommandEvent& event)
{
  Close(TRUE);
}

void clVolumeGridFrame::OnActivate(wxActivateEvent& event)
{
  if (event.GetActive() && canvas)
  {
    canvas->SetCurrent();
  }
}
