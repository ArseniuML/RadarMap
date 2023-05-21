#include "stdafx.h"
#include "RadarMapCom.h"

#include <RadarMap/Timer.h>

RadarMapCom::RadarMapCom():
mIo(radarmap_29k::Dependency<radarmap_29k::CsItems>::Fetch()),
mpRadarMapAsync(NULL),
mInitialized(false),
mIsShown(false)
{
}

STDMETHODIMP RadarMapCom::Initialize(unsigned long time)
{
	try
	{
		char msgbuf [1024] = ""; 
		mgGetLastError(msgbuf, 1024);
		mInitialMG = !strlen(msgbuf);
		if (mInitialMG) 
		{
			mgInit(NULL, NULL);
			cout << "RadarMap: mg api initialized" << endl << flush;
		}

		appConfig.Load("RadarMap.ini");  //Параметры приложения
		techConfig.Load("RadarMap.config"); //Параметры моделируемого радара

		mpRadarMapAsync = new RadarMap::RadarMapAsync();
		mpVideoController = new VideoController(mIo, *mpRadarMapAsync);

		mRequestMap = 0;
		mCancelMap = 0;
		mBrightness = 0;

		mNeedsRedraw = false;
		mRequest = false;
		mReady = true;
		mInitialized = true;

		USES_CONVERSION;

		cout << "RadarMap: Initialized" << endl << flush;
	}
	catch(std::exception& ex)
	{
		cout << "RadarMap: exception in Initialize!" << endl;
		cout << "RadarMap: " << ex.what() << endl << flush;
	}
	catch(...)
	{
		cout << "RadarMap: unknown exception in Initialize!" << endl << flush;
	}

	return S_OK;
}

STDMETHODIMP RadarMapCom::Run(unsigned long time)
{
	if (!mInitialized) return S_OK;

	try
	{
		mSimManager.BeginTact(time);
	
		mpVideoController->Check();

		radarmap_29k::TRadarMapOut	radarParams;
		RadarMap::RequestData requestData;
		RadarMap::Request request;
		graph_29k::TActiveObjects2	activeObjects;

		radarParams = mIo.Targeting.Radar.Map.Control.Out;
		RadarMap::Timer::SetTimeToUpdate(radarParams.TimeToUpdate);

		if (radarParams.RequestMap != mRequestMap)
		{
			//Запрос на построение РЛ карты
			
			cout << "RadarMap: request" << endl << flush;

			mRequestMap = radarParams.RequestMap;
			mCancelMap = radarParams.CancelMap;

			mRequest = true;

			if (!appConfig.Debug.StaticRun)
			{
				requestData.Airplane.X = radarParams.Airplane.X;
				requestData.Airplane.Y = radarParams.Airplane.Y;
				requestData.Airplane.Z = radarParams.Airplane.Z;
				requestData.Airplane.Height = radarParams.Airplane.Height;

				requestData.Airplane.Heading = radarParams.Airplane.Heading;
				requestData.Airplane.Pitch = radarParams.Airplane.Pitch;
				requestData.Airplane.Roll = radarParams.Airplane.Roll;

				requestData.Mode = mIo.Targeting.Radar.Map.Mode;

				requestData.DMin = radarParams.DMin;
				requestData.DMax = radarParams.DMax;

				requestData.AzimuthLeft = radarParams.AzimuthLeft;
				requestData.AzimuthRight = radarParams.AzimuthRight;

				requestData.DCenter = radarParams.DCenter;
				requestData.MapWidth = radarParams.MapWidth;
				requestData.MapHeight = radarParams.MapHeight;
			}
			else
			{
				requestData.Airplane.X = -289882.7;
				requestData.Airplane.Y = 2225.8;
				requestData.Airplane.Z = -38513.7;
				requestData.Airplane.Height = 2228.4;

				requestData.Airplane.Heading = -0.132816;
				requestData.Airplane.Pitch = 0.347064;
				requestData.Airplane.Roll = -0.015528;

				requestData.Mode = 1;

				requestData.DMin = 30709.2;
				requestData.DMax = 119325.8;

				requestData.AzimuthLeft = 0.79;
				requestData.AzimuthRight = -0.79;

				requestData.DCenter = 75000;
				requestData.MapWidth = 88676;
				requestData.MapHeight = 88676;
			}

			request.SetRequest(requestData);

			activeObjects = mIo.SCENARIO_OBJECTS_DATA;
			mpRadarMapAsync->RequestMap(request, activeObjects);
		}
		else if (radarParams.CancelMap != mCancelMap)
		{
			//Отмена запроса на построение РЛ карты

			cout << "RadarMap: cancel" << endl << flush;

			mCancelMap = radarParams.CancelMap;
			mRequest = false;

			mpRadarMapAsync->CancelMap();
		}
		else if (radarParams.TimeToUpdate <= 0  && mRequest)
		{
			//Закончилось время построения - запрос на отрисовку
			//Копируем построенную карту в основной поток
			//и устанавливаем флаг "необходима отрисовка"
			
			int mapReady = mIo.Targeting.Radar.Map.Control.Ready;
			++mapReady;

			mIo.Targeting.Radar.Map.Control.Ready = mapReady;
			mRequest = false;
			
			mpRadarMapAsync->RequestResult();  

			cout << "RadarMap: request result" << endl << flush;
			mNeedsRedraw = true;
		}

		if (radarParams.Brightness != mBrightness)
		{
			//Яркость меняется в реальном времени, поэтому 
			//положение ручки яркости отслеживается постоянно
			//Если оно изменилось, устанавливаем флаг "необходима отрисовка"

			mBrightness = radarParams.Brightness;
			mNeedsRedraw = true;
		}

		if (mNeedsRedraw)
		{
			//Отрисовка карты в основном потоке
			//с текущим значением яркости

			if (appConfig.Debug.StaticRun)
			{
				radarParams.Brightness = 100;
			}
			cout << "RadarMar: brightness " << radarParams.Brightness << endl << flush; 
			mpRadarMapAsync->Render(radarParams.Brightness);
			mNeedsRedraw = false;
		}

		mSimManager.EndTact();
	}
	catch(Simulation::CsError& ex)
	{
		cout << "RadarMap: exception in Run CS" << ex.what() << endl << flush;

	}
	catch(Simulation::DependencyError& ex)
	{
		cout << "RadarMap: exception in Run CS" << ex.what() << endl << flush;
	}	
	catch(std::exception& ex)
	{
		cout << "RadarMap: exception in Run " << ex.what() << endl << flush;
	}
	catch(...)
	{
		cout << "RadarMap: unknown exception in Run" << endl << flush;
	}

	return  S_OK;
}

STDMETHODIMP RadarMapCom::Terminate()
{
	if (!mInitialized) return S_OK;
	mInitialized = false;
	try
	{	 
		delete mpVideoController;
		delete mpRadarMapAsync;
		if (mInitialMG) mgExit();
		cout << "RadarMap: Terminate" << endl <<flush;
	}
	catch(std::exception& ex)
	{
		cout << "RadarMap: exception in Terminate!" << endl;
		cout << "RadarMap: " << ex.what() << endl << flush;
	}
	catch(...)
	{
		cout << "RadarMap:  unknown exception in Terminate!" << endl << flush;
	}

	return S_OK;
}

STDMETHODIMP RadarMapCom::STConnect(IDispatch *pDisp, unsigned long ID)
{ 	
	return mSimManager.STConnect(pDisp, ID);
	cout << "RadarMap: STConnect" << endl << flush;
}