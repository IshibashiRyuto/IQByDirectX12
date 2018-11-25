#include <d3dx12.h>
#include "Dx12Resource.h"
#include "../Device.h"
#include "../Debug/DebugLayer.h"
#include "../Dx12/GraphicsCommandList.h"

Dx12Resource::Dx12Resource(std::shared_ptr<Device> device,
	const D3D12_HEAP_PROPERTIES & heapProp,
	D3D12_HEAP_FLAGS heapFlag,
	const D3D12_RESOURCE_DESC & desc,
	D3D12_RESOURCE_STATES states,
	const D3D12_CLEAR_VALUE* pClearValue,
	const std::wstring& resourceName,
	HRESULT& result)
	: mCurrentState(states)
{
	result = (*device)->CreateCommittedResource(&heapProp,
		heapFlag,
		&desc,
		states,
		pClearValue,
		IID_PPV_ARGS(&mResource));

	if (FAILED(result))
	{
		return;
	}
	mResource->SetName(static_cast<LPCWSTR>(resourceName.data()));
}

Dx12Resource::Dx12Resource(std::shared_ptr<Dx12Resource> resource)
	: mResource(resource->GetResource())
	, mCurrentState(resource->mCurrentState)
{
}

Dx12Resource::Dx12Resource(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state)
	: mResource(resource)
	, mCurrentState(state)
{
}

Dx12Resource::~Dx12Resource()
{
}


std::shared_ptr<Dx12Resource> Dx12Resource::Create(std::shared_ptr<Device> device, const D3D12_HEAP_PROPERTIES & heapProp, D3D12_HEAP_FLAGS heapFlag, const D3D12_RESOURCE_DESC & desc, D3D12_RESOURCE_STATES states, 
		const D3D12_CLEAR_VALUE* pClearValue, const std::wstring & resourceName)

{
	HRESULT result;
	auto pRes = std::shared_ptr<Dx12Resource>(new Dx12Resource(device,
		heapProp,
		heapFlag,
		desc,
		states,
		pClearValue,
		resourceName,
		result));

	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage(L"Failed Create Resource. : " + resourceName);
		return nullptr;
	}
	return pRes;
}

const ComPtr<ID3D12Resource> Dx12Resource::GetResource() const
{
	return mResource;
}

void Dx12Resource::TransitionState(std::shared_ptr<GraphicsCommandList> commandList, D3D12_RESOURCE_STATES transitionState)
{
	if (transitionState != mCurrentState)
	{
		(*commandList)->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(), mCurrentState, transitionState));
		mCurrentState = transitionState;
	}
}
