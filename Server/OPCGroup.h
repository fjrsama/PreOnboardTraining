// OPCGroup.h : Declaration of the COPCGroup

#ifndef OPCGroup_H
#define OPCGroup_H
#include "resource.h"       // main symbols

// change

#include "Server_i.h"
#include "IOPCDataCallback_CP.H"
#include "opcerror.h"
#include "stdafx.h"


#include <sstream>
#include <thread>
#include <memory>
#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

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
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IOPCItemMgt, &__uuidof(IOPCItemMgt)>,
	public IConnectionPointContainerImpl<COPCGroup>,
	public CProxyIOPCDataCallback<COPCGroup>
	
{
public:
	COPCGroup():_thread(t1,this)
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
		_thread.detach();
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
			tmpItem.hServer = m_HandleDistributor.GetNewHandle();
			tmpItem.ItemID = item.szItemID;
			tmpItem.hClient = item.hClient;
			
			m_OPCItems.push_back(tmpItem);			
		});
		/*auto iter = m_OPCItems.begin();
		for (size_t i = 0; i <m_OPCItems.size(); ++i)
		{
			hClientItems[i] = iter->hClient;
			vValues[i] = iter->Data;
			Errors[i] = S_OK;
			++iter;
		}*/

		
		//Fire_OnDataChange(0, hClientGroup, S_OK, S_OK, m_OPCItems.size(), hClientItems, vValues, wQualities, ftTimeStamps, Errors);
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

	BEGIN_CONNECTION_POINT_MAP(COPCGroup)
		CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
	END_CONNECTION_POINT_MAP()
};
const std::set<std::wstring> COPCGroup::ItemMap = { L"ItemY1",L"ItemY2",L"ItemY3" };
//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)

void t1(COPCGroup *pGroup)
{
	CoInitialize(NULL);
	int i = 5;
	while (1)
	{
		Sleep(200);
		auto iter = pGroup->m_OPCItems.begin();
		for (size_t i = 0; i <pGroup->m_OPCItems.size(); ++i)
		{
			pGroup->hClientItems[i] = iter->hClient;
			pGroup->vValues[i] = iter->Data;
			pGroup->Errors[i] = S_OK;
			++iter;
		}
		//MessageBox(0,L"123",0,0);
		pGroup->Fire_OnDataChange();
	}
	CoUninitialize();
}

#endif