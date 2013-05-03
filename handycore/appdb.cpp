#include "stdafx.h"
#include "appdb.h"
#include "global.h"
#include <handycore.h>
#include <map>

bool operator< (const GUID& g1, const GUID& g2)
{
	return memcmp(&g1, &g2, sizeof(GUID)) < 0;
}

typedef std::map<GUID, HRAPP*> appmap_t;
appmap_t g_appmap;
CriticalSection g_csdb;
#define MT_GUARD AutoCS cs_(g_csdb);

HRESULT InitAppDB()
{
	MT_GUARD;

	HANDLE hcfg;
	HANDLE happ;
	HrOpenConfig(NULL, L"hrapp", &hcfg);
	HrOpenConfig(hcfg, L"app", &happ);

	return S_OK;
}

HRESULT UninitAppDB()
{
	MT_GUARD;
	return S_OK;
}
