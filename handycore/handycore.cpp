// handycore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <handycore.h>
#include "global.h"
#include "appdb.h"

HANDYCOREAPI HRESULT HrInitCore(BSTR config_path, HWND serv_wnd)
{
	g_config_path = config_path;
	InitAppDB();
	return S_OK;
}

HANDYCOREAPI HRESULT HrUninitCore()
{
	UninitAppDB();
	return S_OK;
}
