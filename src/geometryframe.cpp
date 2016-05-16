/*********************************************
*** Definition of class clGeometryFrame
***
*** Body File
***
*** J.L. Klaufus - Cranfield 1999
**/

#include <vector>
#include <wx/wx.h>

//#include "mywx.h"
#include "array.h"
#include "canvas.h"
#include "geometry.h"
#include "geometryframe.h"
#include "macro.h"
#include "matrix.h"
#include "parentframe.h"
#include "patch.h"
#include "surface.h"
#include "surfacegrid.h"

/*
** Definition of the Constructor and Destructor
*/
clGeometryFrame::clGeometryFrame(wxAuiNotebook *parent, const wxString& title, const wxPoint& pos, const wxSize& size, const long style, clGeometry *geo):
                           wxPanel(parent, wxID_ANY, pos, size, style, title)
{
  canvas      = NULL;
  geometry    = geo;
  parentFrame = parent;
  SetUpCanvas();
}

clGeometryFrame::~clGeometryFrame(void)
{
  if(canvas)   delete canvas;

//  geometry managed by parentFrame; do NOT delete
//  if(geometry) delete geometry;
}

/*
** Methods for drawing
*/
void clGeometryFrame::BuildVertexArray(std::vector<GLfloat> *vertexArray, std::vector<int> *highLightArray) const
{
  try
  {
    int numSurf = geometry->GetNumberSurfaces();
    for(int s=0; s<numSurf; s++)
    {
      clSurface *surf = geometry->GetSurfacePointer(s);

      int numPatch = surf->GetNumberPatches();
      for(int p=0; p<numPatch; p++)
      {
        // First point
        clPatch          *patch = surf->GetPatchPointer(p);
        tpMatrix<double> coord  = patch->GetCoordinates(0.0f, 0.0f);

        vertexArray->push_back(coord(1,1));
        vertexArray->push_back(coord(1,2));
        vertexArray->push_back(coord(1,3));

        // Second point
        coord = patch->GetCoordinates(1.0f, 0.0f);
        vertexArray->push_back(coord(1,1));
        vertexArray->push_back(coord(1,2));
        vertexArray->push_back(coord(1,3));

        // Third point
        coord = patch->GetCoordinates(1.0f, 1.0f);
        vertexArray->push_back(coord(1,1));
        vertexArray->push_back(coord(1,2));
        vertexArray->push_back(coord(1,3));

        // Fourth point
        coord = patch->GetCoordinates(0.0f, 1.0f);
        vertexArray->push_back(coord(1,1));
        vertexArray->push_back(coord(1,2));
        vertexArray->push_back(coord(1,3));
      }
    }
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometryFrame::BuildVertexArray");
    throw(EX);
  }
}

void clGeometryFrame::SetUpCanvas(void)
{
  try
  {
    GLfloat xMin = geometry->GetMinX();
    GLfloat xMax = geometry->GetMaxX();
    GLfloat yMin = geometry->GetMinY();
    GLfloat yMax = geometry->GetMaxY();
    GLfloat zMin = geometry->GetMinZ();
    GLfloat zMax = geometry->GetMaxZ();

    std::vector<GLfloat> *vertexArray    = new std::vector<GLfloat>;
    std::vector<int>     *highLightArray = new std::vector<int>;
    BuildVertexArray(vertexArray, highLightArray);

    /*
    ** Initialise the canvas
    */
    wxBoxSizer* sizer;
    sizer = new wxBoxSizer( wxHORIZONTAL );

    canvas = new clCanvas(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _("clCanvas"), dsFilled, GL_QUADS);
    sizer->Add( canvas, 1, wxEXPAND | wxALL, 5 );
    SetSizer(sizer);

    canvas->SetMaxima(xMin, xMax, yMin, yMax, zMin, zMax);
    canvas->SetRotations(-30.0f, 30.0f, 0.0f);
    canvas->SetTranslations( -0.5*(xMin+xMax), -0.5*(yMin+yMax), -0.5*(zMin+zMax));
    canvas->SetScaleFactor(1.0f);
    canvas->SetVertices(vertexArray, highLightArray);

//    canvas->DrawIt();

    Show();
  }

  catch(clExceptionTree EX)
  {
    EX.AddMethodToTree("clGeometryFrame::SetUpCanvas");
    throw(EX);
  }
}

/*
** methods for events
*/
void clGeometryFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
  ((clParentFrame *)parentFrame)->CloseFile();
  Close(TRUE);
}

void clGeometryFrame::OnActivate(wxActivateEvent& event)
{
  if (event.GetActive() && canvas)
  {
    canvas->SetCurrent();
  }
}

