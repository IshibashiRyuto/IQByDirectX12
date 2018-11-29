/*
*	@file	PrimitiveModelData.h
*	@brief	�v���~�e�B�u�̃��f���f�[�^�N���X���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/19	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "../ModelData.h"
#include "PrimitiveData.h"

/* �N���X�g�p�錾 */
class ConstantBuffer;

/**
*	@class	PrimitiveModelData
*	@brief	�v���~�e�B�u���f���̃x�[�X�N���X
*/
class PrimitiveModelData :
	public ModelData
{
public:
	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	vertices	: ���_���
	*	@param[in]	indexes		: �C���f�b�N�X���
	*	@param[in]	material	: �P��}�e���A�����
	*/
	PrimitiveModelData(std::shared_ptr<Device> device,
		const std::vector<Primitive::Vertex>& vertices,
		const std::vector<Primitive::Index>& indexes,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);
	virtual ~PrimitiveModelData();

	void Update();

	void Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData& instanceData) const;
protected:
	std::shared_ptr<ConstantBuffer> mMaterialBuf;		//! �}�e���A���p�萔�o�b�t�@
	Primitive::Material mMaterial;						//! �}�e���A��
};

