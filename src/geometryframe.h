/*****************************************
*** Declaration of class clGeometryFrame
***
*** Header File
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef GEOMETRYFRAME_H
#define GEOMETRYFRAME_H

#include <vector>

#include "canvas.h"
#include "geometry.h"
#include "gui.h"

class clGeometryFrame: public wxPanel
{
private:
  wxAuiNotebook    *parentFrame;
  clGeometry       *geometry;

  clCanvas         *canvas;

  void             BuildVertexArray(std::vector<GLfloat>*, std::vector<int>*) const;
  void             SetUpCanvas(void);


public:
  clGeometryFrame(wxAuiNotebook*, const wxString&, const wxPoint&, const wxSize&, const long, clGeometry*);
  ~clGeometryFrame(void);

  void OnClose(wxCommandEvent&);
  void OnActivate(wxActivateEvent&);
};

#endif
