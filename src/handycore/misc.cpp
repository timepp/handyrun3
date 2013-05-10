#include "stdafx.h"
#include <public/handycore.h>

HANDYCOREAPI HRESULT HrLoadTextFile(LPCWSTR path, BSTR* content, UINT codepage)
{
	if (!path || !content || *content) return E_INVALIDARG;

	HANDLE f = ::CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f == INVALID_HANDLE_VALUE)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD len = ::GetFileSize(f, NULL);
	char* buffer = new char[len];
	DWORD bytes_read = 0;
	::ReadFile(f, buffer, len, &bytes_read, NULL);
	::CloseHandle(f);

	char* data = buffer;
	if (len > 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF && codepage == CP_UTF8)
	{
		data += 3;
		len -= 3;
	}

	try
	{
		*content = ::SysAllocString(CA2W(data, len));
	} 
	catch (...) {
		delete buffer;
		return HANDYCORE_E_INVALID_DATA;
	}

	delete buffer;
	return S_OK;
}

HANDYCOREAPI HRESULT HrSaveTextFile(LPCWSTR path, LPCWSTR content, UINT codepage)
{
	if (!path || !content) return E_INVALIDARG;

	HANDLE f = ::CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (f == INVALID_HANDLE_VALUE)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	CW2A data(content, codepage);
	DWORD bytes_write = 0;
	if (codepage == CP_UTF8)
	{
		::WriteFile(f, (LPCVOID)"\xEF\xBB\xBF", 3, &bytes_write, NULL);
	}
	::WriteFile(f, data, strlen(data), &bytes_write, NULL);
	::CloseHandle(f);

	return S_OK;
}
