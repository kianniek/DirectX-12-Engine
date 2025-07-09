#include "pch.h"
#include "RenderAPI.h"

#include <vector>

#include "RenderDataTypes.h"
#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/DXGI/DXGIAdapter.h"

#include "DirectX12/Debug/D12Debug.h"

#include "DirectX12/Pipeline/HLSLShader.h"
#include "DirectX12/Pipeline/D12RootSignature.h"

#include "Utilities/Utilities.h"

namespace Engine {
	using namespace Render;

	RenderAPI::~RenderAPI()
	{
		Release();
	}

	void RenderAPI::Initialize(HWND hwnd, const UINT width, const UINT height)
	{
		mWidth = width;
		mHeight = height;


		//TODO: this could be disabled during non-debug builds (should be coupled with the use of the DebugLayer)
		D12Debug::Get().Enable();

		//Don't need to release these components since it's done for us
		DXGIFactory factory;
		DXGIAdapter adapter = factory.GetAdapter();

		/*Console Output For Debugging*/
		{
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);
			PRINT_W_N("Selected device " << desc.Description)
		}
		/*End Debugging Output*/

		bool hasCreatedDevice = mDevice.Init(adapter.Get());
		mDevice->SetName(L"Main virtual device");

		mCommandQueue.Initialize(mDevice.Get());
		mCommandList.Initialize(mDevice.Get());

		mSwapChain.Initialize(mDevice.Get(), factory.Get(), mCommandQueue.Get(), hwnd, mWidth, mHeight);

