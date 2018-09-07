/*
	@file	ModelData.h
	@brief	���f���f�[�^���i�[����N���X
	@param	history
	2018/08/28	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include<memory>

//����w�b�_�C���N���[�h
#include "../Math/Math.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"

// �N���X�g�p�錾
struct InstanceData;

class ModelData
{
public:
	/// �R���X�g���N�^
	ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<DescriptorHeap> descriptorHeap);

	/// �f�X�g���N�^
	virtual ~ModelData();

	/// ���_�o�b�t�@���擾����
	std::shared_ptr<VertexBuffer> GetVertexBuffer() const;

	/// �C���f�b�N�X�o�b�t�@���擾����
	std::shared_ptr<IndexBuffer> GetIndexBuffer() const;

	/// �f�X�N���v�^�q�[�v���擾����
	std::shared_ptr<DescriptorHeap> GetDescriptorHeap() const;

	/// �`�揈��
	virtual void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

protected:
	std::shared_ptr<VertexBuffer> mVertexBuffer;
	std::shared_ptr<IndexBuffer> mIndexBuffer;
	std::shared_ptr<DescriptorHeap> mDescHeap;
private:
};

