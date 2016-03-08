// ServerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "..\Server\Server_i.h"
#include "..\Server\Server_i.c"
#include <exception>
#include "atlcomcli.h" 
#include "DataCallbackSink.h"
using namespace std;


int main()
{
	CoInitialize(NULL);


	CComPtr<IOPCServer> pOPCServer;
	pOPCServer.CoCreateInstance(CLSID_OPCServer);
	if (pOPCServer != NULL)
	{
		LONG TimeBias = 0;
		FLOAT DeadBand = 0.0f; 
		OPCHANDLE phServerGroup;
		DWORD UpdateRate;

		IOPCItemMgt *tmpOPCItemMgt;
		pOPCServer->AddGroup(L"Group1", TRUE, 200, 1, &TimeBias, &DeadBand, 0, &phServerGroup, &UpdateRate,IID_IOPCItemMgt, (LPUNKNOWN*)&tmpOPCItemMgt);
		CComPtr<IOPCItemMgt> pOPCItemMgt = tmpOPCItemMgt;
		
		DWORD dwCookie;

		CComModule _Module;
		
		_Module.Init(NULL, GetModuleHandle(NULL));
		

		DataCallbackSink *pSink = new DataCallbackSink;


		IConnectionPoint *pCP = NULL;


		IConnectionPointContainer *pCPC = NULL;


		HRESULT hr = pOPCItemMgt->QueryInterface(IID_IConnectionPointContainer, (void **)&pCPC);


		if (pCPC != NULL)
		{
			hr = pCPC->FindConnectionPoint(IID_IOPCDataCallback, &pCP);
			pCPC->Release();
		}

		if (pCP != NULL)
		{
			hr = pCP->Advise(pSink, &dwCookie);
		}
		OPCITEMDEF OPCItem = {
			NULL,
			L"ItemY1",
			FALSE,
			0,
			0,
			NULL,
			VT_R8,
			0
		};
		OPCITEMRESULT *OPCItemResult;
		HRESULT *ErrorResult;
		pOPCItemMgt->AddItems(1, &OPCItem, &OPCItemResult, &ErrorResult);
		while (1);
		pCP->Unadvise(dwCookie);
		delete pSink;
		_Module.Term();
	}
	else
	{
		std::cout << "failed to connect to the OPC server";
	}
    return 0;
}

