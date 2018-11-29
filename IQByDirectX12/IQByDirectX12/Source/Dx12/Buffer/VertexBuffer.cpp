#include <d3dx12.h>
#include "VertexBuffer.h"
#include "../Debug/DebugLayer.h"

VertexBuffer::VertexBuffer(std::shared_ptr<Device> device,
	size_t dataSize,
	size_t dataCount,
	const std::wstring & bufferName,
	HRESULT & result)
	: Dx12Resource
	(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT>(dataSize * dataCount)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		bufferName,
		result
	)
{
	if (FAILED(result))
	{
		return;
	}
	ConstructVertexBufferView(dataSize, dataCount);
}

VertexBuffer::~VertexBuffer()
{
}


std::shared_ptr<VertexBuffer> VertexBuffer::Create(std::shared_ptr<Device> device, const void * pVertices, size_t vertexSize, size_t vertexCount, const std::wstring & bufferName)
{
	HRESULT result;
	auto ret = std::shared_ptr<VertexBuffer>(new VertexBuffer(device, vertexSize, vertexCount, bufferName, result));
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage(L"Failed Create Vertex Buffer. : " + bufferName);
		return nullptr;
	}
	ret->WriteVertexBuffer(pVertices, vertexSize, vertexCount);
	return ret;
}

const D3D12_VERTEX_BUFFER_VIEW & VertexBuffer::GetVertexBufferView() const
{
	return mVertexBufferView;
}


void VertexBuffer::WriteVertexBuffer(const void * data, size_t dataSize, size_t dataCount, unsigned int dataIndexOffset)
{
	void* pBuf;
	mResource->Map(0, nullptr, &pBuf);
	memcpy((char*)pBuf + dataSize * dataIndexOffset, data, dataSize * dataCount);
	mResource->Unmap(0, nullptr);
}

void VertexBuffer::ConstructVertexBufferView(size_t dataSize, size_t dataCount)
{
	mVertexBufferView.BufferLocation = mResource->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = static_cast<UINT>(dataSize);
	mVertexBufferView.SizeInBytes = static_cast<UINT>(dataSize * dataCount);
}

