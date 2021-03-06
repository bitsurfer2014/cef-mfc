
// CEFMFCView.cpp : implementation of the CCEFMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CEFMFC.h"
#endif

#include "CEFMFCDoc.h"
#include "CEFMFCView.h"
#include "MainFrm.h"
#include "DlgAuthenticate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCEFMFCView
const UINT CCEFMFCView::m_pFindDialogMessage = RegisterWindowMessage(FINDMSGSTRING);


IMPLEMENT_DYNCREATE(CCEFMFCView, CView)

BEGIN_MESSAGE_MAP(CCEFMFCView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_NEW_WINDOW, &CCEFMFCView::OnFileNewWindow)
	ON_UPDATE_COMMAND_UI(ID_GO_BACK, &CCEFMFCView::OnUpdateGoBack)
	ON_COMMAND(ID_GO_BACK, &CCEFMFCView::OnGoBack)
	ON_UPDATE_COMMAND_UI(ID_GO_FORWARD, &CCEFMFCView::OnUpdateGoForward)
	ON_COMMAND(ID_GO_FORWARD, &CCEFMFCView::OnGoForward)
	ON_UPDATE_COMMAND_UI(ID_GO_STOP, &CCEFMFCView::OnUpdateGoStop)
	ON_COMMAND(ID_GO_STOP, &CCEFMFCView::OnGoStop)
	ON_COMMAND(ID_GO_REFRESH, &CCEFMFCView::OnGoRefresh)
	ON_COMMAND(ID_GO_HOME_PAGE, &CCEFMFCView::OnGoHome)
	ON_COMMAND(ID_FILE_PRINT, &CCEFMFCView::OnFilePrint)
	ON_COMMAND(ID_EDIT_FIND, &CCEFMFCView::OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CCEFMFCView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, &CCEFMFCView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CCEFMFCView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, &CCEFMFCView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CCEFMFCView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, &CCEFMFCView::OnEditPaste)	
	// CUSTOM
	ON_REGISTERED_MESSAGE(m_pFindDialogMessage, OnFindDialogMessage)
	// CEF
	ON_MESSAGE(WM_APP_CEF_LOAD_START, &CCEFMFCView::OnLoadStart)
	ON_MESSAGE(WM_APP_CEF_LOAD_END, &CCEFMFCView::OnLoadEnd)
	ON_MESSAGE(WM_APP_CEF_TITLE_CHANGE, &CCEFMFCView::OnTitleChange)
	ON_MESSAGE(WM_APP_CEF_STATE_CHANGE, &CCEFMFCView::OnStateChange)
	ON_MESSAGE(WM_APP_CEF_ADDRESS_CHANGE, &CCEFMFCView::OnAddressChange)
	ON_MESSAGE(WM_APP_CEF_STATUS_MESSAGE, &CCEFMFCView::OnStatusMessage)
	ON_MESSAGE(WM_APP_CEF_BEFORE_BROWSE, &CCEFMFCView::OnBeforeBrowse)
	ON_MESSAGE(WM_APP_CEF_DOWNLOAD_UPDATE, &CCEFMFCView::OnDownloadUpdate)	
	ON_MESSAGE(WM_APP_CEF_SEARCH_URL, &CCEFMFCView::OnSearchURL)
	ON_MESSAGE(WM_APP_CEF_CLOSE_BROWSER, &CCEFMFCView::OnCloseBrowser)
	ON_MESSAGE(WM_APP_CEF_NEW_BROWSER, &CCEFMFCView::OnNewBrowser)
	ON_MESSAGE(WM_APP_CEF_WINDOW_CHECK, &CCEFMFCView::OnWindowCheck)
	ON_MESSAGE(WM_APP_CEF_NEW_WINDOW, &CCEFMFCView::OnNewWindow)
	ON_MESSAGE(WM_APP_CEF_AUTHENTICATE, &CCEFMFCView::OnAuthenticate)
	ON_MESSAGE(WM_APP_CEF_BAD_CERTIFICATE, &CCEFMFCView::OnBadCertificate)	
END_MESSAGE_MAP()

// CCEFMFCView construction/destruction

CCEFMFCView::CCEFMFCView()
{
	// TODO: add construction code here
	m_cefBrowser = NULL;
	m_pFindDialog = NULL;
}

