
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars



#pragma comment(lib, "libcef.lib")
#pragma comment(lib, "libcef_dll_wrapper.lib")

#define WM_APP_CEF_LOAD_START		(WM_APP + 301)
#define WM_APP_CEF_LOAD_END			(WM_APP + 302)
#define WM_APP_CEF_TITLE_CHANGE		(WM_APP + 303)
#define WM_APP_CEF_ADDRESS_CHANGE	(WM_APP + 304)
#define WM_APP_CEF_STATE_CHANGE		(WM_APP + 305)
#define WM_APP_CEF_STATUS_MESSAGE	(WM_APP + 306)
#define WM_APP_CEF_NEW_WINDOW		(WM_APP + 310)
#define WM_APP_CEF_WINDOW_CHECK		(WM_APP + 311)
#define WM_APP_CEF_CLOSE_BROWSER	(WM_APP + 312)
#define WM_APP_CEF_NEW_BROWSER		(WM_APP + 315)
#define WM_APP_CEF_SEARCH_URL		(WM_APP + 325)
#define WM_APP_CEF_BEFORE_BROWSE	(WM_APP + 350)
#define WM_APP_CEF_DOWNLOAD_UPDATE	(WM_APP + 355)

#define WM_APP_CEF_AUTHENTICATE		(WM_APP + 370)
#define WM_APP_CEF_BAD_CERTIFICATE	(WM_APP + 371)

#define CEF_MENU_ID_OPEN_LINK		(MENU_ID_USER_FIRST + 100)
#define CEF_MENU_ID_OPEN_LINK_TAB	(MENU_ID_USER_FIRST + 101)
#define CEF_MENU_ID_OPEN_LINK_NEW	(MENU_ID_USER_FIRST + 102)

#define CEF_BIT_IS_LOADING			0x001
#define CEF_BIT_CAN_GO_BACK			0x002
#define CEF_BIT_CAN_GO_FORWARD		0x004

struct CEFAuthenticationValues
{
	LPCTSTR lpszHost;
	LPCTSTR lpszRealm;
	TCHAR szUserName[1024];
	TCHAR szUserPass[1024];
};

struct CEFDownloadItemValues
{
	BOOL bIsValid;
	BOOL bIsInProgress;
	BOOL bIsComplete;
	BOOL bIsCanceled;
	INT nProgress;
	LONGLONG nSpeed;
	LONGLONG nReceived;
	LONGLONG nTotal;
};






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


