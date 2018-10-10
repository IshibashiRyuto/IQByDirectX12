#include "CommandAllocator.h"
#include "Device.h"
#include "GraphicsCommandList.h"



GraphicsCommandList::GraphicsCommandList(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType,const std::wstring & name)
{
	mCommandAllocator = CommandAllocator::Create(device, commandListType, name);
	auto result = device->GetDevice()->CreateCommandList(0, commandListType, mCommandAllocator->Get().Get(), nullptr, IID_PPV_ARGS(&mCommandList));
	mCommandList->SetName(static_cast<LPCWSTR>(name.c_str()));
}

GraphicsCommandList::~GraphicsCommandList()
{
}

std::shared_ptr<GraphicsCommandList> GraphicsCommandList::Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring & name)
{
	return std::shared_ptr<GraphicsCommandList>( new GraphicsCommandList(device, commandListType, name) );
}

ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList::GetCommandList()
{
	return mCommandList;
}

void GraphicsCommandList::Close()
{
	mCommandList->Close();
}

void GraphicsCommandList::Reset()
{
	mCommandAllocator->Get()->Reset();
	mCommandList->Reset(mCommandAllocator->Get().Get(),nullptr);
}

void GraphicsCommandList::Reset(ComPtr<ID3D12PipelineState> pipelineState)
{
	mCommandAllocator->Get()->Reset();
	mCommandList->Reset(mCommandAllocator->Get().Get(), pipelineState.Get());
}

ID3D12GraphicsCommandList * const GraphicsCommandList::operator->()
{
	return mCommandList.Get();
}
