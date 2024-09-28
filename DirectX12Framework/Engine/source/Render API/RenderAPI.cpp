#include "pch.h"
#include "RenderAPI.h"

#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/DXGI/DXGIAdapter.h"

#include "DirectX12/Debug/D12Debug.h"


namespace Engine {

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
		const float clearColor[] = { 1.0f ,0.0f, 1.0f, 1.0f };
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = mSwapChain.GetCurrentRTVHandle();
		mCommandList.GFXCmd()->ClearRenderTargetView(rtvHandle, clearColor, 0, 0);

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
