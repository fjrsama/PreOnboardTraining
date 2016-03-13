// dllmain.h: 模块类的声明。

class CSigGenModule : public ATL::CAtlDllModuleT< CSigGenModule >
{
public :
	DECLARE_LIBID(LIBID_SigGenLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIGGEN, "{E0C4F20B-6855-473C-B755-B22B95CDED9B}")
};

extern class CSigGenModule _AtlModule;
