// Signal.h : CSignal 的声明

#pragma once
#include "resource.h"       // 主符号
#include "SigGen_i.h"
#include <string>
#include <chrono>
#include <cmath>
using namespace ATL;
using namespace std::chrono;
using namespace std;
  

// CSignal

class ATL_NO_VTABLE CSignal :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSignal, &CLSID_Signal>,
	public ISignal
{
public:
	CSignal()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SIGNAL)

	DECLARE_NOT_AGGREGATABLE(CSignal)

	BEGIN_COM_MAP(CSignal)
		COM_INTERFACE_ENTRY(ISignal)
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
	//初始0时刻时间点
	system_clock::time_point m_tpStart = system_clock::now();

	// ISignal Methods
public:
	STDMETHOD(GetCurrentValue)(LPCWSTR szSigID, VARIANT * pvtData)
	{
		// 在此处添加函数实现。
		static const double T1 = 2.0;
		static const double T2 = 2.0;
		static const double T3 = 2.0;
		static const double K1 = 50.0;
		static const double K2 = 50.0;
		static const double K3 = 50.0;

		static const double pi = 3.1415926;

		//获取当前时间点，单位为秒
		double t = (system_clock::now() - m_tpStart).count() / 10000000.0;

		if (t >= 12)
			m_tpStart += seconds(12);

		pvtData->vt = VT_R8;

		if (szSigID == wstring(L"ItemY1"))
		{
			pvtData->dblVal = K1 * sin(2 * pi*t / T1);
		} 
		else if(szSigID == wstring(L"ItemY2"))
		{
			long long N = t / T2;

			if ((t / T2 - N) < 0.5)
				pvtData->dblVal = K2;
			else
				pvtData->dblVal = -K2;

		}
		else if (szSigID == wstring(L"ItemY3"))
		{
			long long N = t / T3;

			if ((t / T3 - N) < 0.5)
				pvtData->dblVal = K3 + 2 * K3 / T3*(t-N*T3);
			else
				pvtData->dblVal = K3 + 2 * K3 / T3*((N+1)*T3-t);
		}
		else
		{
			return ERROR;
		}
			
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(Signal), CSignal)