               mDynamicVertexBuffer.Initialize(mDevice.Get(), KBs(16), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
               mDynamicVertexBuffer.Get()->SetName(L"Dynamic vertex buffer");

                mTextureDescHeap.InitializeSRVHeap(mDevice.Get());
                {
                        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                        srvDesc.Texture2D.MipLevels = 1;
                        mDevice->CreateShaderResourceView(nullptr, &srvDesc, mTextureDescHeap->GetCPUDescriptorHandleForHeapStart());
                }

		std::vector<Vertex> vertices;

#define G_BOX_VERTICES 18

                Vertex vertecesBox[G_BOX_VERTICES];

		// Front face
                vertecesBox[0].position = { -1.0f, -1.0f, -1.0f };
                vertecesBox[0].color = { 0.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[0].uv = {0.0f, 1.0f};
                vertecesBox[1].position = { -1.0f, 1.0f, -1.0f };
                vertecesBox[1].color = { 0.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[1].uv = {0.0f, 0.0f};
                vertecesBox[2].position = { 1.0f, -1.0f, -1.0f };
                vertecesBox[2].color = { 0.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[2].uv = {1.0f, 1.0f};

                vertecesBox[3].position = { -1.0f, 1.0f, -1.0f };
                vertecesBox[3].color = { 0.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[3].uv = {0.0f, 0.0f};
                vertecesBox[4].position = { 1.0f, 1.0f, -1.0f };
                vertecesBox[4].color = { 0.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[4].uv = {1.0f, 0.0f};
                vertecesBox[5].position = { 1.0f, -1.0f, -1.0f };
                vertecesBox[5].color = { 0.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[5].uv = {1.0f, 1.0f};

		// Back face
                vertecesBox[6].position = { -1.0f, -1.0f, 1.0f };
                vertecesBox[6].color = { 1.0f, 0.0f, 0.0f, 1.0f };
                vertecesBox[6].uv = {0.0f, 1.0f};
                vertecesBox[7].position = { -1.0f, 1.0f, 1.0f };
                vertecesBox[7].color = { 1.0f, 1.0f, 0.0f, 1.0f };
                vertecesBox[7].uv = {0.0f, 0.0f};
                vertecesBox[8].position = { 1.0f, -1.0f, 1.0f };
                vertecesBox[8].color = { 1.0f, 0.0f, 0.0f, 1.0f };
                vertecesBox[8].uv = {1.0f, 1.0f};

                vertecesBox[9].position = { -1.0f, 1.0f, 1.0f };
                vertecesBox[9].color = { 1.0f, 0.0f, 0.0f, 1.0f };
                vertecesBox[9].uv = {0.0f, 0.0f};
                vertecesBox[10].position = { 1.0f, 1.0f, 1.0f };
                vertecesBox[10].color = { 1.0f, 0.0f, 0.0f, 1.0f };
                vertecesBox[10].uv = {1.0f, 0.0f};
                vertecesBox[11].position = { 1.0f, -1.0f, 1.0f };
                vertecesBox[11].color = { 1.0f, 0.0f, 0.0f, 1.0f };
                vertecesBox[11].uv = {1.0f, 1.0f};

		// Side face
                vertecesBox[12].position = { -1.0f, -1.0f, 1.0f };
                vertecesBox[12].color = { 0.0f, 0.0f, 1.0f, 1.0f };
                vertecesBox[12].uv = {0.0f, 1.0f};
                vertecesBox[13].position = { -1.0f, 1.0f, 1.0f };
                vertecesBox[13].color = { 0.0f, 0.0f, 1.0f, 1.0f };
                vertecesBox[13].uv = {0.0f, 0.0f};
                vertecesBox[14].position = { -1.0f, -1.0f, -1.0f };
                vertecesBox[14].color = { 0.0f, 0.0f, 1.0f, 1.0f };
                vertecesBox[14].uv = {1.0f, 1.0f};

                vertecesBox[15].position = { -1.0f, 1.0f, 1.0f };
                vertecesBox[15].color = { 0.0f, 0.0f, 1.0f, 1.0f };
                vertecesBox[15].uv = {0.0f, 0.0f};
                vertecesBox[16].position = { -1.0f, 1.0f, -1.0f };
                vertecesBox[16].color = { 0.0f, 0.0f, 1.0f, 1.0f };
                vertecesBox[16].uv = {1.0f, 0.0f};
                vertecesBox[17].position = { -1.0f, -1.0f, -1.0f };
                vertecesBox[17].color = { 0.0f, 0.0f, 1.0f, 1.0f };
                vertecesBox[17].uv = {1.0f, 1.0f};



		for (int i = 0; i < 3; i++) {
			Vertex vertexData;
			vertexData.color = { 0.0f,1.0f,0.0f,1.0f };

			if (i == 0) {
				vertexData.position = { -.5f,-.5f,0.0f };
			}
			else if (i == 1) {
				vertexData.position = { 0.0f,.5f,0.0f };
			}
			else {
				vertexData.position = { .5f,-.5f,0.0f };

			}
			vertices.push_back(vertexData);
		}



		memcpy(mDynamicVertexBuffer.GetCPUMemory(), vertecesBox, sizeof(Vertex) * G_BOX_VERTICES);

		mDynamicVBView.BufferLocation = mDynamicVertexBuffer.Get()->GetGPUVirtualAddress();
		mDynamicVBView.StrideInBytes = sizeof(Vertex);
		mDynamicVBView.SizeInBytes = KBs(16);

		D12RootSignature rstest;
		rstest.Initialize(mDevice.Get());

		/*
		 * Create shader programs
		 * - Wrapper for the shaders and their compilations
		 * - Create the actual shaders/programs
		 *
		 * Setup two input layouts (one for vertex/index buffers + one for datastructures needed for the pipeline/shader programs)
		 * - The pipeline input state
		 * -- Wrapper?
		 * - The root signature
		 * --Wrapper
		 *
		 * Set up the actual pipeline
		 * - Wrapper
		 * -- Set parameters
		 * - Create the functionality that couples everything into a complete pipeline
		 */


		/*
		 * ONLY CPU = default ram / cache
		 * ONLY GPU =  default heap on GPU (VRAM)
		 * Shared CPU and GPU = with rad/write for all - It's stored in the GPU
		 * Read-back memory on GPU (with read on the CPU)
		 */

		mBasePipeline.Initialize(mDevice.Get());

		mDepthBuffer.InitializeAsDepthBuffer(mDevice.Get(), width, height);

		mDepthDescHeap.InitializeDepthHeap(mDevice.Get());

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		mDevice->CreateDepthStencilView(mDepthBuffer.Get(), &dsvDesc, mDepthDescHeap.Get()->GetCPUDescriptorHandleForHeapStart());

		mViewport.TopLeftX = 0;
		mViewport.TopLeftY = 0;
		mViewport.Width = mWidth;
		mViewport.Height = mHeight;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		mSRRect.left = 0;
		mSRRect.right = mViewport.Width;
		mSRRect.top = 0;
		mSRRect.bottom = mViewport.Height;

		DirectX::XMMATRIX viewMatrix;

		viewMatrix = DirectX::XMMatrixLookAtLH
		(
			{-3.0f, 2.0f, -3.0f, 0.0f},
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f }
		);

		DirectX::XMMATRIX projectionMatrix;

		projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(1.1943951f, 16.0f / 9.0f, 1.0f, 50.0f);

		mViewProjectionMatrix = viewMatrix * projectionMatrix;

                mCBPassData.Initialize(mDevice.Get(), Utils::CalculateConstantbufferAlignment(sizeof(PassData)), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

                mCubeWorldMatrix = DirectX::XMMatrixIdentity();
        }

        void RenderAPI::UpdateDraw()
        {
                PassData passData = {};
                passData.viewproject = mViewProjectionMatrix;
                passData.world = mCubeWorldMatrix;
                memcpy(mCBPassData.GetCPUMemory(), &passData, sizeof(PassData));

		//Set it to a render target state
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = mSwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		mCommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		//Try to clear the color of it
		const float clearColor[] = { 0.0f ,0.0f, 0.0f, 1.0f };
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mSwapChain.GetCurrentRTVHandle();
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthDescHeap->GetCPUDescriptorHandleForHeapStart();

		mCommandList.GFXCmd()->ClearRenderTargetView(rtvHandle, clearColor, 0, 0);
		mCommandList.GFXCmd()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, 0);
		mCommandList.GFXCmd()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		mCommandList.GFXCmd()->RSSetViewports(1, &mViewport);
		mCommandList.GFXCmd()->RSSetScissorRects(1, &mSRRect);


		mCommandList.GFXCmd()->SetGraphicsRootSignature(mBasePipeline.GetRS());
		mCommandList.GFXCmd()->SetPipelineState(mBasePipeline.Get());
		mCommandList.GFXCmd()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList.GFXCmd()->IASetVertexBuffers(0, 1, &mDynamicVBView);

                mCommandList.GFXCmd()->SetGraphicsRootConstantBufferView(0, mCBPassData.Get()->GetGPUVirtualAddress());

                ID3D12DescriptorHeap* heaps[] = { mTextureDescHeap.Get() };
                mCommandList.GFXCmd()->SetDescriptorHeaps(1, heaps);
                mCommandList.GFXCmd()->SetGraphicsRootDescriptorTable(1, mTextureDescHeap->GetGPUDescriptorHandleForHeapStart());

		/*

		Do drawing stuff here


		*/

		mCommandList.GFXCmd()->DrawInstanced(G_BOX_VERTICES, 1, 0, 0);


		//setting it back to the present state
		barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = mSwapChain.GetCurrentRenderTarget();
		barrier.Transition.Subresource = 0;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		mCommandList.GFXCmd()->ResourceBarrier(1, &barrier);

		//close the command list
		mCommandList.GFXCmd()->Close();

		//execute the commands in the command list
		mCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)mCommandList.GetAddressOf());
		mCommandQueue.M_ExecuteCommandList(mCommandList.Get());

		mSwapChain.Present();

		//wait for the allocator to be done / not be in use
		while (mCommandQueue.GetFence()->GetCompletedValue() < mCommandQueue.M_GetCurrentFenceValue())
		{
			_mm_pause();
		}

		//try to reset it
		mCommandList.ResetCommandList();

	}

	void RenderAPI::Release()
	{

		mDynamicVertexBuffer.Release();

		mCommandQueue.FlushQueue();

		mSwapChain.Release();

		mCommandList.Release();

		mCommandQueue.Release();

		if (mDevice.Get())
		{
			mDevice.Reset();
		}
	}
}
