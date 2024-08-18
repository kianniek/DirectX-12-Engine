#pragma once
#include "EngineMin.h"

namespace Engine {
	class YT_API Application
	{

	public:
		Application() = default;

		bool Initialize();

		inline bool IsRunning() { return mIsRunning; } //return flag

	private:

		bool mIsRunning = false; //flag to check if the application is running 

	};
}


