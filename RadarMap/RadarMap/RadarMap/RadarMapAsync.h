#pragma once

#include <radarmap_29k.h>
#include <RLib/Window/Window.h>
#include <RadarMap/RadarMapSync.h>

namespace RadarMap {

//Kласс предназначен для 
//асинхронной обработки запросов на построение карты (RequestMap)  и отмену построения (CancelMap)
//синхронной обработки запросов на показ/скрытие окна РЛ карты (Show), копирование построенного кадра из вспомогательного потока (RequestResult), визуализацию кадра с определенной яркостью (Render). 

//Объект класса содержит в себе вспомогательный поток, циклом сообщений для которого служит функция RadarMapAsync::Run().
//Во вспомогательном потоке создается и уничтожается объект класса RadarMapSync, который выполняет синхронную обработку запроса на построение кадра.
//Класс RadarMapAsync является наследником класса RLib::Window::Window, имеет свой контекст рендеринга OpenGL, в котором и происходит отрисовка карты.


class RadarMapAsync : public RLib::Window::Window
{
public:
	RadarMapAsync();
	~RadarMapAsync();

	void Show(int X, int Y, int nWidth, int nHeight);

	void RequestMap(const Request& req, const graph_29k::TActiveObjects2& aoP);
	void CancelMap();
	void RequestResult();

	void Render(float brightness);
	
private:
	static DWORD WINAPI RunStatic(LPVOID lpParam);
	void Run();

	bool CheckCreate();
	bool CheckDestroy();
	bool CheckRequestMap();
	bool CheckRequestResult();

	RadarMapSync* mpRadarMapSync;
	Frame::Frame* mpFrame;

	bool mRequestCreate;
	bool mRequestDestroy;
	
	bool mRequestMap;
	bool mCancelMap;
	bool mStopLoading;
	bool mRequestResult;

	Request mRequest;
	graph_29k::TActiveObjects2 mObjectParams;

	Request mRequestIn;
	graph_29k::TActiveObjects2 mObjectParamsIn;

	IlmThread::Mutex mMutex;
	
	HANDLE mEvCreated;
	HANDLE mEvMapReady;
	
	HANDLE mThread;
};

}