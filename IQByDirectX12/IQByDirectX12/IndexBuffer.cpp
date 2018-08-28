#include <d3dx12.h>
#include <iostream>
#include "IndexBuffer.h"



IndexBuffer::IndexBuffer()
{
}


IndexBuffer::~IndexBuffer()
{
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(ComPtr<ID3D12Device> device, void * pIndexResource, size_t indexCount, size_t indexSize)
{
	auto indexBuffer = std::shared_ptr<IndexBuffer>(new IndexBuffer());
	// �C���f�b�N�X�o�b�t�@���\�[�X�̍쐬
	{
		// TODO: RESOURCE_STATE�̏C��
		// ���\�[�X�X�e�[�g��GENERIC_READ�͔񐄏��B
		// �ڂ������������Ă��Ȃ����߁A����͉��Ƃ��Ďg���Ă���B
		// ����C���\��

		auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(indexSize * indexCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuffer->mIndexBuffer));

		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Index Buffer Resource." << std::endl;
#endif
			return nullptr;
		}
	}

	// �C���f�b�N�X�������\�[�X�o�b�t�@�ɏ�������
	{
		char* pIndexBuffer = nullptr;
		auto result = indexBuffer->mIndexBuffer->Map(0, nullptr, (void**)&pIndexBuffer);
		memcpy(pIndexBuffer, pIndexResource, indexSize * indexCount);
		indexBuffer->mIndexBuffer->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	{
		DXGI_FORMAT indexFormat;
		if (indexSize == 8)
		{
			indexFormat = DXGI_FORMAT_R8_UINT;
		}
		else if (indexSize == 16)
		{
			indexFormat = DXGI_FORMAT_R16_UINT;
		}
		else if (indexSize == 32)
		{
			indexFormat = DXGI_FORMAT_R32_UINT;
		}
		else
		{
#ifdef _DEBUG
			std::cout << "Index Format Unknown." << std::endl;
#endif
			return nullptr;
		}
		indexBuffer->mIndexBufferView.BufferLocation = indexBuffer->mIndexBuffer->GetGPUVirtualAddress();
		indexBuffer->mIndexBufferView.SizeInBytes = indexSize * indexCount;
		indexBuffer->mIndexBufferView.Format = indexFormat;		
	}

	return indexBuffer;
}
