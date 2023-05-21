#include "stdafx.h"
#include "RadarMapAsync.h"

RadarMap::RadarMapAsync::RadarMapAsync():
Window(_T("RadarMapAsync"), NULL, false),
mpRadarMapSync(NULL),
mpFrame(NULL),
mRequestCreate(false),
mRequestDestroy(false),
mRequestMap(false),
mCancelMap(false),
mStopLoading(false),
mRequestResult(false),
mEvCreated(CreateEvent(NULL, FALSE, FALSE, NULL)),
mEvMapReady(CreateEvent(NULL, TRUE, TRUE, NULL)),
mThread(CreateThread(NULL, 0, &RadarMapAsync::RunStatic, this, 0, 0))
{
	//Запрос на создание окна (объекта RadarMapSync)
	//во вспомогательном потоке
	mRequestCreate = true;
	WaitForSingleObject(mEvCreated, INFINITE);

	//Создать объект Frame для отрисовки кадра в контексте окна
	SetGLContext();
	try
	{
		mpFrame = new Frame::Frame();
	}
	catch(std::exception& ex)
	{
		mpFrame = NULL;
		cout << "RadarMap: exception " << ex.what() << endl << flush;
	}
	ResetGLContext();
}

RadarMap::RadarMapAsync::~RadarMapAsync()
{
	{
		IlmThread::Lock lock(mMutex);
		mRequestDestroy = true;
		
		mRequestMap = false;
		mStopLoading = true;
		mCancelMap = true;
	}
	
	WaitForSingleObject(mThread, INFINITE);

	CloseHandle(mEvCreated);
	CloseHandle(mEvMapReady);
	CloseHandle(mThread);

	SetGLContext();
	if (mpFrame)
	{
		delete mpFrame;
		mpFrame = NULL;
	}
	ResetGLContext();
}


void RadarMap::RadarMapAsync::Show( int X, int Y, int nWidth, int nHeight )
{
	IlmThread::Lock lock(mMutex);
	
	Window::Show(X, Y, nWidth, nHeight);
	
	SetGLContext();
	if (mpFrame)
	{
		mpFrame->SetProjectedSize(nWidth, nHeight);
	}
	glViewport(0, 0, nWidth, nHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	SwapBuffers();
	ResetGLContext();

	cout << "RadarMap: show window x = " << X << " ,y = " << Y << " ,width = " << nWidth << " ,height = " << nHeight << endl << flush;
}

void RadarMap::RadarMapAsync::RequestMap( const Request& req, const graph_29k::TActiveObjects2& aoP )
{
	IlmThread::Lock lock(mMutex);
	
	mRequest = req;
	mObjectParams = aoP;
	mRequestMap = true;
}

void RadarMap::RadarMapAsync::CancelMap()
{
	IlmThread::Lock lock(mMutex);
	
	mStopLoading = true;
	mCancelMap = true;
}

void RadarMap::RadarMapAsync::RequestResult()
{
	{
		IlmThread::Lock lock(mMutex);

		SetGLContext();
		if (mpFrame)
		{
			mpFrame->SetRequest(mRequestIn);
		}
		ResetGLContext();

		mStopLoading = true;
		mRequestResult = true;
	}
	WaitForSingleObject(mEvMapReady, INFINITE);
}

void RadarMap::RadarMapAsync::Render( float brightness )
{
	SetGLContext();
	if (mpFrame)
	{
		mpFrame->Render(brightness);
	}
	glFinish();
	SwapBuffers();
	ResetGLContext();
}

DWORD WINAPI RadarMap::RadarMapAsync::RunStatic( LPVOID lpParam )
{
	((RadarMapAsync*)lpParam)->Run();
	return 0;
}

//Цикл сообщений вспомогательного потока
void RadarMap::RadarMapAsync::Run()
{
	while (true)
	{
		if (CheckCreate())
		{
			try
			{
				mpRadarMapSync = new RadarMapSync(this);
			}
			catch(std::exception& ex)
			{
				mpRadarMapSync = NULL;
				cout << "RadarMap: exception " << ex.what() << endl << flush;
			}
			SetEvent(mEvCreated);
		}

		if (CheckDestroy())
		{
			if (mpRadarMapSync) 
			{
				delete mpRadarMapSync;
				mpRadarMapSync = NULL;
			}
			break;
		}

		if(CheckRequestMap())
		{
			if (mpRadarMapSync)
			{
				//Копируем параметры кадра в объект mpRadarMapSync
				mpRadarMapSync->SetRequest(mRequestIn);
				mpRadarMapSync->ActivateTargets(mObjectParamsIn);

				//Загружаем тайлы оптимального LODа
				mpRadarMapSync->LoadOptimalLevel(mStopLoading);

				//Строим карту
				if (!mCancelMap)	mpRadarMapSync->Build();
			}
		}

		CheckRequestResult();
	}
}

bool RadarMap::RadarMapAsync::CheckCreate()
{
	IlmThread::Lock lock(mMutex);
	if (mRequestCreate)
	{
		mRequestCreate = false;
		return true;
	}
	return false;
}

bool RadarMap::RadarMapAsync::CheckDestroy()
{
	IlmThread::Lock lock(mMutex);
	if (mRequestDestroy)
	{
		mRequestDestroy = false;
		return true;
	}
	return false;
}

bool RadarMap::RadarMapAsync::CheckRequestMap()
{
	IlmThread::Lock lock(mMutex);
	if (mRequestMap)
	{
		mRequestIn = mRequest;
		mObjectParamsIn = mObjectParams;

		mCancelMap = false;
		mStopLoading = false;

		ResetEvent(mEvMapReady);
		mRequestMap = false;
		return true;
	}
	return false;
}

bool RadarMap::RadarMapAsync::CheckRequestResult()
{
	IlmThread::Lock lock(mMutex);
	if (mRequestResult)
	{
		if (mpFrame && mpRadarMapSync)
		{
			//Копируем построеннную карту в объект mpFrame
			mpFrame->SetFrame(mpRadarMapSync->GetResult());
			glFinish();
		}

		SetEvent(mEvMapReady);
		mRequestResult = false;

		return true;
	}
	return false;
}