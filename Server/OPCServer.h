// OPCServer.h : Declaration of the COPCServer

#pragma once

#include "OPCGroup.h"

#include "resource.h"       // main symbols
#include "opcerror.h"

#include "Server_i.h"
#include "IOPCDataCallback_CP.H"

#include <map>
#include <string>
#include <memory>
#include <sstream>
#include <iostream>


//#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
//#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
//#endif

using namespace ATL;

// COPCServer

class ATL_NO_VTABLE COPCServer :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COPCServer, &CLSID_OPCServer>,
	public IDispatchImpl<IOPCServer, &IID_IOPCServer, &LIBID_ServerLib, 1, 0>,
	public IConnectionPointContainerImpl<COPCServer>,
	public CProxyIOPCDataCallback<COPCServer>
{
public:
	COPCServer()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_OPCSERVER)

	DECLARE_NOT_AGGREGATABLE(COPCServer)

	BEGIN_CATEGORY_MAP(COPCServer)
		IMPLEMENTED_CATEGORY(CATID_OPCDAServer30)
	END_CATEGORY_MAP()

	BEGIN_COM_MAP(COPCServer)
		COM_INTERFACE_ENTRY(IOPCServer)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	std::map<std::wstring, CComObjectNoLock<COPCGroup>> m_Groups;
public:




	// IOPCServer Methods
public:
	STDMETHOD(AddGroup)(LPCWSTR szName, BOOL bActive, DWORD dwRequestedUpdateRate, OPCHANDLE hClientGroup, LONG * pTimeBias, FLOAT * pPercentDeadband, DWORD dwLCID, OPCHANDLE * phServerGroup, DWORD * pRevisedUpdateRate, REFIID riid, LPUNKNOWN * ppUnk)
	{
		
		using namespace std;
		static wstring DefaultName = L"Group";
		static int DefaultNameCnt = 1;
		CComObjectNoLock<COPCGroup> *pCurrGroup=NULL;
		if (szName == NULL || szName == wstring(L""))
		{
			wstringstream strm;
			strm << DefaultName << DefaultNameCnt++;
			wstring CurrName;
			strm >> CurrName;
			pCurrGroup = &m_Groups[CurrName];
			pCurrGroup->hClientGroup = hClientGroup;
			pCurrGroup->_AtlInitialConstruct();

		}
		else if (m_Groups.find(szName) == m_Groups.end())
		{
			//CreateInstance(&m_Groups[szName]);
			pCurrGroup = &m_Groups[szName];
			pCurrGroup->hClientGroup = hClientGroup;
			pCurrGroup->_AtlInitialConstruct();
			
		}
		else 
		{
			return OPC_E_DUPLICATENAME;
		}
		//*pRevisedUpdateRate = 200;
		//*phServerGroup = 1;
		HRESULT hr = S_OK;
		if( (hr = pCurrGroup->QueryInterface(riid, (void**)ppUnk)) != S_OK)
		{
			MessageBox(NULL,L"Failed to create group",NULL,0);
			
			return E_FAIL;
		}
		//((IOPCItemMgt *)(*ppUnk))->AddItems(1, NULL, NULL, NULL);
		/*if (ppUnk != NULL)
		{
			CComPtr<IOPCServer> pOPCServer;
			pOPCServer.CoCreateInstance(CLSID_OPCServer);
			pOPCServer.CopyTo((IOPCServer**)ppUnk);
			dynamic_cast<IOPCServer*>(*ppUnk)->AddRef();
		}*/
		return S_OK;
	}
	STDMETHOD(GetErrorString)(HRESULT dwError, LCID dwLocale, LPWSTR * ppString)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(GetGroupByName)(LPCWSTR szName, REFIID riid, LPUNKNOWN * ppUnk)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(GetStatus)(OPCSERVERSTATUS ** ppServerStatus)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(RemoveGroup)(OPCHANDLE hServerGroup, BOOL bForce)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(CreateGroupEnumerator)(OPCENUMSCOPE dwScope, REFIID riid, LPUNKNOWN * ppUnk)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	BEGIN_CONNECTION_POINT_MAP(COPCServer)
		CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
	END_CONNECTION_POINT_MAP()
};

OBJECT_ENTRY_AUTO(__uuidof(OPCServer), COPCServer)