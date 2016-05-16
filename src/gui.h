///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/toolbar.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define MENU_NEW 1000
#define MENU_OPEN 1001
#define MENU_SAVE 1002
#define MENU_PRINT 1003
#define MENU_PREVIEW 1004
#define MENU_ABOUT 1005
#define MENU_CLOSE 1006
#define MENU_QUIT 1007
#define MENU_COPY 1008
#define MENU_PASTE 1009
#define MENU_SURFACEGRID 1010
#define MENU_BOUNDARYGRID 1011
#define MENU_VOLUMEGRID 1012
#define MENU_INSERTNODE 1013
#define MENU_DUMPGRID 1014

///////////////////////////////////////////////////////////////////////////////
/// Class clMainFrame
///////////////////////////////////////////////////////////////////////////////
class clMainFrame : public wxFrame 
{
	DECLARE_EVENT_TABLE()
	private:
		
		// Private event handlers
		void _wxFB_OnNewFile( wxCommandEvent& event ){ OnNewFile( event ); }
		void _wxFB_OnOpenFile( wxCommandEvent& event ){ OnOpenFile( event ); }
		void _wxFB_OnSaveFile( wxCommandEvent& event ){ OnSaveFile( event ); }
		void _wxFB_OnPrint( wxCommandEvent& event ){ OnPrint( event ); }
		void _wxFB_OnPreview( wxCommandEvent& event ){ OnPreview( event ); }
		void _wxFB_OnAbout( wxCommandEvent& event ){ OnAbout( event ); }
		void _wxFB_OnCloseFile( wxCommandEvent& event ){ OnCloseFile( event ); }
		void _wxFB_OnQuit( wxCommandEvent& event ){ OnQuit( event ); }
		void _wxFB_OnSurfaceGrid( wxCommandEvent& event ){ OnSurfaceGrid( event ); }
		void _wxFB_OnBoundaryGrid( wxCommandEvent& event ){ OnBoundaryGrid( event ); }
		void _wxFB_OnVolumeGrid( wxCommandEvent& event ){ OnVolumeGrid( event ); }
		void _wxFB_onInsertNode( wxCommandEvent& event ){ onInsertNode( event ); }
		void _wxFB_OnDumpGrid( wxCommandEvent& event ){ OnDumpGrid( event ); }
		void _wxFB_OnExpandTree( wxTreeEvent& event ){ OnExpandTree( event ); }
		void _wxFB_OnSelectionChanged( wxTreeEvent& event ){ OnSelectionChanged( event ); }
		
	
	protected:
		wxToolBar* mainToolBar;
		wxMenuBar* mainMenuBar;
		wxMenu* fileMenu;
		wxMenu* editMenu;
		wxMenu* gridMenu;
		wxMenu* helpMenu;
		wxStatusBar* mainStatusBar;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel3;
		wxTreeCtrl* treeControl;
		wxPanel* m_panel4;
		wxAuiNotebook* noteBook;
		wxPanel* m_panel2;
		wxTextCtrl* textWindow;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNewFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOpenFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSaveFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPrint( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPreview( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCloseFile( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSurfaceGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnBoundaryGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnVolumeGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void onInsertNode( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDumpGrid( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnExpandTree( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnSelectionChanged( wxTreeEvent& event ){ event.Skip(); }
		
	
	public:
		clMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("gridLab"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~clMainFrame();
		void m_splitter1OnIdle( wxIdleEvent& )
		{
		m_splitter1->SetSashPosition( 0 );
		m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( clMainFrame::m_splitter1OnIdle ), NULL, this );
		}
		
		void m_splitter2OnIdle( wxIdleEvent& )
		{
		m_splitter2->SetSashPosition( 194 );
		m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( clMainFrame::m_splitter2OnIdle ), NULL, this );
		}
		
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class clNodeDialog
///////////////////////////////////////////////////////////////////////////////
class clNodeDialog : public wxDialog 
{
	DECLARE_EVENT_TABLE()
	private:
		
		// Private event handlers
		void _wxFB_OnRandomise( wxCommandEvent& event ){ OnRandomise( event ); }
		
	
	protected:
		
		wxButton* randomiseButton;
		wxStaticText* m_staticText1;
		wxSpinCtrl* xSpinControl;
		wxStaticText* m_staticText2;
		wxSpinCtrl* ySpinControl;
		wxStaticText* m_staticText3;
		wxSpinCtrl* zSpinControl;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnRandomise( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		clNodeDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("New Node"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~clNodeDialog();
	
};

#endif //__gui__