CCEFMFCView::~CCEFMFCView()
{
	// release browser if still active
	if( m_cefBrowser )
		m_cefBrowser = NULL;
}

BOOL CCEFMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCEFMFCView drawing

void CCEFMFCView::OnDraw(CDC* /*pDC*/)
{
	CCEFMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CCEFMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCEFMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}


// CCEFMFCView diagnostics

#ifdef _DEBUG
void CCEFMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CCEFMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCEFMFCDoc* CCEFMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCEFMFCDoc)));
	return (CCEFMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CCEFMFCView message handlers
void CCEFMFCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


void CCEFMFCView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// cef stretch to to full view
	if( m_cefBrowser )
	{
		CefWindowHandle hwnd = m_cefBrowser->GetHost()->GetWindowHandle();
		if( hwnd )
		{
			CRect rect;
			GetClientRect( rect );

			HDWP hdwp = BeginDeferWindowPos(1);
			hdwp = DeferWindowPos(hdwp, hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
			EndDeferWindowPos(hdwp);
		}
	}
}


BOOL CCEFMFCView::OnEraseBkgnd(CDC* pDC)
{
	if( m_cefBrowser )
	{
		CefWindowHandle hwnd = m_cefBrowser->GetHost()->GetWindowHandle();
		if( hwnd )
		{
			// Dont erase the background if the browser window has been loaded (this avoids flashing)
			return 0;
		}
	}

	return CView::OnEraseBkgnd(pDC);
}


void CCEFMFCView::OnClose()
{
	// get window handle
	HWND hBrowser = m_cefBrowser->GetHost()->GetWindowHandle();
	// destroy browser window
	::DestroyWindow(hBrowser);

	CView::OnClose();
}


void CCEFMFCView::OnEditFind()
{
	if( m_cefBrowser )
	{
		ASSERT(m_pFindDialog == NULL);
		m_pFindDialog = new CFindReplaceDialog();
		m_pFindDialog->Create(TRUE, _T(""), NULL, FR_DOWN, this);
	}
}


void CCEFMFCView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	CEdit* pEdit = (CEdit*)GetFocus();
	if( IsEdit( pEdit ))
	{
		INT start = 0, end = 0;
		pEdit->GetSel( start, end );
		pCmdUI->Enable( start != end ? TRUE : FALSE );
	}
	else
	{
		if( m_cefBrowser )
			pCmdUI->Enable( TRUE );
	}
}


void CCEFMFCView::OnEditCut()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	if ( IsEdit( pEdit ))
		pEdit->Copy();
	else if( m_cefBrowser )
		m_cefBrowser->GetMainFrame()->Cut();
}


void CCEFMFCView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CEdit* pEdit = (CEdit*)GetFocus();
	if( IsEdit( pEdit ))
	{
		INT start = 0, end = 0;
		pEdit->GetSel( start, end );
		pCmdUI->Enable( start != end ? TRUE : FALSE );
	}
	else
	{
		if( m_cefBrowser )
			pCmdUI->Enable( TRUE );
	}
}


void CCEFMFCView::OnEditCopy()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	if ( IsEdit( pEdit ))
		pEdit->Copy();
	else if( m_cefBrowser )
		m_cefBrowser->GetMainFrame()->Copy();
}


void CCEFMFCView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	CEdit* pEdit = (CEdit*)GetFocus();
	if( IsEdit( pEdit ))
	{
		INT nText = IsClipboardFormatAvailable( CF_TEXT );
		pCmdUI->Enable( nText ? TRUE : FALSE );
	}
	else
	{
		if( m_cefBrowser )
			pCmdUI->Enable( TRUE );
	}
}


void CCEFMFCView::OnEditPaste()
{
	CEdit* pEdit = (CEdit*)GetFocus();
	if ( IsEdit( pEdit ))
		pEdit->Paste();
	else if( m_cefBrowser )
		m_cefBrowser->GetMainFrame()->Paste();
}


LRESULT CCEFMFCView::OnBeforeBrowse(WPARAM wParam, LPARAM lParam)
{
	// get URL
	LPCTSTR lpszURL = (LPCTSTR)wParam;
	BOOL bRedirected = (BOOL)lParam;

	// save URL
	m_szLocationURL = lpszURL;

	// passed the test so allow naviagtion
	return S_OK;
}


