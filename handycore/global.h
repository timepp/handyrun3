#pragma once

#include <Windows.h>
#include <string>

extern std::wstring g_config_path;

struct CriticalSection
{
	CRITICAL_SECTION m_cs;

	CriticalSection()
	{
		::InitializeCriticalSection(&m_cs);
	}
	~CriticalSection()
	{
		::DeleteCriticalSection(&m_cs);
	}
	operator CRITICAL_SECTION*()
	{
		return &m_cs;
	}
};

struct AutoCS
{
	AutoCS(CRITICAL_SECTION* cs) : m_cs(cs)
	{
		::EnterCriticalSection(m_cs);
	}
	~AutoCS()
	{
		::LeaveCriticalSection(m_cs);
	}
	CRITICAL_SECTION* m_cs;
};

