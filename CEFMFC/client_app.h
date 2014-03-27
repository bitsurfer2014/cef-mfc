#pragma once

#include "include/cef_app.h"
#include "client_handler.h"

class ClientApp: public CefApp,
	public CefBrowserProcessHandler
{
public:
	ClientApp(void);
	ClientApp(HWND hWnd);
	~ClientApp(void);

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	BOOL CreateBrowser(HWND hWnd, CRect rect, LPCTSTR pszURL);

public:
	CefRefPtr<ClientHandler> m_cefHandler;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(ClientApp);
};