LRESULT CCEFMFCView::OnDownloadUpdate(WPARAM wParam, LPARAM lParam)
{
	// get CEF info
	CEFDownloadItemValues* lpValues = (CEFDownloadItemValues*)wParam;

	CString szStatus;

	if( lpValues->bIsComplete )
		szStatus.Format( _T("File Download Complete.") );
	else
		szStatus = FormatTransferInfo( lpValues->nReceived, lpValues->nTotal );

	// set status
	((CMainFrame*)GetParentFrame())->SetStatusIndicator( szStatus );

	return S_OK;
}

LRESULT CCEFMFCView::OnLoadStart(WPARAM wParam, LPARAM lParam)
{
	// size browser
	SendMessage( WM_SIZE );

	return S_OK;
}


LRESULT CCEFMFCView::OnLoadEnd(WPARAM wParam, LPARAM lParam)
{
	// size browser
	SendMessage( WM_SIZE );

	return S_OK;
}

LRESULT CCEFMFCView::OnTitleChange(WPARAM wParam, LPARAM lParam)
{
	// get title
	LPCTSTR lpszTitle = (LPCTSTR)wParam;
	// save name
	m_szLocationName = lpszTitle;
	// set title
	((CMainFrame*)GetParentFrame())->SetWindowTitle( m_szLocationName );

	return S_OK;
}


LRESULT CCEFMFCView::OnStateChange(WPARAM wParam, LPARAM lParam)
{
	// get state
	m_nBrowserState = (INT)wParam;

	return S_OK;
}


LRESULT CCEFMFCView::OnAddressChange(WPARAM wParam, LPARAM lParam)
{
	// get URL
	LPCTSTR lpszURL = (LPCTSTR)wParam;
	// save URL
	m_szLocationURL = lpszURL;
	// set title
	((CMainFrame*)GetParentFrame())->SetAddress( m_szLocationURL );
	return S_OK;
}


LRESULT CCEFMFCView::OnStatusMessage(WPARAM wParam, LPARAM lParam)
{
	// get URL
	LPCTSTR lpszStatus = (LPCTSTR)wParam;
	// set status
	((CMainFrame*)GetParentFrame())->SetStatusIndicator( lpszStatus );
	return S_OK;
}


LRESULT CCEFMFCView::OnSearchURL(WPARAM wParam, LPARAM lParam)
{
	// get URL
	LPCTSTR lpszURL = (LPCTSTR)wParam;

	CString szError;
	CString szURL(lpszURL);
	CString szSearch;

	// remove http:// and trainling /
	szURL.Delete( 0, 7 );
	if( szURL.Right( 1 ).Compare( _T("/") ) == 0 )
		szURL.Delete( szURL.GetLength() - 1 );
	// search for string
	szSearch.Format( _T("%s%s"), _T("https://www.google.com/#q="), szURL );

	// navigate to address
	Navigate( szSearch );
	return S_OK;
}


LRESULT CCEFMFCView::OnFindDialogMessage(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pFindDialog != NULL);

    // If the FR_DIALOGTERM flag is set,
    // invalidate the handle identifying the dialog box.
    if (m_pFindDialog->IsTerminating())
    {
		if( m_cefBrowser )
			m_cefBrowser->GetHost()->StopFinding(true);
        m_pFindDialog = NULL;
		m_bFindNext = FALSE;
        return S_OK;
    }

    // If the FR_FINDNEXT flag is set,
    // call the application-defined search routine
    // to search for the requested string.
    if(m_pFindDialog->FindNext())
    {
        //read data from dialog
        CString FindName = m_pFindDialog->GetFindString();
        bool bMatchCase = m_pFindDialog->MatchCase() == TRUE;
        bool bMatchWholeWord = m_pFindDialog->MatchWholeWord() == TRUE;
        bool bSearchDown = m_pFindDialog->SearchDown() == TRUE;

		CT2CA pszConvertedAnsiString(FindName);
		CefString csFind = pszConvertedAnsiString;

        // find in browser
		if( m_cefBrowser )
			m_cefBrowser->GetHost()->Find(0, csFind, bSearchDown, bMatchCase, m_bFindNext);
		if( !m_bFindNext )
			m_bFindNext = TRUE;
    }
	return S_OK;
}


LRESULT CCEFMFCView::OnCloseBrowser(WPARAM wParam, LPARAM lParam)
{
	// get URL
	INT nBrowserID = (INT)wParam;

	// set new browser
	m_cefBrowser = NULL;

	return S_OK;
}


