#include <iostream>
#include "CommandAllocator.h"
#include "Device.h"
#include "../Debug\DebugLayer.h"



CommandAllocator::CommandAllocator()
	: mCommandAllocator(nullptr)
{
}


CommandAllocator::~CommandAllocator()
{
}

std::shared_ptr<CommandAllocator> CommandAllocator::Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring& name)
{
	auto commandAllocator = std::shared_ptr<CommandAllocator>(new CommandAllocator());
	auto result = device->GetDevice()->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&(commandAllocator->mCommandAllocator)));
	if (result == S_OK)
	{
		commandAllocator->mCommandAllocator->SetName(name.c_str());
		return commandAllocator;
	}
	DebugLayer::GetInstance().PrintDebugMessage("Failed Create CommandAllocator.");
	return nullptr;
}

ComPtr<ID3D12CommandAllocator> CommandAllocator::GetAllocator() const
{
	return mCommandAllocator;
}

ID3D12CommandAllocator * const CommandAllocator::operator->() const
{
	return mCommandAllocator.Get();
}
