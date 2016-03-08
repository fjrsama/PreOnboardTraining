// Test.h : Declaration of the CTest

#pragma once
#include "resource.h"       // main symbols



#include "ATLTest_i.h"
#include "ATLTest.idl"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CTest

class ATL_NO_VTABLE CTest :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTest, &CLSID_Test>,
	public IDispatchImpl<ITest, &IID_ITest, &LIBID_ATLTestLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<ITestClass1, &__uuidof(ITestClass1)>
{
public:
	CTest()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_TEST)

	DECLARE_NOT_AGGREGATABLE(CTest)

	BEGIN_COM_MAP(CTest)
		COM_INTERFACE_ENTRY(ITest)
		COM_INTERFACE_ENTRY2(IDispatch, ITestClass1)
		COM_INTERFACE_ENTRY(ITestClass1)
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



	STDMETHOD(Hello)();

	// ITestClass1 Methods
public:
};

OBJECT_ENTRY_AUTO(__uuidof(Test), CTest)
