#include <d3dx12.h>
#include <iostream>
#include "IndexBuffer.h"
#include "Debug\DebugLayer.h"



IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(ComPtr<ID3D12Device> device, void * pIndexResource, size_t indexCount, size_t indexSize)
{
	auto indexBuffer = std::shared_ptr<IndexBuffer>(new IndexBuffer());
	indexBuffer->mIndexCount = (unsigned int)indexCount;
	// インデックスバッファリソースの作成
	{
		// TODO: RESOURCE_STATEの修正
		// リソースステートのGENERIC_READは非推奨。
		// 詳しい調査をしていないため、今回は仮として使っている。
		// 後日修正予定

		auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(indexSize * indexCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuffer->mIndexBuffer));

		if (FAILED(result))
		{
			DebugLayer::GetInstance().PrintDebugMessage("Failed Create Index Buffer Resource.");
			return nullptr;
		}
	}

	// インデックス情報をリソースバッファに書き込み
	{
		char* pIndexBuffer = nullptr;
		auto result = indexBuffer->mIndexBuffer->Map(0, nullptr, (void**)&pIndexBuffer);
		memcpy(pIndexBuffer, pIndexResource, indexSize * indexCount);
		indexBuffer->mIndexBuffer->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	{
		DXGI_FORMAT indexFormat;
		if (indexSize == 1)
		{
			indexFormat = DXGI_FORMAT_R8_UINT;
		}
		else if (indexSize == 2)
		{
			indexFormat = DXGI_FORMAT_R16_UINT;
		}
		else if (indexSize == 4)
		{
			indexFormat = DXGI_FORMAT_R32_UINT;
		}
		else
		{
			DebugLayer::GetInstance().PrintDebugMessage("Index Format Unknown.");
			return nullptr;
		}
		indexBuffer->mIndexBufferView.BufferLocation = indexBuffer->mIndexBuffer->GetGPUVirtualAddress();
		indexBuffer->mIndexBufferView.SizeInBytes = (UINT)(indexSize * indexCount);
		indexBuffer->mIndexBufferView.Format = indexFormat;		
	}

	return indexBuffer;
}

const D3D12_INDEX_BUFFER_VIEW & IndexBuffer::GetIndexBufferView()
{
	return mIndexBufferView;
}

unsigned int IndexBuffer::GetIndexCount()
{
	return mIndexCount;
}
