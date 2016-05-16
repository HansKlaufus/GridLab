/*****************************************
*** Declaration of class clSurfaceGridFrame
***
*** Header File
***
*** J.L. Klaufus - Haarlem 2009
**/

#ifndef SURFACEGRIDFRAME_H
#define SURFACEGRIDFRAME_H

#include <vector>

#include "canvas.h"
#include "enumerations.h"
#include "surfacegrid.h"

class clSurfaceGridFrame: public wxPanel
{
private:
  wxAuiNotebook    *parentFrame;

  clCanvas         *canvas;
  clSurfaceGrid    *surfaceGrid;

  enHighLight       highLightObjectType;
  int              *highLightObjects;

  void              BuildVertexArray(std::vector<GLfloat>*, std::vector<int>*) const;
  void              CreateMenuBar(void);
  void              SetUpCanvas(void);

public:
  clSurfaceGridFrame(wxAuiNotebook*, const wxString&, const wxPoint&, const wxSize&, const long, clSurfaceGrid*);
  ~clSurfaceGridFrame(void);

  void RedrawCanvas(void);
  void RedrawCanvas(int);
  void SetHighLight(enHighLight, int*);

  void OnClose(wxCommandEvent&);
  void OnActivate(wxActivateEvent&);
};

#endif
