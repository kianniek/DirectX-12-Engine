#pragma once

#include <wrl.h>

namespace Engine
{
	class D12Resource : public Microsoft::WRL::ComPtr<ID3D12Resource>
	{
	public:
		D12Resource() = default;
		~D12Resource();
               void Initialize(ID3D12Device* pDevice, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);

               void InitializeAsDepthBuffer(ID3D12Device* pDevice, const unsigned int width, const unsigned int height);

               void InitializeAsTexture2D(ID3D12Device* pDevice, unsigned int width, unsigned int height,
                       DXGI_FORMAT format, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);
		void Release();
		void* GetCPUMemory();
	private:
		void* mMemory = nullptr;

	};
}

