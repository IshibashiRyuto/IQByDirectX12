/*
*	@file	Sprite.h
*	@brief	Sprite�N���X���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/13	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include<memory>
#include "../Math/Math.h"

/* �N���X�g�p�錾 */
class Texture;
class InstancingDataManager;
class Device;
class PipelineStateObject;

/**	@class	Sprite
*	@brief	2D�X�v���C�g�N���X
*
*	2D�I�u�W�F�N�g�ł���X�v���C�g�f�[�^���i�[����N���X
*	�ʒu�w��A��]�w��A�{���w�蓙�ݒ肵�ĕ`��\
*	2D�f�[�^�ł��邽�߁A2�����ȊO�̏��������Ȃ����ߒ���
*	3D�ϊ��Ƃ��Ď�舵�����Ƃ��s��
*	3�����X�v���C�g����舵���ꍇ�ɂ�Plane�N���X���g�p���邱��
*/
class Sprite
{
public:
	/**
	*	@brief	�R���X�g���N�^
	*/
	Sprite(int spriteHandle);

	/**
	*	@brief	�f�X�g���N�^
	*/
	~Sprite();

	/**
	*	@brief	�X�v���C�g�쐬���Ɏg�p����p�C�v���C���X�e�[�g�I�u�W�F�N�g���Z�b�g����
	*	@param[in]	pso	: �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	*/
	static void SetPipelineStateObject(std::shared_ptr<PipelineStateObject> pso);

	/**
	*	@brief	�X�v���C�g���쐬����
	*	@param[in] texture	: �e�N�X�`��
	*	@retval	�X�v���C�g�̃X�}�[�g�|�C���^
	*	@note	�e�N�X�`���T�C�Y�̃X�v���C�g���쐬����
	*/
	static std::shared_ptr<Sprite> Create(std::shared_ptr<Device> device, std::shared_ptr<Texture> texture);
	


	/**
	*	@brief	�X�v���C�g��`�悷��
	*/
	void Draw() const;

	/**
	*	@brief	�X�v���C�g�̕`��ʒu��ݒ肷��
	*/
	void SetPosition(float x, float y);
	void SetPosition(const Math::Vector2& pos);

	/**
	*	@brief	�X�v���C�g�̉�]�p�x��ݒ肷��
	*/
	void SetRotation(float rad);

	/**
	*	@brief	�X�v���C�g�̊g�嗦��ݒ肷��
	*/
	void SetScale(float scale);
	void SetScale(const Math::Vector2& scale);

	/**
	*	@brief	�X�v���C�g�ɒ���t����e�N�X�`����ύX����
	*/
	void SetTexture(std::shared_ptr<Texture> texture);

	/**
	*	@brief	�X�v���C�g�𕡐�����
	*
	*	���݂̃X�v���C�g����p���āA�X�v���C�g�𕡐�����
	*	�������ꂽ�X�v���C�g�͌ʂ̈ʒu�A��]�p�A�X�P�[���������A
	*	���_�T�C�Y�ƃe�N�X�`���͋��L�����
	*	���������ق����`������͗ǂ��Ȃ邪�A�e�N�X�`���������ւ����
	*	���ׂẴe�N�X�`���������ς�邽�ߒ��ӂ��邱��
	*/
	std::shared_ptr<Sprite> Duplicate();

private:
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static std::shared_ptr<PipelineStateObject> sPSO;

	int mSpriteHandle;						//! �X�v���C�g�f�[�^�̃n���h��
	std::shared_ptr<Texture> mTexture;		//! �e�N�X�`��
	Math::Vector2 mPos;						//! �`��ʒu
	float mAngle;							//! ��]�p�x
	Math::Vector2 mScale;					//! �g�嗦
	Math::Matrix3x3 mInstMat;				//! �C���X�^���X�s��
	InstancingDataManager& mInstDataMgr;	//! �C���X�^���X�p�f�[�^�}�l�[�W���ւ̎Q��

	/**
	*	@brief	���݂̈ʒu�A��]�A�g�k����s����v�Z����
	*/
	void CalcMatrix();
};

