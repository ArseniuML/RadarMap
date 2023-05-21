#pragma once

namespace RLib {
	namespace Window {

class Window
{
public:
	Window(const LPCTSTR lpszClassName, Window* pWndShare = NULL, bool autoSetGLContext = true);
	~Window(void);

	virtual void Show(int X, int Y, int nWidth, int nHeight);
	
	inline BOOL WINAPI Hide() 
	{
		return ::ShowWindow(mHWND, SW_HIDE);
	}

	inline void GetClientRect(LPRECT lpRect) 
	{	
		::GetClientRect(mHWND,  lpRect);
	}
	
	inline BOOL WINAPI SwapBuffers() 
	{
		return ::SwapBuffers(mHDC);
	}

	inline BOOL WINAPI SetGLContext()
	{
		mHDCReset = ::wglGetCurrentDC();
		mHRCReset = ::wglGetCurrentContext();
		return ::wglMakeCurrent(mHDC, mHRC);
	}

	inline BOOL WINAPI ResetGLContext()
	{
		return ::wglMakeCurrent(mHDCReset, mHRCReset);
	}

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM	wParam,	LPARAM	lParam);

private:
	WNDCLASS mWndClass;

	HWND mHWND;

	HDC mHDC;
	HGLRC mHRC;
	
	bool mAutoSetGLContext;
	HDC mHDCReset;
	HGLRC mHRCReset;
};

	}
}
