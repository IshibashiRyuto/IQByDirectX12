/*
*	@file	SpriteData.h
*	@brief	�X�v���C�g�f�[�^���i�[����N���X
*	@author	Ishibashi Ryuto
*	@date	2018/11/20	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include "../Math/Math.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Dx12/DescriptorHeap.h"


/* �N���X�g�p�錾 */
struct InstanceData;
class PipelineStateObject;
class GraphicsCommandList;


/**
*	@class	SpriteData
*	@brief	�X�v���C�g�f�[�^���Ǘ�����N���X
*/
class SpriteData
{
public:
	/**
	*	@brief	�R���X�g���N�^
	*	@param[in]	vertexBuffer	: ���_�o�b�t�@���
	*	@param[in]	descHeap		: �f�X�N���v�^�q�[�v
	*	@param[in]	pso				: �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	*/
	SpriteData(std::shared_ptr<VertexBuffer> vertexBuffer,
		std::shared_ptr<DescriptorHeap> descHeap,
		std::shared_ptr<PipelineStateObject> pso);

	/**
	*	@brief	�f�X�g���N�^
	*/
	~SpriteData();

	/**
	*	@brief	�X�v���C�g�f�[�^���쐬����
	*/
	static std::shared_ptr<SpriteData> Create(std::shared_ptr<VertexBuffer> vertexBuffer,
		std::shared_ptr<DescriptorHeap> descHeap,
		std::shared_ptr<PipelineStateObject> pso);

	/**
	*	@biref	�X�V����
	*/
	void Update();

	/**
	*	@brief	�`�揈��
	*	@param[in]	commandList		: �`��Ώۂ̃R�}���h���X�g
	*	@param[in]	InstanceData	: �C���X�^���V���O�p�f�[�^
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	std::shared_ptr<VertexBuffer>	mVertexBuffer;	//! ���_�o�b�t�@
	std::shared_ptr<DescriptorHeap>	mDescHeap;		//! �f�X�N���v�^�q�[�v
	std::shared_ptr<PipelineStateObject> mPSO;		//! �p�C�v���C���X�e�[�g�I�u�W�F�N�g
};

