#include "pch.h"
#include "Application.h"

#include <windowsx.h>
#include <iostream>

namespace Engine {

	LRESULT CALLBACK WindProck(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		switch (msg)
		{

		case WM_NCCREATE: {
			std::cout << "Created a window" << std::endl;

			break;
			}
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	bool Application::Initialize()
	{
		WNDCLASS wndClass = {};
		wndClass.lpszClassName = L"BaseWindowClass";
		wndClass.style = 0;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

		wndClass.lpszMenuName = 0;
		wndClass.hInstance = 0;
		wndClass.lpfnWndProc = WindProck;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;

		RegisterClass(&wndClass); //Register the class with a refrence to the class

		//cant use this variable after the function returns; pointer will be assigned but not a correct handle
		mWindowHandle = CreateWindow(L"BaseWindowClass", L"KOSMO ENGINE WINDOW", WS_OVERLAPPEDWINDOW, 460, 20, 720, 720, 0, 0, 0, 0) ; //Look at the Lparam later

		if (!mWindowHandle) {
			return false;
		}

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

		mIsRunning = true;

		return true;
	}

	void Application::Update()
	{
	}

}