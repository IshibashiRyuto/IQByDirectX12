#include <iostream>
#include "CommandQueue.h"



CommandQueue::CommandQueue()
	: mCommandQueue(nullptr)
{
}


CommandQueue::~CommandQueue()
{
}

std::shared_ptr<CommandQueue> CommandQueue::Create(ComPtr<ID3D12Device> device)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.NodeMask = 0;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	return Create(device,desc);
}

std::shared_ptr<CommandQueue> CommandQueue::Create(ComPtr<ID3D12Device> device, const D3D12_COMMAND_QUEUE_DESC & commandQueueDesc)
{
	auto commandQueue = std::shared_ptr<CommandQueue>(new CommandQueue());
	auto result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&(commandQueue->mCommandQueue)));
	if (FAILED(result))
	{
#ifdef _DEBUG
		std::cout << "Failed Create CommandQueue." << std::endl;
#endif
		return nullptr;
	}
	
	result = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&(commandQueue->mFence)));
	if (FAILED(result))
	{

#ifdef _DEBUG
		std::cout << "Failed Create CommandQueue Fance." << std::endl;
#endif
		return nullptr;
	}
	return commandQueue;
}

ComPtr<ID3D12CommandQueue> CommandQueue::GetCommandQueue() const
{
	return mCommandQueue;
}

void CommandQueue::ExecuteCommandList(UINT numCommandLists, ID3D12CommandList *const * ppCommandLists)
{
	mCommandQueue->ExecuteCommandLists(numCommandLists, ppCommandLists);
}

void CommandQueue::Signal()
{
	mCommandQueue->Signal(mFence.Get(), mFenceValue);
	//while (mFence->GetCompletedValue() != mFenceValue);
	if (mFence->GetCompletedValue() < mFenceValue)
	{
		mFenceEvent = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
		WaitForSingleObject(mFenceEvent, INFINITE);
		CloseHandle(mFenceEvent);
	}
	
	++mFenceValue;
}
