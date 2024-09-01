#include "pch.h"
#include "D12Device.h"

namespace Engine {

	D12Device::D12Device(IDXGIAdapter* pAdapter)
	{
		Init(pAdapter);
	}

    bool D12Device::Init(IDXGIAdapter* pAdapter)
    {
        HRESULT hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_1_0_CORE, IID_PPV_ARGS(GetAddressOf()));

        if (FAILED(hr)) {
            KOSMO_EVAL_HR(hr, "Error creating D12 device");
            return false;
        }

        return true;
    }

}
