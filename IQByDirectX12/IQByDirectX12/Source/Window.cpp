#include "Window.h"
#include <iostream>
#include "Debug\DebugLayer.h"

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
		DebugLayer::GetInstance().PrintDebugMessage("Failed RegisterClassEx");
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
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create Window");
		return false;
	}

	// �E�B���h�E�\��
	ShowWindow(mHWnd, SW_SHOW);

	return true;
}

// ImGui_ImplWin32.cpp�Œ�`�ς݂�ImGui�p�v���V�[�W�����֐����g�p
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT _WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
