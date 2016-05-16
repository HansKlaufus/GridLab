/*****************************************
*** Declaration of class clParentFrame
***
*** Header File
***
*** J.L. Klaufus - Cranfield 1999
**/

#ifndef PARENTFRAME_H
#define PARENTFRAME_H

#include <wx/aui/aui.h>
#include <wx/treectrl.h>

#include "geometry.h"
#include "geometryframe.h"
#include "gui.h"
#include "surfacegrid.h"
#include "surfacegridframe.h"
#include "unstructured3d.h"
#include "volumegridframe.h"

class clParentFrame: public clMainFrame
{
private:
  clGeometryFrame    *geometryFrame;
  clSurfaceGridFrame *surfaceGridFrame;
  clVolumeGridFrame  *volumeGridFrame;

//  tpList<clGeometry>       geometryList;
//  tpList<clSurfaceGrid>    surfaceGridList;
//  tpList<clUnstructured3D> volumeGridList;

  clGeometry        *geometry;
  clSurfaceGrid     *surfaceGrid;
  clUnstructured3D  *volumeGrid;

  void CreateGeometryFrame(wxString);
  void CreateSurfaceGridFrame(wxString);
  void CreateVolumeGridFrame(wxString);
  void CreateTree(wxString);

  void OnNewFile(wxCommandEvent&);
  void OnOpenFile(wxCommandEvent&);
  void OnSaveFile(wxCommandEvent&);
  void OnCloseFile(wxCommandEvent&);
  void OnPrint(wxCommandEvent&);
  void OnPrintSetup(wxCommandEvent&);
  void OnPageSetup(wxCommandEvent&);
  void OnPreview(wxCommandEvent&);
  void OnQuit(wxCommandEvent&);
  void OnSurfaceGrid(wxCommandEvent&);
  void OnBoundaryGrid(wxCommandEvent&);
  void OnVolumeGrid(wxCommandEvent&);
  void OnDumpGrid(wxCommandEvent&);
  void OnAbout(wxCommandEvent&);

  void OnSize(wxSizeEvent&);
  void OnExpandTree(wxTreeEvent&);
  void OnSelectionChanged(wxTreeEvent&);

  void RandomNodeInsertion(const int);
  void RandomCircumCentreInsertion(const int);

public:
  clParentFrame(wxWindow*, const wxWindowID, const wxString&, const wxPoint&, const wxSize&, const long);

  ~clParentFrame(void);

  void CloseFile(void);
  void Report(std::string);
  void Report(wxString);
};

#endif