LRESULT CCEFMFCView::OnNewBrowser(WPARAM wParam, LPARAM lParam)
{
	// get URL
	INT nBrowserID = (INT)wParam;
	CefBrowser* browser = (CefBrowser*)lParam;

	// set new browser
	m_cefBrowser = browser;

	return S_OK;
}


LRESULT CCEFMFCView::OnWindowCheck(WPARAM wParam, LPARAM lParam)
{
	// get CEF info
	CefPopupFeatures* popupFeatures = (CefPopupFeatures*)wParam;
	LPCTSTR lpszURL = (LPCTSTR)lParam;

	// good to go
	return S_OK;
}


LRESULT CCEFMFCView::OnNewWindow(WPARAM wParam, LPARAM lParam)
{
	// get CEF info
	CefPopupFeatures* popupFeatures = (CefPopupFeatures*)wParam;
	CefWindowInfo* windowInfo = (CefWindowInfo*)lParam;

	// Get a pointer to the application object
	CWinApp* pApp = AfxGetApp();
	CFrameWnd* pNewFrame = NULL;
	CCEFMFCView* pView = NULL;
	
	// Get the correct document template
	CDocTemplate* pDocTemplate;
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	pDocTemplate = pApp->GetNextDocTemplate(pos);

	ASSERT(pDocTemplate);

	// Create the new frame
	pNewFrame = pDocTemplate->CreateNewFrame(GetDocument(), (CFrameWnd*)AfxGetMainWnd());
	ASSERT(pNewFrame);

	// Activate the frame and set its active view
	pDocTemplate->InitialUpdateFrame(pNewFrame, NULL);

	// Get view
	pView = (CCEFMFCView*)pNewFrame->GetActiveView();
    ASSERT(pView);

	//// show GUI
	((CMainFrame*)pView->GetParentFrame())->ShowToolBar( popupFeatures->toolBarVisible );
	((CMainFrame*)pView->GetParentFrame())->ShowStatusBar( popupFeatures->statusBarVisible );

	// set sizes and loaction
	((CMainFrame*)pView->GetParentFrame())->SetFrameTop( popupFeatures->x );
	((CMainFrame*)pView->GetParentFrame())->SetFrameLeft( popupFeatures->y );
	((CMainFrame*)pView->GetParentFrame())->SetFrameHeight( popupFeatures->height );
	((CMainFrame*)pView->GetParentFrame())->SetFrameWidth( popupFeatures->width );

	// set window info
	HWND hWnd = pView->GetSafeHwnd();
	CRect rect;
	pView->GetClientRect( rect );

	// set as child so a new window is not opened
	windowInfo->SetAsChild( hWnd, rect );

	return S_OK;
}


LRESULT CCEFMFCView::OnAuthenticate(WPARAM wParam, LPARAM lParam)
{
	// get name and realm
	CEFAuthenticationValues* lpValues = (CEFAuthenticationValues*)wParam;

	// setup message
	CDlgAuthenticate dlg;
	dlg.m_szMessage.Format(_T("The server \"%s\" requires a username and password. The server says: \"%s\"."), lpValues->lpszHost, lpValues->lpszRealm );

	// show dialog
	if( dlg.DoModal() == IDOK )
	{
		_tcscpy_s( lpValues->szUserName, dlg.m_szUserName );
		_tcscpy_s( lpValues->szUserPass, dlg.m_szPassword );
		return S_OK;
	}

	return S_FALSE;
}


LRESULT CCEFMFCView::OnBadCertificate(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}


// CEF Client functions
void CCEFMFCView::Navigate(LPCTSTR pszURL)
{
	if( m_cefBrowser )
	{
		m_cefBrowser->GetMainFrame()->LoadURL( pszURL );

		// set focus to browser
		::SetFocus( m_cefBrowser->GetHost()->GetWindowHandle() );
		m_cefBrowser->GetHost()->SetFocus(true);
	}
}


void CCEFMFCView::GoHome()
{
	Navigate( theApp.m_szHomePage );
}


void CCEFMFCView::Stop()
{
	if( m_cefBrowser )
		m_cefBrowser->StopLoad();
}


CString CCEFMFCView::GetLocationName()
{
	return m_szLocationName;
}


CString CCEFMFCView::GetLocationURL()
{
	return m_szLocationURL;
}


