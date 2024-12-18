#pragma once
#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_6.h>

namespace Engine {
	class D12Device : public Microsoft::WRL::ComPtr<ID3D12Device>
	{

	public:
		D12Device() = default;

		D12Device(IDXGIAdapter* pAdapter);

		bool Init(IDXGIAdapter* pAdapter);
	};
}

