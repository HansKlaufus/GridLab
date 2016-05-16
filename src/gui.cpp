///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

#include "../bitmaps/help.xpm"
#include "../bitmaps/new.xpm"
#include "../bitmaps/open.xpm"
#include "../bitmaps/preview.xpm"
#include "../bitmaps/print.xpm"
#include "../bitmaps/save.xpm"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE( clMainFrame, wxFrame )
	EVT_TOOL( MENU_NEW, clMainFrame::_wxFB_OnNewFile )
	EVT_TOOL( MENU_OPEN, clMainFrame::_wxFB_OnOpenFile )
	EVT_TOOL( MENU_SAVE, clMainFrame::_wxFB_OnSaveFile )
	EVT_TOOL( MENU_PRINT, clMainFrame::_wxFB_OnPrint )
	EVT_TOOL( MENU_PREVIEW, clMainFrame::_wxFB_OnPreview )
	EVT_TOOL( MENU_ABOUT, clMainFrame::_wxFB_OnAbout )
	EVT_MENU( MENU_NEW, clMainFrame::_wxFB_OnNewFile )
	EVT_MENU( MENU_OPEN, clMainFrame::_wxFB_OnOpenFile )
	EVT_MENU( MENU_SAVE, clMainFrame::_wxFB_OnSaveFile )
	EVT_MENU( MENU_CLOSE, clMainFrame::_wxFB_OnCloseFile )
	EVT_MENU( MENU_PRINT, clMainFrame::_wxFB_OnPrint )
	EVT_MENU( MENU_PREVIEW, clMainFrame::_wxFB_OnPreview )
	EVT_MENU( MENU_QUIT, clMainFrame::_wxFB_OnQuit )
	EVT_MENU( MENU_SURFACEGRID, clMainFrame::_wxFB_OnSurfaceGrid )
	EVT_MENU( MENU_BOUNDARYGRID, clMainFrame::_wxFB_OnBoundaryGrid )
	EVT_MENU( MENU_VOLUMEGRID, clMainFrame::_wxFB_OnVolumeGrid )
	EVT_MENU( MENU_INSERTNODE, clMainFrame::_wxFB_onInsertNode )
	EVT_MENU( MENU_DUMPGRID, clMainFrame::_wxFB_OnDumpGrid )
	EVT_MENU( MENU_ABOUT, clMainFrame::_wxFB_OnAbout )
	EVT_TREE_ITEM_EXPANDED( wxID_ANY, clMainFrame::_wxFB_OnExpandTree )
	EVT_TREE_SEL_CHANGED( wxID_ANY, clMainFrame::_wxFB_OnSelectionChanged )
END_EVENT_TABLE()

