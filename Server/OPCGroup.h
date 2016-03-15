// OPCGroup.h : Declaration of the COPCGroup

#pragma once

#include "resource.h"       // main symbols



#include "Server_i.h"
#include "IOPCDataCallback_CP.H"
#include "opcerror.h"
#include "stdafx.h"
#include "..\SigGen\SigGen_i.h"
#include "..\SigGen\SigGen_i.c"
#include <sstream>
#include <thread>
#include <memory>


//#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
//#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
//#endif

using namespace ATL;

//COPCItem
struct OPCItem
{
	VARIANT Data;
	OPCHANDLE	hServer;
	OPCHANDLE hClient;
	std::wstring ItemID;
};

// COPCGroup
class COPCGroup;
void t1(COPCGroup*);

class ATL_NO_VTABLE COPCGroup :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IDispatchImpl<IOPCItemMgt, &__uuidof(IOPCItemMgt)>,
	public IConnectionPointContainerImpl<COPCGroup>,
	public CProxyIOPCDataCallback<COPCGroup>
	
{
public:
	COPCGroup() :_thread(std::mem_fun(&COPCGroup::t1), this)
	{
		for (int i = 0; i < 100; ++i)
		{
			hClientItems[i] = 0;
			vValues[i].vt = VT_R8;
			vValues[i].dblVal = 0.0;
			wQualities[i] = 0;

			ftTimeStamps[i].dwHighDateTime = 0;
			ftTimeStamps[i].dwLowDateTime = 0;
			Errors[i] = 0;
		}
	}
	~COPCGroup()
	{
		_thread.join();
	}


	BEGIN_COM_MAP(COPCGroup)
		COM_INTERFACE_ENTRY2(IDispatch, IOPCItemMgt)
		COM_INTERFACE_ENTRY(IOPCItemMgt)
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

public:
	HandleDistributor m_HandleDistributor;
	HandleDistributor m_MarshallCookieDistributor;
	static const std::set<std::wstring> ItemMap;
	std::list<OPCItem> m_OPCItems;
	OPCHANDLE hClientGroup;
	friend class COPCServer;

	OPCHANDLE hClientItems[100];
	VARIANT vValues[100];
	WORD wQualities[100];
	FILETIME ftTimeStamps[100];
	HRESULT Errors[100];
	IStream *pStm = NULL;
	std::thread _thread;
	std::map<DWORD, IStream*> m_MarshallMap;
	
	// IOPCItemMgt Methods
public:
	STDMETHOD(AddItems)(DWORD dwCount, OPCITEMDEF * pItemArray, OPCITEMRESULT ** ppAddResults, HRESULT ** ppErrors)
	{
		// Add your function implementation here.
		
		using namespace std;
		for_each(pItemArray, pItemArray + dwCount, [&](OPCITEMDEF & item)
		{
			if (ItemMap.find(item.szItemID) == ItemMap.end())
			{
				*ppErrors = new HRESULT(OPC_E_UNKNOWNITEMID);
				return E_FAIL;
			}
			OPCItem tmpItem;
			tmpItem.Data.vt = item.vtRequestedDataType;
			tmpItem.Data.dblVal = 0.1;
			tmpItem.hServer = m_HandleDistributor.GetNewHandle();
			tmpItem.ItemID = item.szItemID;
			tmpItem.hClient = item.hClient;			
			m_OPCItems.push_back(tmpItem);			
		});
		return S_OK;
	}
	STDMETHOD(ValidateItems)(DWORD dwCount, OPCITEMDEF * pItemArray, BOOL bBlobUpdate, OPCITEMRESULT ** ppValidationResults, HRESULT ** ppErrors)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(RemoveItems)(DWORD dwCount, OPCHANDLE * phServer, HRESULT ** ppErrors)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(SetActiveState)(DWORD dwCount, OPCHANDLE * phServer, BOOL bActive, HRESULT ** ppErrors)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(SetClientHandles)(DWORD dwCount, OPCHANDLE * phServer, OPCHANDLE * phClient, HRESULT ** ppErrors)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(SetDatatypes)(DWORD dwCount, OPCHANDLE * phServer, VARTYPE * pRequestedDatatypes, HRESULT ** ppErrors)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}
	STDMETHOD(CreateEnumerator)(REFIID riid, LPUNKNOWN * ppUnk)
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}


	void Fire_OnDataChange()
	{
		CProxyIOPCDataCallback::Fire_OnDataChange(0, hClientGroup, S_OK, S_OK, m_OPCItems.size(), hClientItems, vValues, wQualities, ftTimeStamps, Errors);
	}


	STDMETHODIMP Advise(
		_Inout_ IUnknown* pUnkSink,
		_Out_ DWORD* pdwCookie)
	{
		//HRESULT hr = IConnectionPointImpl::Advise(pUnkSink, pdwCookie);
		//return hr;
		IUnknown* p = NULL;
		HRESULT hRes = S_OK;
		if (pdwCookie != NULL)
			*pdwCookie = 0;
		if (pUnkSink == NULL || pdwCookie == NULL)
			return E_POINTER;
		IID iid;
		GetConnectionInterface(&iid);
		hRes = pUnkSink->QueryInterface(iid, (void**)&p);
		if (SUCCEEDED(hRes))
		{
			Lock();
			IStream *pStream = NULL;
			CoMarshalInterThreadInterfaceInStream(iid, p, &pStream);
			if (pStream)
			{
				*pdwCookie = m_MarshallCookieDistributor.GetNewHandle();
				m_MarshallMap[*pdwCookie] = pStream;
			}
			hRes = (*pdwCookie != NULL) ? S_OK : CONNECT_E_ADVISELIMIT;
			Unlock();

			if (hRes != S_OK)
				p->Release();
		}
		else if (hRes == E_NOINTERFACE)
			hRes = CONNECT_E_CANNOTCONNECT;
		if (FAILED(hRes))
			*pdwCookie = 0;
		return hRes;

	}

	STDMETHODIMP Unadvise(_In_ DWORD dwCookie)
	{
		HRESULT hRes = S_OK;
		IID iid;
		GetConnectionInterface(&iid);
		Lock();
		IUnknown* p = NULL;
		LPSTREAM pStream = m_MarshallMap[dwCookie];
		if (pStream!=NULL)
		{
			CoGetInterfaceAndReleaseStream(pStream, iid, (void**)&p);
			m_MarshallMap.erase(dwCookie);
			m_MarshallCookieDistributor.ReleaseHandle(dwCookie);
			hRes = p == NULL ? CONNECT_E_NOCONNECTION : S_OK;
		}
		Unlock();
		if (hRes == S_OK && p != NULL)
			p->Release();
		return hRes;
	}

	void t1()
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		CComPtr<ISignal> pSignal;
		pSignal.CoCreateInstance(CLSID_Signal);


		while (m_dwRef>=0)
		{
			if (m_OPCItems.size() <= 0) continue;
			Sleep(60);
			if (m_OPCItems.size() <= 0) continue;
			Lock();
			auto iter = m_OPCItems.begin();
			for (size_t i = 0; i <m_OPCItems.size(); ++i)
			{
				pSignal->GetCurrentValue(iter->ItemID.c_str(), &iter->Data);
				hClientItems[i] = iter->hClient;
				vValues[i] = iter->Data;
				Errors[i] = S_OK;
				++iter;
			}
			Fire_OnDataChange();
			Unlock();
		}
		CoUninitialize();
	}

	BEGIN_CONNECTION_POINT_MAP(COPCGroup)
		CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
	END_CONNECTION_POINT_MAP()
};
const std::set<std::wstring> COPCGroup::ItemMap = { L"ItemY1",L"ItemY2",L"ItemY3" };
//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)

//void t1(COPCGroup *pGroup)
//{
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//	while (pGroup->m_dwRef > 0)
//	{
//		if (pGroup->m_OPCItems.size() <= 0) continue;
//		Sleep(200);
//		if (pGroup->m_OPCItems.size() <= 0) continue;
//		
//		pGroup->Lock();
//		auto iter = pGroup->m_OPCItems.begin();
//		for (size_t i = 0; i <pGroup->m_OPCItems.size(); ++i)
//		{
//			pGroup->hClientItems[i] = iter->hClient;
//			pGroup->vValues[i] = iter->Data;
//			pGroup->Errors[i] = S_OK;
//			++iter;
//		}
//		pGroup->Fire_OnDataChange();
//		pGroup->Unlock();
//	}
//	MessageBox(NULL, 0, L"end", 0);
//	CoUninitialize();
//}
