
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CEFMFC.h"

#include "MainFrm.h"
#include "CEFMFCDoc.h"
#include "CEFMFCView.h"

#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_ADDRESS_BOX, &CMainFrame::OnAddressEnter)
	ON_COMMAND(IDOK, &CMainFrame::OnAddressEnter)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// disable menu
	SetMenu( NULL );

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));

	// prevent loading from registry for secondary windows
	GetDockingManager()->DisableRestoreDockState();

	// set sizes
	CMFCToolBar::SetSizes( CSize(40,40), CSize(32, 32) );
	CMFCToolBar::SetMenuSizes( CSize(22,22), CSize(16, 16) );

	if(!m_wndReBar.Create( this, RBS_BANDBORDERS, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP, AFX_IDW_REBAR))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	//if (!m_wndMenuBar.Create(this))
	//{
	//	TRACE0("Failed to create menubar\n");
	//	return -1;      // fail to create
	//}

	//m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	//CMFCPopupMenu::SetForceMenuFocus(FALSE);


	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// rebar variables
	REBARBANDINFO rbbi;
	CSize sizeBar;
	DWORD dwStyle = 0;

	// add bar to rebar
	if( !m_wndReBar.AddBar( &m_wndToolBar, NULL, NULL, dwStyle ))
	{
		TRACE0("Failed to add Navbar\n");
		return -1;      // fail to create
	}

	// get sizes
	sizeBar = m_wndToolBar.CalcSize( FALSE );
	// reset rbbi
	memset( &rbbi, NULL, sizeof(REBARBANDINFO) );

	// adjust rebar
	//rbbi.cbSize = sizeof(rbbi);
	rbbi.cbSize = m_wndReBar.GetReBarBandInfoSize();
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_ID;
	rbbi.cxMinChild = sizeBar.cx;
	rbbi.cyMinChild = sizeBar.cy;
	rbbi.cx = rbbi.cxIdeal = sizeBar.cx + 30;
	rbbi.wID = IDR_MAINFRAME;
	m_wndReBar.GetReBarCtrl().SetBandInfo( 0, &rbbi );

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);


	// Create a combo box for the address bar:
	bNameValid = FALSE;
	CString strAddressLabel;
	bNameValid = strAddressLabel.LoadString( IDS_ADDRESS );
	ASSERT(bNameValid);
	if (!m_wndAddress.Create (CBS_DROPDOWN | WS_CHILD, CRect(0, 0, 400, 120), this, ID_ADDRESS_BOX))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}

	// add home page
	m_wndAddress.AddString( theApp.m_szHomePage );

	// set height
	m_wndAddress.SetHeight( 20 );

	// rebar style
	dwStyle = RBBS_FIXEDBMP | RBBS_BREAK | RBBS_GRIPPERALWAYS; 

	// add bar to rebar
	if( !m_wndReBar.AddBar( &m_wndAddress, NULL, NULL, dwStyle ))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	// adjust rebar
	CRect rectAddress;
	m_wndAddress.GetEditCtrl()->GetWindowRect(&rectAddress);

	// reset rbbi
	memset( &rbbi, NULL, sizeof(REBARBANDINFO) );

	// adjust rebar
	//rbbi.cbSize = sizeof(rbbi);
	rbbi.cbSize = m_wndReBar.GetReBarBandInfoSize();
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_ID;
	rbbi.cyMinChild = rectAddress.Height() + 12;
	rbbi.cxIdeal = 400;
	rbbi.cx = 400;
	rbbi.wID = ID_ADDRESS_BOX;
	m_wndReBar.GetReBarCtrl().SetBandInfo( 1, &rbbi );


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	DockPane( &m_wndReBar );

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnAddressEnter()
{
	// address bar disabled
	if( !m_wndAddress )
		return;

	CString szURL;
	m_wndAddress.GetEditCtrl()->GetWindowText( szURL );

	if( szURL.IsEmpty() )
		return;

	CView* pActiveView = GetActiveView();
	if( !pActiveView )
		return;

	CCEFMFCView* pView = (CCEFMFCView*)GetActiveView();
	if( pView )
	{
		// navigate to URL
		pView->Navigate( szURL );

		// add it
		if( m_wndAddress.FindString( szURL ) == CB_ERR )
			m_wndAddress.AddString( szURL );
	}
}


