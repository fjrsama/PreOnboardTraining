#include "stdafx.h"
#include "DataCallbackSink.h"

using namespace std;


DataCallbackSink::DataCallbackSink(std::list<double> wave[3],CWnd *p)
{
	m_wave = wave;
	pWnd = p;
}


DataCallbackSink::~DataCallbackSink()
{
}


HRESULT DataCallbackSink::QueryInterface(REFIID riid, void  **ppvObject)
{
	if (riid == IID_IUnknown)
	{
		*ppvObject = static_cast<IUnknown*>(this);
		AddRef();
	}
	else if (riid == IID_IOPCDataCallback)
	{
		*ppvObject = static_cast<IOPCDataCallback*>(this);
		AddRef();
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE DataCallbackSink::AddRef()
{
	return ++m_cRef;
}

ULONG STDMETHODCALLTYPE DataCallbackSink::Release()
{
	--m_cRef;
	if (m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

//add //release

HRESULT __stdcall DataCallbackSink::OnDataChange(
	/* [in] */ DWORD dwTransid,
	/* [in] */ OPCHANDLE hGroup,
	/* [in] */ HRESULT hrMasterquality,
	/* [in] */ HRESULT hrMastererror,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ OPCHANDLE *phClientItems,
	/* [size_is][in] */ VARIANT *pvValues,
	/* [size_is][in] */ WORD *pwQualities,
	/* [size_is][in] */ FILETIME *pftTimeStamps,
	/* [size_is][in] */ HRESULT *pErrors)
{
	for (int i = 0; i < dwCount; ++i)
	{
		m_wave[i].push_back(pvValues[i].dblVal);
		::SendMessage(pWnd->m_hWnd, WM_PAINT, 0, 0);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DataCallbackSink::OnReadComplete(
	/* [in] */ DWORD dwTransid,
	/* [in] */ OPCHANDLE hGroup,
	/* [in] */ HRESULT hrMasterquality,
	/* [in] */ HRESULT hrMastererror,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ OPCHANDLE *phClientItems,
	/* [size_is][in] */ VARIANT *pvValues,
	/* [size_is][in] */ WORD *pwQualities,
	/* [size_is][in] */ FILETIME *pftTimeStamps,
	/* [size_is][in] */ HRESULT *pErrors)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DataCallbackSink::OnWriteComplete(
	/* [in] */ DWORD dwTransid,
	/* [in] */ OPCHANDLE hGroup,
	/* [in] */ HRESULT hrMastererr,
	/* [in] */ DWORD dwCount,
	/* [size_is][in] */ OPCHANDLE *pClienthandles,
	/* [size_is][in] */ HRESULT *pErrors)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DataCallbackSink::OnCancelComplete(
	/* [in] */ DWORD dwTransid,
	/* [in] */ OPCHANDLE hGroup)
{
	return E_NOTIMPL;
}

//ULONG DataCallbackSink::AddRef()
//{
//	return InternalAddRef();
//}
//ULONG DataCallbackSink::Release()
//{
//	ULONG l = InternalRelease();
//	if (l == 0)
//	{
//		// Lock the module to avoid DLL unload when destruction of member variables take a long time
//		ModuleLockHelper lock;
//		delete this;
//	}
//	return l;
//}
//
//HRESULT DataCallbackSink::QueryInterface(
//	REFIID iid,
//	_COM_Outptr_ void** ppvObject) throw()
//{
//	return _InternalQueryInterface(iid, ppvObject);
//}