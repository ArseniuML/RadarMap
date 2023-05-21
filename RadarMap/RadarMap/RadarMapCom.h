// Класс веализует стандартные для тренажера интерфейсы

#pragma once
#include "resource.h"       // основные символы



#include "RadarMap_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Однопотоковые COM-объекты не поддерживаются должным образом платформой Windows CE, например платформами Windows Mobile, в которых не предусмотрена полная поддержка DCOM. Определите _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA, чтобы принудить ATL поддерживать создание однопотоковых COM-объектов и разрешить использование его реализаций однопотоковых COM-объектов. Для потоковой модели в вашем rgs-файле задано значение 'Free', поскольку это единственная потоковая модель, поддерживаемая не-DCOM платформами Windows CE."
#endif

using namespace ATL;

#import "..\..\common\dll\CommonTypes.dll"  no_namespace, named_guids exclude("IRT")  exclude("ISTC")

#include <Simulation/SimManager.h>
#include <radarmap_29k.h>
#include <graph_29k.h>
#include <CommServerData.h>

#include <RadarMap/RadarMapAsync.h>

//#include <Video/TVCaptureVideoController.h>
#include <Video/HDMICaptureVideoController.h>

class ATL_NO_VTABLE RadarMapCom :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<RadarMapCom, &CLSID_RadarMapCom>,
	public IDispatchImpl<IRadarMap, &IID_IRadarMap, &LIBID_RadarMapLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IRT, &__uuidof(IRT), &LIBID_RadarMapLib, /* wMajor = */ 1, /* wMinor = */ 0>,
	public IDispatchImpl<ISTC, &__uuidof(ISTC), &LIBID_RadarMapLib, /* wMajor = */ 1, /* wMinor = */ 0>
{
public:
	RadarMapCom();

	DECLARE_REGISTRY_RESOURCEID(IDR_RADARMAPCOM)


	BEGIN_COM_MAP(RadarMapCom)
		COM_INTERFACE_ENTRY(IRadarMap)
		COM_INTERFACE_ENTRY2(IDispatch, IRT)
		COM_INTERFACE_ENTRY(IRT)
		COM_INTERFACE_ENTRY(ISTC)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	// IRT Methods
public:
	STDMETHOD(Initialize)(unsigned long time);
	STDMETHOD(Run)(unsigned long time);
	STDMETHOD(Terminate)();

	// ISTC Methods
public:
	STDMETHOD(STConnect)(IDispatch * pDisp, unsigned long ID);

private:
	ICS* mpICS;

	Simulation::SimManager mSimManager;
	radarmap_29k::CsItems& mIo;

	typedef Video::HDMICaptureVideoController VideoController;

	VideoController* mpVideoController;
	RadarMap::RadarMapAsync* mpRadarMapAsync;

	int mRequestMap;
	int mCancelMap;

	int mBrightness;
	bool mNeedsRedraw;

	bool mInitialized;
	bool mRequest;
	bool mReady;

	bool mInitialMG;
	bool mIsShown;
};

OBJECT_ENTRY_AUTO(__uuidof(RadarMapCom), RadarMapCom)

