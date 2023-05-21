#pragma once

#include <radarmap_29k.h>
#include <RLib/Window/Window.h>
#include <RadarMap/RadarMapSync.h>

namespace RadarMap {

//K���� ������������ ��� 
//����������� ��������� �������� �� ���������� ����� (RequestMap)  � ������ ���������� (CancelMap)
//���������� ��������� �������� �� �����/������� ���� �� ����� (Show), ����������� ������������ ����� �� ���������������� ������ (RequestResult), ������������ ����� � ������������ �������� (Render). 

//������ ������ �������� � ���� ��������������� �����, ������ ��������� ��� �������� ������ ������� RadarMapAsync::Run().
//�� ��������������� ������ ��������� � ������������ ������ ������ RadarMapSync, ������� ��������� ���������� ��������� ������� �� ���������� �����.
//����� RadarMapAsync �������� ����������� ������ RLib::Window::Window, ����� ���� �������� ���������� OpenGL, � ������� � ���������� ��������� �����.


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