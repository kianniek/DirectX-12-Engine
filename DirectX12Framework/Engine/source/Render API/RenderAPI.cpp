#include "pch.h"
#include "RenderAPI.h"

#include <vector>

#include "RenderDataTypes.h"
#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/DXGI/DXGIAdapter.h"

#include "DirectX12/Debug/D12Debug.h"

#include "DirectX12/Pipeline/HLSLShader.h"
#include "DirectX12/Pipeline/D12RootSignature.h"

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

		std::vector<Vertex> vertices;

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

		

		void* destination = nullptr;

		mDynamicVertexBuffer->Map(0, 0, &destination);

		//memcpy(destination, &vertexData, sizeof(Vertex));
		memcpy(destination, vertices.data(), sizeof(Vertex) * vertices.size());

		mDynamicVertexBuffer->Unmap(0, 0);

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

		if (!hasCreatedDevice) {
			// Log the error
			std::cerr << "Failed to create D12 device." << std::endl;

			// Handle cleanup
			// release any resources that might have been partially allocated.
			//TODO: Cleanup();

			// Depending on your application, you may:
			// 1. Return from the current function
			// 2. Exit the application with an error code
			// 3. Retry device creation under different conditions (e.g., lower feature levels)

			// we'll exit the application to prevent any further D12 calls.
			std::exit(EXIT_FAILURE);
		}

		// Proceed with other D12 calls only if the device was successfully created
		// Your other D12 initialization or rendering code here

		/*
		ACCESS THE HARDWARE -> CREATE A VIRTUAL DEVICE (context for the D12  API)

		COM SYSTEM

		*/
	}

	void RenderAPI::UpdateDraw()
	{
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
		mCommandList.GFXCmd()->ClearRenderTargetView(rtvHandle, clearColor, 0, 0);

		mCommandList.GFXCmd()->OMSetRenderTargets(1, &rtvHandle, false, 0);

		mCommandList.GFXCmd()->RSSetViewports(1, &mViewport);
		mCommandList.GFXCmd()->RSSetScissorRects(1, &mSRRect);



		mCommandList.GFXCmd()->SetGraphicsRootSignature(mBasePipeline.GetRS());
		mCommandList.GFXCmd()->SetPipelineState(mBasePipeline.Get());
		mCommandList.GFXCmd()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList.GFXCmd()->IASetVertexBuffers(0, 1, &mDynamicVBView);

		/*

		Do drawing stuff here


		*/

		mCommandList.GFXCmd()->DrawInstanced(3, 1, 0, 0);


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
