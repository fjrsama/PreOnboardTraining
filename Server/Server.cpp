// Server.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "Server_i.h"

using namespace ATL;


class CServerModule : public ATL::CAtlExeModuleT< CServerModule >
{
public :
	DECLARE_LIBID(LIBID_ServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SERVER, "{5FC6ED2D-261F-4E5B-AFB7-A6283B6082CF}")
	};

CServerModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

