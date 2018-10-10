#include <iostream>
#include <d3dx12.h>
#include "InstanceBuffer.h"
#include "Debug\DebugLayer.h"


InstanceBuffer::InstanceBuffer()
{
}


InstanceBuffer::~InstanceBuffer()
{
	mInstanceBuffer->Unmap(0, nullptr);
}

std::shared_ptr<InstanceBuffer> InstanceBuffer::Create(ComPtr<ID3D12Device> device, size_t instanceDataSize, int maxInstanceCount)
{
	auto instanceBuffer = std::shared_ptr<InstanceBuffer>(new InstanceBuffer());
	// �o�b�t�@���\�[�X�쐬
	{
		// TODO: RESOURCE_STATE�̏C��
		// ���\�[�X�X�e�[�g��GENERIC_READ�͔񐄏��B
		// �ڂ������������Ă��Ȃ����߁A����͉��Ƃ��Ďg���Ă���B
		// ����C���\��
		auto result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(instanceDataSize * maxInstanceCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&instanceBuffer->mInstanceBuffer)
		);
		if (FAILED(result))
		{
			DebugLayer::GetInstance().PrintDebugMessage("Failed Create Vertex Buffer Resource.");
			return nullptr;
		}
	}

	// ���_�o�b�t�@�r���[�̍쐬
	{
		instanceBuffer->mVertexBufferView.BufferLocation = instanceBuffer->mInstanceBuffer->GetGPUVirtualAddress();
		instanceBuffer->mVertexBufferView.StrideInBytes = (UINT)instanceDataSize;
		instanceBuffer->mVertexBufferView.SizeInBytes = (UINT)(instanceDataSize * maxInstanceCount);
	}
	instanceBuffer->mMaxInstanceCount = maxInstanceCount;
	instanceBuffer->mInstanceDataSize = (int)instanceDataSize;

	// ���_���}�b�v
	{
		instanceBuffer->mInstanceBuffer->Map(0, nullptr, &instanceBuffer->pInstanceBuffer);
	}

	return instanceBuffer;
}

const D3D12_VERTEX_BUFFER_VIEW & InstanceBuffer::GetVertexBufferView() const
{
	return mVertexBufferView;
}

void InstanceBuffer::SetInstanceData(void * pInstanceData, int instanceDataIndex)
{
	if (instanceDataIndex >= mMaxInstanceCount)
	{
		return;
	}

	memcpy((char*)pInstanceBuffer + mInstanceDataSize * instanceDataIndex, pInstanceData, mInstanceDataSize);
}
