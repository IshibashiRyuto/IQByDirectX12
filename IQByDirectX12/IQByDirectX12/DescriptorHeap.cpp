#include <iostream>
#include "DescriptorHeap.h"
#include "Texture.h"


DescriptorHeap::DescriptorHeap(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc)
	: HEAP_STRIDE(device->GetDescriptorHandleIncrementSize(heapDesc.Type))
	, mDevice(device)
	, mNumDescriptors(heapDesc.NumDescriptors)
{
}


DescriptorHeap::~DescriptorHeap()
{
}

std::shared_ptr<DescriptorHeap> DescriptorHeap::Create(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC & desc)
{
	auto descHeap = std::shared_ptr<DescriptorHeap>(new DescriptorHeap(device, desc) );
	
	auto result = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap->mDescriptorHeap));
	
	if (FAILED(result))
	{
#ifdef _DEBUG
		std::cout << "Failed Create Descriptor Heap." << std::endl;
		return nullptr;
#endif
	}

	return descHeap;
}

std::shared_ptr<DescriptorHeap> DescriptorHeap::Create(ComPtr<ID3D12Device> device, UINT numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	return Create(device, heapDesc);
}

void DescriptorHeap::SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC & constantBufferView, UINT index)
{
	auto handle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += HEAP_STRIDE * index;
	mDevice->CreateConstantBufferView(&constantBufferView, handle);
}

void DescriptorHeap::SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC & shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index)
{
	auto handle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += HEAP_STRIDE * index;
	mDevice->CreateShaderResourceView(shaderResource.Get(), &shaderResourceView, handle);
}

void DescriptorHeap::SetTexture(std::shared_ptr<Texture> texture, UINT index)
{
	auto srv = texture->GetShaderResourceView();
	SetShaderResourceView(srv, texture->GetTextureData(), index);
	texture->SetShaderResourceView(srv);
}

void DescriptorHeap::SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC & unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index)
{
}

void DescriptorHeap::BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	auto gpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	commandList->SetDescriptorHeaps(1, (mDescriptorHeap.GetAddressOf()));
	commandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
	gpuHandle.ptr += HEAP_STRIDE;
	commandList->SetGraphicsRootDescriptorTable(1, gpuHandle);
}
