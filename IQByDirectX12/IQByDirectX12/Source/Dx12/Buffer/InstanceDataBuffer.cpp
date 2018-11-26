#include <d3dx12.h>
#include "InstanceDataBuffer.h"
#include "../Debug/DebugLayer.h"


InstanceDataBuffer::InstanceDataBuffer(std::shared_ptr<Device> device, size_t instanceDataSize, size_t maxInstanceCount, const std::wstring & bufferName, HRESULT & result)

	: Dx12Resource
	(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT>(instanceDataSize * maxInstanceCount)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		bufferName,
		result
	)
	, mMaxInstanceCount(static_cast<unsigned int>(maxInstanceCount))
	, mInstanceDataSize(instanceDataSize)
{
	if (FAILED(result))
	{
		return;
	}
	ConstructVertexBufferView(instanceDataSize, maxInstanceCount);
	Map();
}

InstanceDataBuffer::~InstanceDataBuffer()
{
}

std::shared_ptr<InstanceDataBuffer> InstanceDataBuffer::Create(std::shared_ptr<Device> device, size_t instanceDataSize, size_t maxInstanceCount, const std::wstring & bufferName)
{
	HRESULT result;
	auto ret = std::shared_ptr<InstanceDataBuffer>(new InstanceDataBuffer(device, instanceDataSize, maxInstanceCount, bufferName, result));
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage(L"Failed Create Instance Data Buffer. : " + bufferName);
		return nullptr;
	}
	return ret;
}

void InstanceDataBuffer::SetInstanceData(void * pInstanceData, int instanceDataIndex)
{
	if (mMaxInstanceCount <= instanceDataIndex || 0 > instanceDataIndex)
	{
		return;
	}
	memcpy((char*)m_pInstanceBuffer + mInstanceDataSize * instanceDataIndex, pInstanceData, mInstanceDataSize);
}

const D3D12_VERTEX_BUFFER_VIEW & InstanceDataBuffer::GetVertexBufferView() const
{
	return mVertexBufferView;
}

void InstanceDataBuffer::ConstructVertexBufferView(size_t dataSize, size_t dataCount)
{
	mVertexBufferView.BufferLocation = mResource->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = static_cast<UINT>(dataSize);
	mVertexBufferView.SizeInBytes = static_cast<UINT>(dataSize * dataCount);
}

void InstanceDataBuffer::Map()
{
	mResource->Map(0, nullptr, &m_pInstanceBuffer);
}
