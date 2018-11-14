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

///	@class	Sprite
/// @brief	2D�X�v���C�g�N���X
///
///	2D�I�u�W�F�N�g�ł���X�v���C�g�f�[�^���i�[����N���X
/// �ʒu�w��A��]�w��A�{���w�蓙�ݒ肵�ĕ`��\
///	2D�f�[�^�ł��邽�߁A2�����ȊO�̏��������Ȃ����ߒ���
/// 3D�ϊ��Ƃ��Ď�舵�����Ƃ��s��
///	3�����X�v���C�g����舵���ꍇ�ɂ�Plane�N���X���g�p���邱��
class Sprite
{
public:
	/// @brief	�R���X�g���N�^
	Sprite();

	/// @brief	�f�X�g���N�^
	~Sprite();

	/// @brief	�X�v���C�g���쐬����
	/// @retval	�X�v���C�g�̃X�}�[�g�|�C���^
	static std::shared_ptr<Sprite> Create();
	
	/// @brief	�X�v���C�g��`�悷��
	void Draw() const;

	/// @brief	�X�v���C�g�̕`��ʒu��ݒ肷��
	void SetPosition();

	/// @brief	�X�v���C�g�̉�]�p�x��ݒ肷��
	void SetRotation();

	/// @brief	�X�v���C�g�̊g�嗦��ݒ肷��

	///	@brief	�X�v���C�g�ɒ���t����e�N�X�`����ύX����
	void SetTexture();

private:

};

