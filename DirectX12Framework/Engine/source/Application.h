#pragma once

#include <windows.h>

#include "EngineMin.h"

namespace Engine {
	class KOSMO_API Application
	{

	public:
		Application() = default;


		bool Initialize();

		void OnCreate(HWND hwnd);

		void Update();

		void OnDestroy();

		inline bool IsRunning() { return mIsRunning; } //return flag


	private:
		HWND mWindowHandle = nullptr;
		bool mIsRunning = false; //flag to check if the application is running 

	};
}


