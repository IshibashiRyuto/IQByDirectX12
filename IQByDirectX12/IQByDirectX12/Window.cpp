#include "Window.h"
#include <iostream>


Window::Window(HINSTANCE hInstance)
	: mHInst(hInstance)
{
	if (!_CreateWindowClassEx() || !_CreateWindow())
	{
		return;
	}
}


Window::~Window()
{
}

HWND Window::GetWindowHandle() const
{
	return mHWnd;
}

const RECT & Window::GetWindowRect() const
{
	return WINDOW_RECT;
}


bool Window::_CreateWindowClassEx()
{
	mWndClass = WNDCLASSEX();
	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)_WindowProc;
	mWndClass.lpszClassName = WINDOW_TITLE;
	mWndClass.hInstance = mHInst;

	if (RegisterClassEx(&mWndClass) == 0)
	{
#ifdef _DEBUG
		std::cout << "Failed RegisterClassEx" << std::endl;
#endif
		return false;
	}
	return true;
}

bool Window::_CreateWindow()
{
	// �E�B���h�E�T�C�Y�̎w��
	auto wndRect = WINDOW_RECT;
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);

	mHWnd = CreateWindow(mWndClass.lpszClassName,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		nullptr,
		nullptr,
		mWndClass.hInstance,
		nullptr);

	// �E�B���h�E�������s��
	if (mHWnd == NULL)
	{
#ifdef _DEBUG
		std::cout << "Failed Create Window" << std::endl;
#endif
		return false;
	}

	// �E�B���h�E�\��
	ShowWindow(mHWnd, SW_SHOW);

	return true;
}

LRESULT _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		//�I������
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
