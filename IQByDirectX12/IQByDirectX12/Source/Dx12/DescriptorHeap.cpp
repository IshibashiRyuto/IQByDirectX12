#include <iostream>
#include "DescriptorHeap.h"
#include "Device.h"
#include "../Dx12/Buffer/Texture.h"
#include "../Debug/DebugLayer.h"
#include <d3dx12.h>


DescriptorHeap::DescriptorHeap(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc, HRESULT& result)
	: HEAP_STRIDE((*device)->GetDescriptorHandleIncrementSize(heapDesc.Type))
	, mDevice(device)
	, mNumDescriptors(heapDesc.NumDescriptors)
{
	result = (*device)->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
}


DescriptorHeap::~DescriptorHeap()
{
}

std::shared_ptr<DescriptorHeap> DescriptorHeap::Create(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC & desc)
{
	HRESULT result;
	auto descHeap = std::shared_ptr<DescriptorHeap>(new DescriptorHeap(device, desc, result) );
	
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create Descriptor Heap.");
		return nullptr;
	}

	return descHeap;
}

std::shared_ptr<DescriptorHeap> DescriptorHeap::Create(std::shared_ptr<Device> device, UINT numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	return Create(device, heapDesc);
}

std::shared_ptr<DescriptorHeap> DescriptorHeap::Create(std::shared_ptr<Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, unsigned int descriptorsNum, D3D12_DESCRIPTOR_HEAP_FLAGS flag)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.NumDescriptors = descriptorsNum;
	heapDesc.Flags = flag;
	heapDesc.Type = heapType;
	return Create(device, heapDesc);
}

void DescriptorHeap::SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC & constantBufferView, UINT index)
{
	auto handle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += HEAP_STRIDE * index;
	(*mDevice)->CreateConstantBufferView(&constantBufferView, handle);
}

void DescriptorHeap::SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC & shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index)
{
	auto handle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += HEAP_STRIDE * index;
	(*mDevice)->CreateShaderResourceView(shaderResource.Get(), &shaderResourceView, handle);
}

void DescriptorHeap::SetTexture(std::shared_ptr<Texture> texture, UINT index)
{
	if (!texture)
	{
		return;
	}
	auto srv = texture->GetShaderResourceViewDesc();
	SetShaderResourceView(srv, texture->GetResource(), index);
}


void DescriptorHeap::SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC & unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index)
{
}

void DescriptorHeap::SetDepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC & depthStencilViewDesc, ComPtr<ID3D12Resource> resource, UINT index)
{
	auto handle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += HEAP_STRIDE * index;
	(*mDevice)->CreateDepthStencilView(resource.Get(), &depthStencilViewDesc, handle);
}

void DescriptorHeap::BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	mGraphicsCommandList = commandList;
	commandList->SetDescriptorHeaps(1, (mDescriptorHeap.GetAddressOf()));
}

void DescriptorHeap::BindRootDescriptorTable(int rootParamIndex, int descriptorHeapIndex)
{
	if (descriptorHeapIndex >= (int)mNumDescriptors)
	{
		return;
	}
	auto gpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += HEAP_STRIDE * descriptorHeapIndex;
	if (!mGraphicsCommandList)
	{
		return;
	}

	mGraphicsCommandList->SetGraphicsRootDescriptorTable(rootParamIndex, gpuHandle);
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHandle(UINT index) const
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),index, HEAP_STRIDE );
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHandle(UINT index) const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), index, HEAP_STRIDE);
}
