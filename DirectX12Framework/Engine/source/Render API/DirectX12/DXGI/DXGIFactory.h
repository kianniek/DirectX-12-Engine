#pragma once
#include <wrl.h>

#include "DXGIAdapter.h"

namespace Engine {


	class DXGIFactory : Microsoft::WRL::ComPtr<IDXGIFactory>
	{
	public:
		DXGIFactory();

		DXGIAdapter GetAdapter();

	};
}

