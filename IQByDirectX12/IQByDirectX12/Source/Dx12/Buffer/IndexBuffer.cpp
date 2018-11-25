#include <d3dx12.h>
#include "IndexBuffer.h"
#include "../Debug/DebugLayer.h"

IndexBuffer::IndexBuffer(std::shared_ptr<Device> device,
	size_t indexDataSize,
	size_t indexCount,
	const std::wstring & bufferName,
	HRESULT & result)
	: Dx12Resource
	(
		device,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(indexDataSize*indexCount),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		nullptr,
		bufferName,
		result
	)
{
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage(L"Failed Create Index Buffer. : " + bufferName );
		return;
	}
	ConstructIndexBufferView(indexDataSize, indexCount, result);
}

IndexBuffer::~IndexBuffer()
{
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(std::shared_ptr<Device> device,
	void * pVertexIndexes,
	size_t indexSize,
	size_t indexCount,
	const std::wstring & bufferName)
{
	HRESULT result;
	auto ret = std::shared_ptr<IndexBuffer>(new IndexBuffer(device, indexSize, indexCount, bufferName, result));
	if (FAILED(result))
	{
		return nullptr;
	}

	ret->WriteIndexData(pVertexIndexes, indexSize, indexCount);
	return ret;
}

const D3D12_INDEX_BUFFER_VIEW & IndexBuffer::GetIndexBufferView() const
{
	return mIndexBufferView;
}

void IndexBuffer::WriteIndexData(void * pIndexData, size_t indexDataSize, size_t dataCount, unsigned int offsetIndex)
{
	void* pResource;
	mResource->Map(0, nullptr, &pResource);
	memcpy((char*)pResource + indexDataSize * offsetIndex, pIndexData, indexDataSize * dataCount);
}

void IndexBuffer::ConstructIndexBufferView(size_t indexDataSize, size_t indexCount, HRESULT& result)
{
	DXGI_FORMAT format;
	if (indexDataSize == 1)
	{
		format = DXGI_FORMAT_R8_UINT;
	}
	else if (indexDataSize == 2)
	{
		format = DXGI_FORMAT_R16_UINT;
	}
	else if (indexDataSize == 4)
	{
		format = DXGI_FORMAT_R32_UINT;
	}
	else
	{
		result = E_FAIL;
		return;
	}

	mIndexBufferView.BufferLocation = mResource->GetGPUVirtualAddress();
	mIndexBufferView.SizeInBytes = static_cast<UINT>(indexDataSize * indexCount);
	mIndexBufferView.Format = format;
}
