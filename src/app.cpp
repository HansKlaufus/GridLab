/****************************************************************
*** Definition of class clApp
***
*** Body File
***
*** J.L. Klaufus - Cranfield 1999
**/

//#include "mywx.h"
#include <wx/wx.h>

#include "app.h"
#include "macro.h"
#include "parentframe.h"

IMPLEMENT_APP(clApp)

/*
** Initialisation of the Application
*/
bool clApp::OnInit(void)
{
  // Create the main frame window
//  clParentFrame* parentFrame = new clParentFrame((wxFrame *) NULL, -1, (char *) APP_NAME, wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE | wxHSCROLL | wxVSCROLL);
  clParentFrame* parentFrame = new clParentFrame((wxFrame *) NULL, -1, _(APP_NAME), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE | wxHSCROLL | wxVSCROLL);

  // Refer to an application icon
#ifdef __WXMSW__
  parentFrame->SetIcon(wxIcon(_("parent_icn")));
#else
  parentFrame->SetIcon(wxIcon(_("gridLab_xpm")));
#endif

  /*
  ** Show the main window
  */
  parentFrame->Centre(wxBOTH);
  parentFrame->Show(TRUE);
  SetTopWindow(parentFrame);

  return TRUE;
}

