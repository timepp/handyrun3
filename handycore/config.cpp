#include "stdafx.h"
#include <handycore.h>
#include <thirdparty/rapidxml/rapidxml.hpp>
#include <thirdparty/rapidxml/rapidxml_print.hpp>
#include <map>
#include <sstream>
#include <iostream>
#include <OleAuto.h>
#include "global.h"

struct XmlReader
{
	rapidxml::xml_node<wchar_t>* node;
	std::wstring path; // 磁盘文件路径, 路径不为空, 说明node为顶层的document结点
	HANDLE root; // 顶层的document的handle
	bool dirty; // 是否被写入过数据

	XmlReader() : node(NULL), root(NULL), dirty(false)
	{
	}
	XmlReader(rapidxml::xml_node<wchar_t>* n, const std::wstring& p, HANDLE r, bool d) 
		: node(n), path(p), root(r), dirty(d)
	{
	}
};

typedef std::map<HANDLE, XmlReader> xrmap_t;

std::wstring g_config_path;
xrmap_t g_xrmap;
int handle_value = 0;

CriticalSection g_cs_config;
#define MT_GUARD AutoCS cs_(g_cs_config);
#define CHECK_PARAM(x) if (!(x)) return E_INVALIDARG;

// RPEOF ==> Return Proper Error On Fail
#define HANDLE_TO_READER_RPEOF(h, it, xr) \
	xrmap_t::iterator it = g_xrmap.find(h); \
	if (it == g_xrmap.end()) return HANDYCORE_E_INVALID_HANDLE; \
	XmlReader& xr = it->second;

static HANDLE AddXmlReader(rapidxml::xml_node<wchar_t>* node, const std::wstring& path, HANDLE root)
{
	HANDLE h = (HANDLE)(++handle_value);
	if (!root)
	{
		root = h;
	}
	g_xrmap[h] = XmlReader(node, path, root, false);
	return h;
}

HANDYCOREAPI HRESULT HrOpenConfig(HANDLE hparent, LPCWSTR name, HANDLE* hcfg)
{
	CHECK_PARAM(hcfg);
	MT_GUARD;

	if (!hparent) // open file
	{
		std::wstring path = g_config_path + L"\\" + name + L".xml";
		BSTR text = NULL;
		HRESULT hr = HrLoadTextFile(path.c_str(), &text, CP_UTF8);
		rapidxml::xml_document<wchar_t>* doc = new rapidxml::xml_document<wchar_t>;
		if (SUCCEEDED(hr))
		{
			wchar_t* buffer = doc->allocate_string(text);
			::SysFreeString(text);
			try
			{
				doc->parse<0>(buffer);
			}
			catch (...) {
			}
		}

		*hcfg = AddXmlReader(doc, path, NULL);
	}
	else
	{
		HANDLE_TO_READER_RPEOF(hparent, it, xr);
		rapidxml::xml_node<wchar_t>* node = xr.node->first_node(name);
		if (!node)
		{
			node = xr.node->document()->allocate_node(rapidxml::node_element, name);
			xr.node->append_node(node);
		}

		*hcfg = AddXmlReader(node, L"", xr.root);
	}

	return S_OK;
}

HANDYCOREAPI HRESULT HrSaveConfig(HANDLE hcfg, LPCWSTR path)
{
	CHECK_PARAM(hcfg && path);
	MT_GUARD;

	HANDLE_TO_READER_RPEOF(hcfg, it, xr);
	std::wostringstream oss;
	rapidxml::print<wchar_t>(oss, *xr.node);
	return HrSaveTextFile(path, oss.str().c_str(), CP_UTF8);
}

HANDYCOREAPI HRESULT HrCloseConfig(HANDLE hcfg)
{
	MT_GUARD;

	HANDLE_TO_READER_RPEOF(hcfg, it, xr);
	if (!xr.root)
	{
		// 顶层document结点, 需要存文件, 删除结点
		if (xr.dirty)
		{
			HrSaveConfig(hcfg, xr.path.c_str());
		}
		delete xr.node;
	}

	g_xrmap.erase(it);

	return S_OK;
}

HANDYCOREAPI HRESULT HrGetConfigString(HANDLE hcfg, LPCWSTR name, BSTR* value)
{
	CHECK_PARAM(hcfg && name && value);
	MT_GUARD;

	HANDLE_TO_READER_RPEOF(hcfg, it, xr);

	rapidxml::xml_attribute<wchar_t>* attr = xr.node->first_attribute(name);
	if (!attr)
	{
		return HANDYCORE_E_VALUE_NOT_FOUND;
	}

	*value = ::SysAllocString(attr->value());
	return S_OK;
}

HANDYCOREAPI HRESULT HrSetConfigString(HANDLE hcfg, LPCWSTR name, LPCWSTR value)
{
	CHECK_PARAM(hcfg && name && value);
	MT_GUARD;

	HANDLE_TO_READER_RPEOF(hcfg, it, xr);

	rapidxml::xml_attribute<wchar_t>* attr = xr.node->first_attribute(name);
	if (attr)
	{
		attr->value(xr.node->document()->allocate_string(value));
	}
	else
	{
		attr = xr.node->document()->allocate_attribute(name, value);
		xr.node->append_attribute(attr);
	}
	
	xr.dirty = true;
	xrmap_t::iterator it2 = g_xrmap.find(xr.root);
	if (it2 == g_xrmap.end())
	{
		// no!
	}
	it2->second.dirty = true;

	return S_OK;
}