void CMainFrame::SetAddress(LPCTSTR lpszUrl)
{
	// address bar disabled
	if( !m_wndAddress )
		return;

	// set title
	m_wndAddress.GetEditCtrl()->SetWindowText( lpszUrl );
}


void CMainFrame::SetWindowTitle(LPCTSTR lpszTitle)
{
	// set title
	SetWindowText( lpszTitle );
}


void CMainFrame::SetStatusIndicator(LPCTSTR lpszStatus)
{
	// set text
	m_wndStatusBar.SetPaneText( 0, lpszStatus );
}


void CMainFrame::ShowToolBar(BOOL bShow)
{
	//Show or hide tool bar
	if( m_wndReBar )
	{
		if( bShow )
			m_wndReBar.ShowWindow( SW_SHOWNORMAL );
		else
			m_wndReBar.ShowWindow( SW_HIDE );
	}
}


void CMainFrame::ShowStatusBar(BOOL bShow)
{
	//Show or hide tool bar
	if( m_wndStatusBar )
	{
		if( bShow )
			m_wndStatusBar.ShowWindow( SW_SHOWNORMAL );
		else
			m_wndStatusBar.ShowWindow( SW_HIDE );
	}
}


void CMainFrame::ShowAddressBar(BOOL bShow)
{
	//Show or hide address bar
	if( m_wndReBar )
		m_wndReBar.GetReBarCtrl().ShowBand( m_wndReBar.GetReBarCtrl().IDToIndex( ID_ADDRESS_BOX ), bShow );
}


void CMainFrame::SetFrameWidth(LONG nWidth)
{
	if (nWidth >= 0)
	{
		// get border sizes
		INT nOther = 0;
		nOther = 2 * GetSystemMetrics( SM_CXSIZEFRAME );
		nOther += GetSystemMetrics( SM_CXHSCROLL );

		// set window size
		CRect rectFrame;
		GetWindowRect(rectFrame);
		CSize size = rectFrame.Size();
		SetWindowPos( NULL, 0, 0, nWidth + nOther, size.cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE );
	}
}


void CMainFrame::SetFrameHeight(LONG nHeight)
{
	if (nHeight >= 0)
	{
		CRect rectFrame;
		CRect rectToolBar;
		CRect rectStatusBar;
		INT nOther = 0;

		// get rebar and status bar height
		if( m_wndReBar )
			if( m_wndReBar.IsWindowVisible() )
			{
				m_wndReBar.GetWindowRect( rectToolBar );
				nOther += rectToolBar.Height();
			}

		if( m_wndStatusBar )
			if( m_wndStatusBar.IsWindowVisible() )
			{
				m_wndStatusBar.GetWindowRect( rectStatusBar );
				nOther += rectStatusBar.Height();
			}

		// get caption height
		nOther += GetSystemMetrics( SM_CYCAPTION );
		// get border sizes
		nOther += 2 * GetSystemMetrics( SM_CYSIZEFRAME );

		GetWindowRect(rectFrame);
		CSize size = rectFrame.Size();

		SetWindowPos( NULL, 0, 0, size.cx, nHeight + nOther, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE );
	}
}


void CMainFrame::SetFrameTop(LONG nTop)
{
	// fix for pop-ups
	if( nTop == 9999 )
		return;

	if( nTop >= 0 )
	{
		CRect rectFrame;
		GetWindowRect( rectFrame );
		SetWindowPos( NULL, rectFrame.left, nTop, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}
}


void CMainFrame::SetFrameLeft(LONG nLeft)
{
	// fix for pop-ups
	if( nLeft == 9999 )
		return;

	if( nLeft >= 0 )
	{
		CRect rectFrame;
		GetWindowRect( rectFrame );
		SetWindowPos( NULL, nLeft, rectFrame.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}
}
