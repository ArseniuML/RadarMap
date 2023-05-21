#include "stdafx.h"
#include "Window.h"

#include <iostream>

RLib::Window::Window::Window(const LPCTSTR lpszClassName, Window* pWndShare, bool autoSetGLContext):
mAutoSetGLContext(autoSetGLContext)
{
	memset (&mWndClass, 0, sizeof(mWndClass));
	
	mWndClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	mWndClass.lpszClassName	= lpszClassName;
	mWndClass.lpfnWndProc = (WNDPROC) WndProc;
	if (!RegisterClass(&mWndClass)) 
		throw std::exception("Can't register window class");

	mHWND = CreateWindow
	(
		lpszClassName, 
		lpszClassName, 
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP, 
		0, 
		0, 
		0, 
		0, 
		NULL, 
		NULL, 
		0, 
		NULL
	);
	if (!mHWND) 	throw std::exception("Can't create window");

	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// ������ ���� ���������
		1,				// ����� ������ (?)
		PFD_DRAW_TO_WINDOW |// ������ ��� ����
		PFD_SUPPORT_OPENGL |// ������ ��� OpenGL
		PFD_DOUBLEBUFFER,// ������ ��� �������� ������
		PFD_TYPE_RGBA,	// ��������� RGBA ������
		24,				// ����� 24 ��� ������� �����
		0, 0, 0, 0, 0, 0,// ������������� �������� ����� (?)
		0,				// ��� ������ ������������
		0,				// ��������� ��� ������������ (?)
		0,				// ��� ������ �����������
		0, 0, 0, 0,		// ���� ����������� ������������ (?)
		32,				// 32 ������ Z-����� (����� �������)  
		0,				// ����� ���������
		0,				// ��� ��������������� ������� (?)
		PFD_MAIN_PLANE,	// ������� ���� ���������
		0,				// ������ (?)
		0, 0, 0			// ����� ���� ������������ (?)
	};

	mHDC = GetDC(mHWND);
	
	int nPixelFormat = ChoosePixelFormat(mHDC, 	&pfd);
	if (!nPixelFormat)	 throw std::exception("Can't find a suitable pixel format");
	
	if (!SetPixelFormat(mHDC, nPixelFormat, &pfd)) throw std::exception("Can't set the pixel format");

	mHRC = wglCreateContext(GetDC(mHWND));
	if (!mHRC) throw std::exception("Can't create a GL rendering context");

	if (pWndShare) 
		if (!wglShareLists(mHRC, pWndShare->mHRC))
			throw
				std::exception("Can't share GL rendering contexts");

	if (mAutoSetGLContext) SetGLContext();

	std::cout << "RadarMap: Window HDC " << mHDC << " HRC " << mHRC << std::endl << std::flush; 
}

RLib::Window::Window::~Window(void)
{
	if (mAutoSetGLContext) ResetGLContext();
	wglDeleteContext(mHRC);
	
	ReleaseDC(mHWND, mHDC);
	DestroyWindow(mHWND);
	
	UnregisterClass(mWndClass.lpszClassName, 0);
}

#ifdef _DEBUG
#define RADAR_WINDOW_POSITION HWND_TOP
#else
#define RADAR_WINDOW_POSITION HWND_TOP
#endif

void RLib::Window::Window::Show(int X, int Y, int nWidth, int nHeight)
{
	SetWindowPos
	(
		mHWND,	
		RADAR_WINDOW_POSITION,
		X, 
		Y, 
		nWidth,  
		nHeight, 
		SWP_SHOWWINDOW
	);
	ShowWindow(mHWND, SW_SHOW);
	UpdateWindow(mHWND);
	SetActiveWindow(mHWND);
}

LRESULT CALLBACK RLib::Window::Window::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_SETCURSOR:
		SetCursor(NULL);
		return 0;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

