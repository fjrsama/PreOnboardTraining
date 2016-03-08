// OPCGroup.h : Declaration of the COPCGroup

#pragma once
#include "resource.h"       // main symbols



#include "Server_i.h"
#include "opcerror.h"
#include "IOPCDataCallback_CP.H"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

//COPCItem
struct OPCItem
{
	VARIANT Data;
	OPCHANDLE	hServer;
	std::wstring ItemID;
};

// COPCGroup

class ATL_NO_VTABLE COPCGroup :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IOPCItemMgt, &__uuidof(IOPCItemMgt)>,
	public IConnectionPointContainerImpl<COPCGroup>,
	public CProxyIOPCDataCallback<COPCGroup>
{
public:
	COPCGroup()
	{
	}

	DECLARE_NOT_AGGREGATABLE(COPCGroup)

	BEGIN_COM_MAP(COPCGroup)
		COM_INTERFACE_ENTRY(IDispatch)
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

private:
	HandleDistributor m_HandleDistributor;
	static const std::set<std::wstring> ItemMap;
	std::list<OPCItem> m_OPCItems;

public:




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

	BEGIN_CONNECTION_POINT_MAP(COPCGroup)
		CONNECTION_POINT_ENTRY(__uuidof(IOPCDataCallback))
	END_CONNECTION_POINT_MAP()
};
const std::set<std::wstring> COPCGroup::ItemMap = { L"ItemY1",L"ItemY2",L"ItemY3" };
//OBJECT_ENTRY_AUTO(__uuidof(OPCGroup), COPCGroup)



