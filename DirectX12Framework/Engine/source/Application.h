#pragma once
#include "EngineMin.h"

#include <windows.h>

#include "./Render API/RenderAPI.h"

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
		
		RenderAPI mRenderer;

	private: //Variables
		HWND mWindowHandle = nullptr;
		bool mIsRunning = false; //flag to check if the application is running 

	private:
		UINT mWidth = 720;
		UINT mHeight = 720;
	};
}


