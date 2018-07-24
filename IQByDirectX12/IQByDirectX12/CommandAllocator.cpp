#include <iostream>
#include "CommandAllocator.h"



CommandAllocator::CommandAllocator()
	: mCommandAllocator(nullptr)
{
}


CommandAllocator::~CommandAllocator()
{
}

std::shared_ptr<CommandAllocator> CommandAllocator::Create(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE commandListType)
{
	auto commandAllocator = std::shared_ptr<CommandAllocator>(new CommandAllocator());
	auto result = device->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&(commandAllocator->mCommandAllocator)));
	if (result == S_OK)
	{
		return commandAllocator;
	}
#ifdef _DEBUG
	std::cout << "Failed Create CommandAllocator." << std::endl;
#endif
	return nullptr;
}

ComPtr<ID3D12CommandAllocator> CommandAllocator::Get() const
{
	return mCommandAllocator;
}
