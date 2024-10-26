#include "pch.h"
#include "D12CommandQueue.h"

namespace Engine
{
	D12CommandQueue::~D12CommandQueue()
	{
		Reset();
	}

	void D12CommandQueue::Initialize(ID3D12Device* pDevice)
	{
		//Create the resources needed:
		//The queue itself
		//The Fence

		//Direct Command Queue
		D3D12_COMMAND_QUEUE_DESC description = {};		//cant use a CONST because the DX12 API moves values* on its own
		description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		description.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		description.NodeMask = 0; //TODO: If support is needed for SLI dual GPU setup, look at this NodeMask

		KOSMO_EVAL_HR(pDevice->CreateCommandQueue(&description, IID_PPV_ARGS(GetAddressOf())), "Error creating command queue");
		KOSMO_EVAL_HR(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf())), "Error creating the fence");
	}

	void D12CommandQueue::M_ExecuteCommandList(ID3D12CommandList* pCommandList)
	{
		Get()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&pCommandList);
		mCurrentFenceValue++;
		Get()->Signal(mFence.Get(), mCurrentFenceValue);
	}

	void D12CommandQueue::Release()
	{
		if (Get())
		{
			Reset();
		}

		if (mFence.Get())
		{
			mFence.Reset();
		}

	}

	void D12CommandQueue::FlushQueue()
	{
		if (Get())
		{
			for (unsigned int i = 0; i < G_MAX_SWAPCHAIN_BUFFERS; i++)
			{
				Get()->Signal(mFence.Get(), mCurrentFenceValue + i);

			}
		}
	}
}
