#include <iostream>
#include "CommandQueue.h"
#include "Device.h"
#include "../Debug/DebugLayer.h"
#include "GraphicsCommandList.h"



CommandQueue::CommandQueue()
	: mCommandQueue(nullptr)
	, mFenceEvent( CreateEvent(nullptr, false, false, nullptr))
{
}


CommandQueue::~CommandQueue()
{
	CloseHandle(mFenceEvent);
}

std::shared_ptr<CommandQueue> CommandQueue::Create(std::shared_ptr<Device> device)
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.NodeMask = 0;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	return Create(device,desc);
}

std::shared_ptr<CommandQueue> CommandQueue::Create(std::shared_ptr<Device> device, const D3D12_COMMAND_QUEUE_DESC & commandQueueDesc)
{
	auto commandQueue = std::shared_ptr<CommandQueue>(new CommandQueue());
	auto result = device->GetDevice()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&(commandQueue->mCommandQueue)));
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create CommandQueue.");
		return nullptr;
	}
	
	result = device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&(commandQueue->mFence)));
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create CommandQueue Fance.");
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

void CommandQueue::ExecuteCommandList(const std::shared_ptr<GraphicsCommandList> commandList)
{
	ID3D12CommandList* cmdList = commandList->GetCommandList().Get();
	mCommandQueue->ExecuteCommandLists(1, &cmdList);
}

void CommandQueue::ExecuteCommandList(const std::vector<std::shared_ptr<GraphicsCommandList>>& commandLists)
{
	std::vector<ID3D12CommandList*> commandListsArray;
	commandListsArray.resize(commandLists.size());
	for (unsigned int i = 0; i < commandLists.size(); ++i)
	{
		commandListsArray[i] = commandLists[i]->GetCommandList().Get();
	}
	mCommandQueue->ExecuteCommandLists(static_cast<UINT>(commandListsArray.size()), commandListsArray.data());
}

void CommandQueue::Signal()
{
	mCommandQueue->Signal(mFence.Get(), mFenceValue);
	mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
	WaitForSingleObject(mFenceEvent, INFINITE);
	
	++mFenceValue;
}
