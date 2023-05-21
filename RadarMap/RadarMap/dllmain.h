// dllmain.h: объ€вление класса модул€.

class CRadarMapModule : public ATL::CAtlDllModuleT< CRadarMapModule >
{
public :
	DECLARE_LIBID(LIBID_RadarMapLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_RADARMAP, "{F8D41707-EB1D-45B0-B3B5-8030CD3BB282}")
};

extern class CRadarMapModule _AtlModule;
