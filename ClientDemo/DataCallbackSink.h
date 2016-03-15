#ifndef DATACALLBACKSINK_H
#define DATACALLBACKSINK_H

#include "..\Server\Server_i.h"
#include <atlbase.h>
#include <atlcom.h>
#include <list>

using namespace ATL;

class ClientDemoDlg;
class  DataCallbackSink :public IOPCDataCallback

{
private:
	ULONG m_cRef;
	std::list<double> *m_wave;
	CWnd * pWnd;
public:
	//IUnknown
	STDMETHOD(QueryInterface)(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	



	STDMETHOD(OnDataChange)(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE *phClientItems,
		/* [size_is][in] */ VARIANT *pvValues,
		/* [size_is][in] */ WORD *pwQualities,
		/* [size_is][in] */ FILETIME *pftTimeStamps,
		/* [size_is][in] */ HRESULT *pErrors);

	virtual HRESULT STDMETHODCALLTYPE OnReadComplete(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE *phClientItems,
		/* [size_is][in] */ VARIANT *pvValues,
		/* [size_is][in] */ WORD *pwQualities,
		/* [size_is][in] */ FILETIME *pftTimeStamps,
		/* [size_is][in] */ HRESULT *pErrors);

	virtual HRESULT STDMETHODCALLTYPE OnWriteComplete(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMastererr,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE *pClienthandles,
		/* [size_is][in] */ HRESULT *pErrors);

	virtual HRESULT STDMETHODCALLTYPE OnCancelComplete(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup);


	DataCallbackSink(std::list<double> wave[3],CWnd *p);
	~DataCallbackSink();
};

#endif