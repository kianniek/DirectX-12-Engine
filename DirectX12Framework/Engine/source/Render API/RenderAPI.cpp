#include "pch.h"
#include "RenderAPI.h"

#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/DXGI/DXGIAdapter.h"



namespace Engine {

	RenderAPI::~RenderAPI()
	{

	}

	void RenderAPI::Initialize(HWND hwnd)
	{

		DXGIFactory factory;

		DXGIAdapter adapter = factory.GetAdapter();

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		PRINT_W_N("Selected device " << desc.Description)

		bool hasCreatedDevice = mDevice.Init(adapter.Get());

		mDevice->SetName(L"Main virtual device");

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
}