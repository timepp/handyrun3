#pragma once
#include <windows.h>
#include <wtypes.h>

/// interface between handyrun and its plugins

/*
	HRAPP : handyrun app, runnable objects in HandyRun
*/
struct HRAPP
{
	DWORD size;

	BSTR path;
	BSTR param;
	BSTR startdir;
	BSTR env;
	UINT64 affinity;
	DWORD show_type;

	BSTR comment;
	BSTR tag;
	BSTR icon;
};

typedef GUID APPID;

__interface IHandyRun
{
	// app management
	HRESULT EnumApp(APPID* ids, DWORD idslen, DWORD* reqlen);
	HRESULT AddApp(const HRAPP* app, APPID* id);
	HRESULT DelApp(APPID appid);
	HRESULT ModifyApp(APPID appid, const HRAPP* app);
	HRESULT GetApp(APPID appid, HRAPP* app);
	HRESULT ConfigureApp(APPID appid);

	HRESULT HrOpenConfig(HANDLE hparent, LPCWSTR name, HANDLE* hcfg);
	HRESULT HrSaveConfig(HANDLE hcfg, LPCWSTR path);
	HRESULT HrCloseConfig(HANDLE hcfg);
	HRESULT HrGetConfigString(HANDLE hcfg, LPCWSTR name, BSTR* value);
	HRESULT HrSetConfigString(HANDLE hcfg, LPCWSTR name, LPCWSTR value);

	// misc
	HRESULT ShowSettings();
	HRESULT LoadTextFile(LPCWSTR path, BSTR* content, UINT codepage);
	HRESULT SaveTextFile(LPCWSTR path, LPCWSTR content, UINT codepage);
};

#define FACILITY_HANDYRUN 0x1e7
#define HANDYCORE_E_INVALID_HANDLE        MAKE_HRESULT(1, FACILITY_HANDYCORE, 1)
#define HANDYCORE_E_CONFIG_NOT_FOUND      MAKE_HRESULT(1, FACILITY_HANDYCORE, 2)
#define HANDYCORE_E_VALUE_NOT_FOUND       MAKE_HRESULT(1, FACILITY_HANDYCORE, 3)
#define HANDYCORE_E_INVALID_DATA          MAKE_HRESULT(1, FACILITY_HANDYCORE, 4)

/// plugin interfaces

HRESULT HrpInitialize(IHandyRun* handyrun, BSTR* name, BSTR* description);
HRESULT HrpUninitialize();