void CCEFMFCView::OnUpdateGoStop(CCmdUI *pCmdUI)
{
	if( m_cefBrowser )
		pCmdUI->Enable( m_nBrowserState & CEF_BIT_IS_LOADING );
}


void CCEFMFCView::OnGoStop()
{
	if( m_cefBrowser )
		m_cefBrowser->StopLoad();
}


void CCEFMFCView::OnGoRefresh()
{
	if( m_cefBrowser )
		m_cefBrowser->ReloadIgnoreCache();
}


void CCEFMFCView::OnGoHome()
{
	Navigate( theApp.m_szHomePage );
}


void CCEFMFCView::OnFilePrint()
{
	if( m_cefBrowser )
		m_cefBrowser->GetHost()->Print();
}


void CCEFMFCView::OnUpdateGoBack(CCmdUI *pCmdUI)
{
	if( m_cefBrowser )
		pCmdUI->Enable( m_nBrowserState & CEF_BIT_CAN_GO_BACK );
}


void CCEFMFCView::OnGoBack()
{
	if( m_cefBrowser )
		m_cefBrowser->GoBack();
}


void CCEFMFCView::OnUpdateGoForward(CCmdUI *pCmdUI)
{
	if( m_cefBrowser )
		pCmdUI->Enable( m_nBrowserState & CEF_BIT_CAN_GO_FORWARD );
}


void CCEFMFCView::OnGoForward()
{
	if( m_cefBrowser )
		m_cefBrowser->GoForward();
}


void CCEFMFCView::OnFileNewWindow()
{
	// Get a pointer to the application object
	CWinApp* pApp = AfxGetApp();
	CFrameWnd* pNewFrame = NULL;
	CCEFMFCView* pView = NULL;
	
	// Get the correct document template
	CDocTemplate* pDocTemplate;
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	pDocTemplate = pApp->GetNextDocTemplate(pos);

	ASSERT(pDocTemplate);

	// Create the new frame
	pNewFrame = pDocTemplate->CreateNewFrame(GetDocument(), (CFrameWnd*)AfxGetMainWnd());
	ASSERT(pNewFrame);

	// Activate the frame and set its active view
	pDocTemplate->InitialUpdateFrame(pNewFrame, NULL);

	// Get view
	pView = (CCEFMFCView*)pNewFrame->GetActiveView();
    ASSERT(pView);

	// get rect
	CRect rect;
	pView->GetClientRect( rect );

	CString szURL = GetLocationURL();

	// set browser
	theApp.m_cefApp->CreateBrowser( pView->GetSafeHwnd(), rect, GetLocationURL() );
}


BOOL CCEFMFCView::IsEdit(CWnd* pWnd)
{
	if( pWnd == NULL )
		return FALSE;

	HWND hWnd = pWnd->GetSafeHwnd();
	if (hWnd == NULL)
		return FALSE;
	
	TCHAR szClassName[6];
	::GetClassName(hWnd, szClassName, 6) && _tcsicmp(szClassName, _T("Edit")) == 0;
	//TRACE( _T("Class=%s\n"), szClassName );

	return ::GetClassName(hWnd, szClassName, 6) && _tcsicmp(szClassName, _T("Edit")) == 0;
}


CString CCEFMFCView::FormatTransferInfo(ULONGLONG dwBytesRead, ULONGLONG dwFileSize)
{
  CString szText;
  CString szRead;
  CString szSize;

  // format read
  if(dwBytesRead < 1024)
    szRead.Format(_T("%I64u B"), dwBytesRead);
  else if (dwBytesRead < 1048576)
    szRead.Format(_T("%0.1f KB"), static_cast<LONGLONG>(dwBytesRead)/1024.0);
  else
    szRead.Format(_T("%0.2f MB"), static_cast<LONGLONG>(dwBytesRead)/1048576.0);

  // format size
  if (dwFileSize < 1024)
    szSize.Format(_T("%I64u B"), dwFileSize);
  else if (dwFileSize < 1048576)
    szSize.Format(_T("%0.1f KB"), static_cast<LONGLONG>(dwFileSize)/1024.0);
  else
    szSize.Format(_T("%0.2f MB"), static_cast<LONGLONG>(dwFileSize)/1048576.0);

  // return text
  szText.Format(_T("Downloading file. Received %s of %s."), szRead, szSize );

  return szText;
}
