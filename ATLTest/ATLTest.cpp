// ATLTest.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "ATLTest_i.h"


using namespace ATL;


class CATLTestModule : public ATL::CAtlExeModuleT< CATLTestModule >
{
public :
	DECLARE_LIBID(LIBID_ATLTestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLTEST, "{ADD4D50F-9111-4FE9-BC24-E4475B7CE7FB}")
	};

CATLTestModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

