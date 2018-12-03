/**
*	@file	PrimitiveCreator.h
*	@brief	PrimitiveCreator�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/29	���ō쐬
*/
#pragma once
#include <memory>
#include "../Math/Math.h"
#include "PrimitiveData.h"

/* �N���X�g�p�錾 */
class Device;
class Model;
class PipelineStateObject;
class RootSignature;

/**
*	@class	PrimitiveCreator
*	@brief	�v���~�e�B�u���f���������Ǘ�����N���X
*/
class PrimitiveCreator
{
public:
	~PrimitiveCreator();

	/**
	*	@brief	�C���X�^���X���擾����
	*/
	static PrimitiveCreator& GetInstance()
	{
		static PrimitiveCreator inst;
		return inst;
	}

	/**
	*	@brief	����������
	*
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	pipelineState	: �v���~�e�B�u�p�p�C�v���C���X�e�[�g
	*	@param[in]	rootSignature	: �v���~�e�B�u�p���[�g�V�O�l�`��
	*/
	void Initialize(std::shared_ptr<Device> device, std::shared_ptr<PipelineStateObject> pipelineState,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	���𐶐�����
	*	
	*	@param[in]	size		: ���̃T�C�Y
	*	@param[in]	material	: �}�e���A��
	*/
	std::shared_ptr<Model> CreatePlane(const Math::Vector2& size, const Primitive::Material& material = Primitive::Material());

private:
	/**
	*	@brief	�R���X�g���N�^
	*/
	PrimitiveCreator();

	/**
	*	@brief	�R�s�[�R���X�g���N�^
	*/
	PrimitiveCreator(PrimitiveCreator &){}

	/**
	*	@brief	�R�s�[����̋֎~
	*/
	void operator=(PrimitiveCreator& ) { }
	
	bool mIsInit{ false };									//! �������������I�����Ă��邩
	std::shared_ptr<Device> mDevice;						//! �f�o�C�X
	std::shared_ptr<PipelineStateObject> mPipelineState;	//! �v���~�e�B�u�p�p�C�v���C���X�e�[�g
	std::shared_ptr<RootSignature> mRootSignature;			//! ���[�g�V�O�l�`��
	std::shared_ptr<PipelineStateObject> mShadowPSO;		//! �[�x�������ݗp
};

