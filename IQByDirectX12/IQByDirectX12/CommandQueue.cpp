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
	if (result == S_OK)
	{
		return commandQueue;
	}
#ifdef _DEBUG
	std::cout << "Failed Create CommandQueue." << std::endl;
#endif
	return nullptr;
}

ComPtr<ID3D12CommandQueue> CommandQueue::GetCommandQueue() const
{
	return mCommandQueue;
}
