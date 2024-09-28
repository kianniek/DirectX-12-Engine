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
		frameCount++;
		PRINT_W_N("FrameCount:" << frameCount)
	}

	void RenderAPI::Release()
	{
		mSwapChain.Release();

		mCommandList.Release();

		mCommandQueue.Release();

		if (mDevice.Get())
		{
			mDevice.Reset();
		}
	}
}
