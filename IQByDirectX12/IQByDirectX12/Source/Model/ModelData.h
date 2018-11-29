/*
	@file	ModelData.h
	@brief	���f���f�[�^���i�[����N���X
	@author	Ishibashi Ryuto
	@date	2018/08/28	���ō쐬
			2018/11/25	�o�b�t�@�N���X�̃��t�@�N�^�����O�ɑΉ�
			2018/11/27	DrawNoMat�ǉ�
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include<memory>
#include <list>
#include "../Math/Math.h"
#include "../Dx12/Buffer/VertexBuffer.h"
#include "../Dx12/Buffer/IndexBuffer.h"
#include "../Dx12/DescriptorHeap.h"

// �N���X�g�p�錾
struct InstanceData;
class Pose;
class PipelineStateObject;

class ModelData
{
public:
	/// �R���X�g���N�^
	ModelData(std::shared_ptr<VertexBuffer> vertexBuffer, 
		std::shared_ptr<IndexBuffer> indexBuffer,
		std::shared_ptr<DescriptorHeap> descriptorHeap,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);

	/// �f�X�g���N�^
	virtual ~ModelData();

	/// ���_�o�b�t�@���擾����
	std::shared_ptr<VertexBuffer> GetVertexBuffer() const;

	/// �C���f�b�N�X�o�b�t�@���擾����
	std::shared_ptr<IndexBuffer> GetIndexBuffer() const;

	/// �f�X�N���v�^�q�[�v���擾����
	std::shared_ptr<DescriptorHeap> GetDescriptorHeap() const;

	/// Debug
	/// �p�������擾����
	std::shared_ptr<Pose> _DebugGetPose() const;

	/// �X�V����
	virtual void Update();

	/// �`�揈��
	virtual void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

	/**
	*	@brief	�}�e���A�����g�p�����ɕ`�悷��(���f���̃K�������`��)
	*/
	virtual void DrawNoMat(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;

protected:
	std::shared_ptr<VertexBuffer>			mVertexBuffer;
	std::shared_ptr<IndexBuffer>			mIndexBuffer;
	std::shared_ptr<DescriptorHeap>			mDescHeap;
	std::shared_ptr<Pose>					mPose;
	std::shared_ptr<PipelineStateObject>	mPipelineStateObject;
private:
};

