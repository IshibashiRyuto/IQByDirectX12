#include <d3dx12.h>
#include <iostream>
#include "VertexBuffer.h"



VertexBuffer::VertexBuffer()
{
}


VertexBuffer::~VertexBuffer()
{
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(ComPtr<ID3D12Device> device, void * pVertexResource, size_t vertexCount, size_t vertexSize)
{
	auto vertexBuffer = std::shared_ptr<VertexBuffer>(new VertexBuffer());
	// 頂点バッファリソースの作成
	{
		// TODO: RESOURCE_STATEの修正
		// リソースステートのGENERIC_READは非推奨。
		// 詳しい調査をしていないため、今回は仮として使っている。
		// 後日修正予定
		auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(vertexSize * vertexCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBuffer->mVertexBuffer));

		if (FAILED(result))
		{
			std::cout << "Failed Create Vertex Buffer Resource." << std::endl;
			return nullptr;
		}
	}

	// 頂点情報をリソースバッファに書き込み
	{
		char* pVertexBuffer = nullptr;
		auto result = vertexBuffer->mVertexBuffer->Map(0, nullptr, (void**)&pVertexBuffer);
		memcpy(pVertexBuffer, pVertexResource, vertexSize * vertexCount);
		vertexBuffer->mVertexBuffer->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	{
		vertexBuffer->mVertexBufferView.BufferLocation = vertexBuffer->mVertexBuffer->GetGPUVirtualAddress();
		vertexBuffer->mVertexBufferView.StrideInBytes = (UINT)vertexSize;
		vertexBuffer->mVertexBufferView.SizeInBytes = (UINT)(vertexSize*vertexCount);
	}
	return vertexBuffer;
}

const D3D12_VERTEX_BUFFER_VIEW & VertexBuffer::GetVertexBufferView()
{
	return mVertexBufferView;
}
