#pragma once

#include <windows.h>
#include <wtypes.h>

#ifdef HANDYCORE_EXPORTS
#define HANDYCOREAPI extern "C" __declspec(dllexport) 
#else
#define HANDYCOREAPI extern "C" __declspec(dllimport)
#pragma comment(lib, "handycore.lib")
#endif

#define FACILITY_HANDYCORE 0x1e7
#define HANDYCORE_E_INVALID_HANDLE        MAKE_HRESULT(1, FACILITY_HANDYCORE, 1)
#define HANDYCORE_E_CONFIG_NOT_FOUND      MAKE_HRESULT(1, FACILITY_HANDYCORE, 2)
#define HANDYCORE_E_VALUE_NOT_FOUND       MAKE_HRESULT(1, FACILITY_HANDYCORE, 3)
#define HANDYCORE_E_INVALID_DATA        MAKE_HRESULT(1, FACILITY_HANDYCORE, 4)

/*
	HRAPP : handyrun app 可以运行的对象
*/
struct HRAPP
{
	DWORD size;
	BSTR path;
	BSTR param;
	BSTR startdir;
	BSTR comment;
	BSTR tag;
	BSTR icon;
	BSTR env;
	UINT64 affinity;
	DWORD show_type;
};

typedef GUID APPID;

HANDYCOREAPI HRESULT HrInitCore(BSTR config_path, HWND serv_wnd);
HANDYCOREAPI HRESULT HrUninitCore();

/* 应用管理 */

HANDYCOREAPI HRESULT HrEnumApp(APPID* ids, DWORD idslen, DWORD* reqlen);

HANDYCOREAPI HRESULT HrAddApp(HRAPP* app, APPID* id);
HANDYCOREAPI HRESULT HrDelApp(APPID appid);
HANDYCOREAPI HRESULT HrModifyApp(APPID appid, HRAPP* app);

HANDYCOREAPI HRESULT HrGetApp(APPID appid, HRAPP* app);
HANDYCOREAPI HRESULT HrFreeApp(HRAPP* app);

HANDYCOREAPI HRESULT HrConfigureApp(APPID appid);

/* 配置存储 */

HANDYCOREAPI HRESULT HrOpenConfig(HANDLE hparent, LPCWSTR name, HANDLE* hcfg);
HANDYCOREAPI HRESULT HrSaveConfig(HANDLE hcfg, LPCWSTR path);
HANDYCOREAPI HRESULT HrCloseConfig(HANDLE hcfg);
HANDYCOREAPI HRESULT HrGetConfigString(HANDLE hcfg, LPCWSTR name, BSTR* value);
HANDYCOREAPI HRESULT HrSetConfigString(HANDLE hcfg, LPCWSTR name, LPCWSTR value);

/* 杂项 */

HANDYCOREAPI HRESULT HrShowSettings();
HANDYCOREAPI HRESULT HrLoadTextFile(LPCWSTR path, BSTR* content, UINT codepage);
HANDYCOREAPI HRESULT HrSaveTextFile(LPCWSTR path, LPCWSTR content, UINT codepage);
