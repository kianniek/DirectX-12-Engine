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

                void OnMouseMove(WPARAM state, int x, int y);

                void LoadTexture(const wchar_t* path) { mRenderer.LoadTextureFromFile(path); }

		inline bool IsRunning() { return mIsRunning; } //return flag


	private: 
		
		RenderAPI mRenderer;

        private: //Variables
                HWND mWindowHandle = nullptr;
                bool mIsRunning = false; //flag to check if the application is running

                POINT mLastMousePos{0,0};
                float mRotX = 0.0f;
                float mRotY = 0.0f;

	private:
		UINT mWidth = 960;
		UINT mHeight = 540;
	};
}


