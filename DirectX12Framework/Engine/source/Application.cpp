#include "pch.h"
#include "Application.h"

#include "Render API/DirectX12/Debug/DXGIDebug.h"

//#include <windowsx.h>


namespace Engine {

	LRESULT CALLBACK WindProck(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		switch (msg)
		{

		case WM_NCCREATE: {
			LPCREATESTRUCT param = reinterpret_cast<LPCREATESTRUCT>(lparam);
			Application* pointer = reinterpret_cast<Application*>(param->lpCreateParams);

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pointer));
			std::cout << "Send create message" << std::endl;

			break;
		}

                case WM_CREATE: {

			Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			pointer->OnCreate(hwnd);

                        break;
                }

               case WM_MOUSEMOVE: {
                        Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
                        pointer->OnMouseMove(wparam, LOWORD(lparam), HIWORD(lparam));
                        break;
                }

		case WM_DESTROY: { //User pressed the Close/Exit Button
			Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			pointer->OnDestroy(); //Call OnDestroy
			PostQuitMessage(0); //Exit message with no errors
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
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

		wndClass.lpszMenuName = 0;
		wndClass.hInstance = 0;
		wndClass.lpfnWndProc = WindProck;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;

		RegisterClass(&wndClass); //Register the class with a refrence to the class

		//cant use this variable after the function returns; pointer will be assigned but not a correct handle
		mWindowHandle = CreateWindow(
			L"BaseWindowClass",				//window class name 
			L"KOSMO ENGINE WINDOW",			//window caption
			WS_OVERLAPPEDWINDOW,			//window style
			460,							//initial x position
			20,								//initial y position
			mWidth,							//initial x size
			mHeight, 							//initial y size
			0, 								//parent window handle
			0, 								//window menu handle
			0, 								//initial x position
			this
		); //'TODO: Look at the Lparam 

		if (!mWindowHandle) {
			return false;
		}

		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);

		mIsRunning = true;

		return true;
	}

        void Application::OnCreate(HWND hwnd)
        {
                std::cout << "Created the actual window" << std::endl;
                mRenderer.Initialize(hwnd, mWidth, mHeight);
                mRenderer.SetCubeWorldMatrix(
                        DirectX::XMMatrixTranslation(0.0f, 1.0f, 0.0f));

        }

	void Application::Update()
	{
		MSG message;

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			// Print the message details
			/*
			std::cout << "Message: " << message.message << std::endl;
			std::cout << "HWND: " << message.hwnd << std::endl;
			std::cout << "WPARAM: " << message.wParam << std::endl;
			std::cout << "LPARAM: " << message.lParam << std::endl;
			std::cout << "Time: " << message.time << std::endl;
			std::cout << "Point: (" << message.pt.x << ", " << message.pt.y << ")" << std::endl;
			std::cout << "---------------------------------" << std::endl;
			*/
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (mIsRunning)
		{
			mRenderer.UpdateDraw();
		}
	}

        void Application::OnDestroy()
        {
                std::cout << "Closed the window \nshutting down the application" << std::endl;

                mRenderer.Release();
                DXGIDebug::Get().GetLiveObjects();

                mIsRunning = false;
        }

        void Application::OnMouseMove(WPARAM state, int x, int y)
        {
                if (state & MK_LBUTTON)
                {
                        int dx = x - mLastMousePos.x;
                        int dy = y - mLastMousePos.y;
                        mRotX += dy * 0.01f;
                        mRotY += dx * 0.01f;
                        mRenderer.SetCubeWorldMatrix(
                                DirectX::XMMatrixTranslation(0.0f, 1.0f, 0.0f) *
                                DirectX::XMMatrixRotationX(mRotX) *
                                DirectX::XMMatrixRotationY(mRotY));
                }

                mLastMousePos.x = x;
                mLastMousePos.y = y;
        }
}