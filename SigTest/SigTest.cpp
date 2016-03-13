// SigTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <thread>
//#include "..\SigGen\SigGen_i.h"
//#include "..\SigGen\SigGen_i.c"

using namespace std;
using namespace std::chrono;
int main()
{

	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	//CComPtr<ISignal> pSignal = NULL;
	//HRESULT hr = ::CoCreateInstance(CLSID_Signal, NULL, CLSCTX_ALL, IID_IUnknown, (void **)&pSignal);
	//if(pSignal!=NULL)
	//while (1)
	//{
	//	this_thread::sleep_for(milliseconds(200));
	//	VARIANT vt;
	//	LPWSTR str = L"ItemY1";
	//	HRESULT hr = pSignal->GetCurrentValue(str, &vt);
	//	if (hr == S_OK)
	//		cout << vt.dblVal << endl;
	//}
	CoUninitialize();
    return 0;
}

