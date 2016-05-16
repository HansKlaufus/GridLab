/*****************************************
*** Declaration of class clVolumeGridFrame
***
*** Header File
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef VOLUMEGRIDFRAME_H
#define VOLUMEGRIDFRAME_H

#include <vector>

#include "canvas.h"
#include "enumerations.h"
#include "parentframe.h"
#include "unstructured3d.h"

class clVolumeGridFrame: public wxPanel
{
private:
  wxAuiNotebook    *parentFrame;
  clUnstructured3D *volumeGrid;

  clCanvas         *canvas;

  enHighLight       highLightObjectType;
  int              *highLightObjects;

  void             BuildVertexArray(std::vector<GLfloat>*, std::vector<int>*) const;
  void             CreateMenuBar(void);
  void             SetUpCanvas(void);

public:
  clVolumeGridFrame(wxAuiNotebook*, const wxString&, const wxPoint&, const wxSize&, const long, clUnstructured3D*);
  ~clVolumeGridFrame(void);

  void RedrawCanvas(void);
  void RedrawCanvas(int);
  void SetHighLight(enHighLight, int*);

  void OnClose(wxCommandEvent&);
  void OnActivate(wxActivateEvent&);
};

#endif
