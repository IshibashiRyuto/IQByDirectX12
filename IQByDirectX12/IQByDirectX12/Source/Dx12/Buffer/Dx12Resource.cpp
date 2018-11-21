#include "Dx12Resource.h"
#include "../Device.h"

Dx12Resource::Dx12Resource(std::shared_ptr<Device> device,
	const D3D12_HEAP_PROPERTIES & heapProp,
	D3D12_HEAP_FLAGS heapFlag,
	const D3D12_RESOURCE_DESC & desc,
	D3D12_RESOURCE_STATES states,
	const D3D12_CLEAR_VALUE & clearValue,
	HRESULT& result)
{
	result = (*device)->CreateCommittedResource(&heapProp,
		heapFlag,
		&desc,
		states,
		&clearValue,
		IID_PPV_ARGS(&mResource));
}

Dx12Resource::~Dx12Resource()
{
}


std::shared_ptr<Dx12Resource> Dx12Resource::Create(std::shared_ptr<Device> device, const D3D12_HEAP_PROPERTIES & heapProp, D3D12_HEAP_FLAGS heapFlag, const D3D12_RESOURCE_DESC & desc, D3D12_RESOURCE_STATES states, const D3D12_CLEAR_VALUE & clearValue, const std::wstring & resourceName)
{
	HRESULT result;
	auto pRes = new Dx12Resource(device,
		heapProp,
		heapFlag,
		desc,
		states,
		clearValue,
		result);

	if (FAILED(result))
	{
		return nullptr;
	}
	pRes->mResource->SetName(static_cast<LPCWSTR>(resourceName.data()));
	return std::shared_ptr<Dx12Resource>(pRes);
}

const ComPtr<ID3D12Resource> Dx12Resource::GetResource() const
{
	return mResource;
}
