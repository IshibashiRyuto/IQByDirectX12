/**
*	@file	PrimitivePlane.h
*	@brief	PrimitivePlane�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/28	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "PrimitiveModelData.h"

/**
*	@class	PrimitivePlane
*	@brief	�v���~�e�B�u�̕���
*/
class PrimitivePlane :
	public PrimitiveModelData
{
public:
	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	vertices		: ���_���
	*	@param[in]	indexes			: ���_�C���f�b�N�X���
	*	@param[in]	material		: �}�e���A�����
	*	@param[in]	pipelineState	: �p�C�v���C���X�e�[�g
	*	@param[in]	rootSignature	: ���[�g�V�O�l�`��
	*/
	PrimitivePlane(std::shared_ptr<Device> device,
		const std::vector<Primitive::Vertex>& vertices,
		const std::vector<short>& indexes,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineState,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);
	~PrimitivePlane();

	/**
	*	@brief	���𐶐�����
	*	
	*	@param[in]	device			: �����p�f�o�C�X
	*	@param[in]	size			: ���̃T�C�Y
	*	@param[in]	material		: �}�e���A��
	*	@param[in]	pipelineState	: �p�C�v���C���X�e�[�g
	*/
	static std::shared_ptr<PrimitivePlane> Create(
		std::shared_ptr<Device> device,
		const Math::Vector2& size,
		const Primitive::Material& material,
		std::shared_ptr<PipelineStateObject> pipelineState,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);

private:

	/**
	*	@brief	���_�����\�z����
	*
	*	@param[in]	size		: ���̃T�C�Y
	*	@param[out]	vertices	: ���_���i�[��
	*	@param[out] indexes		: ���_�C���f�b�N�X���i�[��
	*/
	static void ConstructVertices(const Math::Vector2& size, std::vector<Primitive::Vertex>& vertices, std::vector<short>& indexes);

};

