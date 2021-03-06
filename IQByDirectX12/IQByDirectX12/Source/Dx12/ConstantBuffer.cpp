#include<d3dx12.h>
#include<iostream>
#include "ConstantBuffer.h"
#include "Device.h"
#include "../Debug\DebugLayer.h"



ConstantBuffer::ConstantBuffer(UINT elementSize, UINT elementCount)
	: mElementSize( (elementSize + 0xff) & ~0xff )
	, mElementCount(elementCount)
{
}


ConstantBuffer::~ConstantBuffer()
{
}

std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(std::shared_ptr<Device> device, UINT elementSize, UINT elementCount)
{
	auto constantBuffer = std::shared_ptr<ConstantBuffer>(new ConstantBuffer(elementSize, elementCount));

	// 定数バッファリソースの作成
	if(!constantBuffer->CreateConstantBuffer(device->GetDevice()))
	{
		return nullptr;
	}

	// 定数バッファビューの作成
	constantBuffer->CreateConstantBufferView();
	

	return constantBuffer;
}

bool ConstantBuffer::SetData(const void * data, UINT elementSize, UINT elementIndex)
{
	if (elementIndex >= mElementCount || elementSize > mElementSize)
	{
		return false;
	}

	std::memcpy(mBufAddress + mElementSize * elementIndex, data, elementSize);
	return true;
}

const D3D12_CONSTANT_BUFFER_VIEW_DESC & ConstantBuffer::GetConstantBufferView(UINT elementIndex)
{
	mCBVDesc.BufferLocation = elementIndex < mElementCount? mBuffer->GetGPUVirtualAddress() + mElementSize * elementIndex: mBuffer->GetGPUVirtualAddress();
	return mCBVDesc;
}

bool ConstantBuffer::CreateConstantBuffer(const ComPtr<ID3D12Device> device)
{
	auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mElementSize * mElementCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mBuffer));

	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create Constant Buffer Resource.");
		return false;
	}

	D3D12_RANGE range{};

	result = mBuffer->Map(0, &range, (void**)&mBufAddress);
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Map by ConstantBuffer.");
		return false;
	}

	return true;
}

void ConstantBuffer::CreateConstantBufferView()
{
	mCBVDesc.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mCBVDesc.SizeInBytes = mElementSize;
}
