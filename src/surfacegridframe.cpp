/*********************************************
*** Definition of class clSurfaceGridFrame
***
*** Body File
***
*** J.L. Klaufus - Haarlem 2009
**/

#include <vector>
#include <wx/wx.h>

#include "array.h"
#include "canvas.h"
#include "macro.h"
#include "matrix.h"
#include "node2d.h"
#include "parentframe.h"
#include "surfacegrid.h"
#include "surfacegridframe.h"

/*
** Definition of the Constructor and Destructor
*/
clSurfaceGridFrame::clSurfaceGridFrame(wxAuiNotebook *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style, clSurfaceGrid *surfGrid):wxPanel(parent, wxID_ANY, pos, size, style, title)
{
  canvas               = NULL;
  surfaceGrid          = surfGrid;
  parentFrame          = parent;
  highLightObjectType  = hlNone;
  SetUpCanvas();
}

clSurfaceGridFrame::~clSurfaceGridFrame(void)
{
  if(canvas)   delete canvas;

// The following items are managed by parentFrame and should NOT be deleted
//  if(surfaceGrid) delete surfaceGrid;
//  if(highLightObjects) delete highLightObjects;
}

void clSurfaceGridFrame::SetUpCanvas(void)
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
    EX.AddMethodToTree("clSurfaceGridFrame::SetUpCanvas");
    throw(EX);
  }
}

/*
** Methods for drawing
*/
void clSurfaceGridFrame::BuildVertexArray(std::vector<GLfloat> *vertexArray, std::vector<int> *highLightArray) const
{
  try
  {
    std::cout << "Building vertex array..." << std::endl;
    if(!surfaceGrid)
    {
      clException("clSurfaceGridFrame", "BuildVertexArray", "SurfaceGrid not initialised.");
    }
    else
    {
      int numVertices = 0;
      int newProgress = 0;
      int oldProgress = 0;

      // For each triangle in the surfaceGrid
      int numberElements = surfaceGrid->GetNumberElements();
      for(int theElementIndex=0; theElementIndex<numberElements; theElementIndex++)
      {
        // Print progress
        newProgress = theElementIndex*100.0/numberElements;
        if((newProgress-oldProgress)>2)
        {
            std::cout << "Progress: " << newProgress << "%" << std::endl;
            oldProgress = newProgress;
        }

        // Next element
        clTriangle *theElement = surfaceGrid->GetElementPointer(theElementIndex);

        // For each node in the element
        int numberNodes = theElement->GetNumberNodes();
        for(int theNodeIndex=0; theNodeIndex<numberNodes; theNodeIndex++)
        {
          int       theNodeID    = theElement->GetNodeID(theNodeIndex);
          int       theNodeEntry = surfaceGrid->GetNodeIndex(theNodeID);
          clNode3D *theNode      = surfaceGrid->GetNodePointer(theNodeEntry);

          // Retrieve the node from the surfaceGrid
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
              //TODO: Add hlFace action
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

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGridFrame::BuildVertexArray");
    throw(EX);
  }
}

void clSurfaceGridFrame::RedrawCanvas(void)
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
    EX.AddMethodToTree("clSurfaceGridFrame::RedrawCanvas");
    throw(EX);
  }
}

void clSurfaceGridFrame::SetHighLight(enHighLight theHighlightedObject, int *theObjectEntries)
{
  try
  {
    highLightObjectType = theHighlightedObject;
    highLightObjects    = theObjectEntries;
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clSurfaceGridFrame::SetHighLight");
    throw(EX);
  }
}

/*
** methods for events
*/
void clSurfaceGridFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
  Close(TRUE);
}

void clSurfaceGridFrame::OnActivate(wxActivateEvent& event)
{
  if (event.GetActive() && canvas)
  {
    canvas->SetCurrent();
  }
}

