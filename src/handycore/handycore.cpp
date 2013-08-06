// handycore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include <public/handycore.h>

#include <thirdparty/bdlog/include/bdlog_impl.h>

#include "global.h"
#include "appdb.h"

HRESULT TargetFunction(int x)
{
	// Call third_party_function1
	// error handling
	// Call third_party_function2
	// error handling
	// Call third_party_function3
	// error handling
}

HANDYCOREAPI HRESULT HrInitCore(BSTR config_path, HWND serv_wnd)
{
	LOG_FUNCTION(config_path, serv_wnd);

	g_config_path = config_path;
	InitAppDB();
	return S_OK;
}

HANDYCOREAPI HRESULT HrUninitCore()
{
	LOG_FUNCTION();

	UninitAppDB();
	return S_OK;
}