clMainFrame::clMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mainToolBar = this->CreateToolBar( 0, wxID_ANY ); 
	mainToolBar->AddTool( MENU_NEW, wxT("New"), wxBitmap( new_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mainToolBar->AddTool( MENU_OPEN, wxT("Open"), wxBitmap( open_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mainToolBar->AddTool( MENU_SAVE, wxT("Save"), wxBitmap( save_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mainToolBar->AddSeparator();
	mainToolBar->AddTool( MENU_PRINT, wxT("Print"), wxBitmap( print_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mainToolBar->AddTool( MENU_PREVIEW, wxT("Preview"), wxBitmap( preview_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mainToolBar->AddSeparator();
	mainToolBar->AddTool( MENU_ABOUT, wxT("About"), wxBitmap( help_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mainToolBar->Realize();
	
	mainMenuBar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* newMenuItem;
	newMenuItem = new wxMenuItem( fileMenu, MENU_NEW, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	newMenuItem->SetBitmaps( wxBitmap( new_xpm ) );
	#elif defined( __WXGTK__ )
	newMenuItem->SetBitmap( wxBitmap( new_xpm ) );
	#endif
	fileMenu->Append( newMenuItem );
	
	wxMenuItem* openMenuItem;
	openMenuItem = new wxMenuItem( fileMenu, MENU_OPEN, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	openMenuItem->SetBitmaps( wxBitmap( open_xpm ) );
	#elif defined( __WXGTK__ )
	openMenuItem->SetBitmap( wxBitmap( open_xpm ) );
	#endif
	fileMenu->Append( openMenuItem );
	
	wxMenuItem* saveMenuItem;
	saveMenuItem = new wxMenuItem( fileMenu, MENU_SAVE, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	saveMenuItem->SetBitmaps( wxBitmap( save_xpm ) );
	#elif defined( __WXGTK__ )
	saveMenuItem->SetBitmap( wxBitmap( save_xpm ) );
	#endif
	fileMenu->Append( saveMenuItem );
	saveMenuItem->Enable( false );
	
	wxMenuItem* closeMenuItem;
	closeMenuItem = new wxMenuItem( fileMenu, MENU_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( closeMenuItem );
	closeMenuItem->Enable( false );
	
	fileMenu->AppendSeparator();
	
	wxMenuItem* printMenuItem;
	printMenuItem = new wxMenuItem( fileMenu, MENU_PRINT, wxString( wxT("Print") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	printMenuItem->SetBitmaps( wxBitmap( print_xpm ) );
	#elif defined( __WXGTK__ )
	printMenuItem->SetBitmap( wxBitmap( print_xpm ) );
	#endif
	fileMenu->Append( printMenuItem );
	
	wxMenuItem* previewMenuItem;
	previewMenuItem = new wxMenuItem( fileMenu, MENU_PREVIEW, wxString( wxT("Preview") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	previewMenuItem->SetBitmaps( wxBitmap( preview_xpm ) );
	#elif defined( __WXGTK__ )
	previewMenuItem->SetBitmap( wxBitmap( preview_xpm ) );
	#endif
	fileMenu->Append( previewMenuItem );
	
	fileMenu->AppendSeparator();
	
	wxMenuItem* quitMenuItem;
	quitMenuItem = new wxMenuItem( fileMenu, MENU_QUIT, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( quitMenuItem );
	
	mainMenuBar->Append( fileMenu, wxT("File") );
	
	editMenu = new wxMenu();
	wxMenuItem* copyMenuItem;
	copyMenuItem = new wxMenuItem( editMenu, MENU_COPY, wxString( wxT("Copy") ) , wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( copyMenuItem );
	copyMenuItem->Enable( false );
	
	wxMenuItem* pasteMenuItem;
	pasteMenuItem = new wxMenuItem( editMenu, MENU_PASTE, wxString( wxT("Paste") ) , wxEmptyString, wxITEM_NORMAL );
	editMenu->Append( pasteMenuItem );
	pasteMenuItem->Enable( false );
	
	mainMenuBar->Append( editMenu, wxT("Edit") );
	
	gridMenu = new wxMenu();
	wxMenuItem* surfaceGridMenuItem;
	surfaceGridMenuItem = new wxMenuItem( gridMenu, MENU_SURFACEGRID, wxString( wxT("Surface Grid") ) , wxEmptyString, wxITEM_NORMAL );
	gridMenu->Append( surfaceGridMenuItem );
	
	wxMenuItem* boundaryGridMenuItem;
	boundaryGridMenuItem = new wxMenuItem( gridMenu, MENU_BOUNDARYGRID, wxString( wxT("Boundary Grid") ) , wxEmptyString, wxITEM_NORMAL );
	gridMenu->Append( boundaryGridMenuItem );
	boundaryGridMenuItem->Enable( false );
	
	wxMenuItem* volumeGridMenuItem;
	volumeGridMenuItem = new wxMenuItem( gridMenu, MENU_VOLUMEGRID, wxString( wxT("Volume Grid") ) , wxEmptyString, wxITEM_NORMAL );
	gridMenu->Append( volumeGridMenuItem );
	
	gridMenu->AppendSeparator();
	
	wxMenuItem* insertNodeMenuItem;
	insertNodeMenuItem = new wxMenuItem( gridMenu, MENU_INSERTNODE, wxString( wxT("Insert Node") ) , wxEmptyString, wxITEM_NORMAL );
	gridMenu->Append( insertNodeMenuItem );
	
	wxMenuItem* dumpGridMenuItem;
	dumpGridMenuItem = new wxMenuItem( gridMenu, MENU_DUMPGRID, wxString( wxT("Dump Grid") ) , wxEmptyString, wxITEM_NORMAL );
	gridMenu->Append( dumpGridMenuItem );
	
	mainMenuBar->Append( gridMenu, wxT("Grid") );
	
	helpMenu = new wxMenu();
	wxMenuItem* aboutMenuItem;
	aboutMenuItem = new wxMenuItem( helpMenu, MENU_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	helpMenu->Append( aboutMenuItem );
	
	mainMenuBar->Append( helpMenu, wxT("Help") );
	
	this->SetMenuBar( mainMenuBar );
	
	mainStatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* m_bSizer1;
	m_bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( clMainFrame::m_splitter1OnIdle ), NULL, this );
	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* m_bSizer2;
	m_bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_splitter2 = new wxSplitterWindow( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( clMainFrame::m_splitter2OnIdle ), NULL, this );
	m_panel3 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	treeControl = new wxTreeCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer11->Add( treeControl, 1, wxALL|wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer11 );
	m_panel3->Layout();
	bSizer11->Fit( m_panel3 );
	m_panel4 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	noteBook = new wxAuiNotebook( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );
	
	bSizer12->Add( noteBook, 1, wxEXPAND | wxALL, 5 );
	
	m_panel4->SetSizer( bSizer12 );
	m_panel4->Layout();
	bSizer12->Fit( m_panel4 );
	m_splitter2->SplitVertically( m_panel3, m_panel4, 194 );
	m_bSizer2->Add( m_splitter2, 1, wxEXPAND, 5 );
	
	m_panel1->SetSizer( m_bSizer2 );
	m_panel1->Layout();
	m_bSizer2->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	textWindow = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY );
	bSizer8->Add( textWindow, 1, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer8 );
	m_panel2->Layout();
	bSizer8->Fit( m_panel2 );
	m_splitter1->SplitHorizontally( m_panel1, m_panel2, 0 );
	m_bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	this->SetSizer( m_bSizer1 );
	this->Layout();
}

clMainFrame::~clMainFrame()
{
}

BEGIN_EVENT_TABLE( clNodeDialog, wxDialog )
	EVT_BUTTON( wxID_ANY, clNodeDialog::_wxFB_OnRandomise )
END_EVENT_TABLE()

clNodeDialog::clNodeDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Specify node coordinates:") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	randomiseButton = new wxButton( this, wxID_ANY, wxT("Random"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( randomiseButton, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer2->Add( m_staticText1, 0, wxALL, 5 );
	
	xSpinControl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer2->Add( xSpinControl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer2->Add( m_staticText2, 0, wxALL, 5 );
	
	ySpinControl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer2->Add( ySpinControl, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Z:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	zSpinControl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer2->Add( zSpinControl, 0, wxALL|wxEXPAND, 5 );
	
	sbSizer1->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	bSizer6->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer6->Add( m_sdbSizer1, 0, wxEXPAND, 15 );
	
	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );
}

clNodeDialog::~clNodeDialog()
{
}
