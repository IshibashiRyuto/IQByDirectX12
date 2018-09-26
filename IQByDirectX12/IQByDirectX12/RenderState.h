/*
*	@file RenderState.h
*	@brief �����_�[�X�e�[�g�����܂Ƃ߂��t�@�C��
*	@author Ishibashi Ryuto
*	@date
*	2018/09/26 ���ō쐬
*/
#pragma once

/// @brief �A���t�@�u�����h�I�v�V����
enum class AlphaBlendType
{
	Opacity = 0,	//! �s����
	Blend = 1,		//! ����
	Add = 2,		//! ���Z
	Sub = 3,		//! ���Z
	Mul = 4,		//! ��Z
};

/// @brief �e�N�X�`���̃t�B���^�^�C�v
enum class TextureFilterType 
{
	Nearest = 0,	//! �j�A���X�g���
	Linear = 1,		//! �o�C���j�A���
};

/// @brief �e�N�X�`���̃��[�v�^�C�v
enum class TextureWrapType
{
	Repeat = 0,		//! �J��Ԃ�
	Clamp = 1,		//! �ł��؂�
};

enum class CullingType
{
	Frond = 0,
	Back = 1,
	Double = 2,
};

///	@brief �����_�[�X�e�[�g���W�񂵂��\����
struct RenderState
{
	bool				depthTest	: 1;	//! �[�x�e�X�g���s����	
	bool				depthWrite	: 1;	//! �[�x�o�b�t�@�ւ̏������݂��s����
	AlphaBlendType		alphaBlend;			//! �A���t�@�u�����h
	CullingType			cullingType;		//! �J�����O�^�C�v
	TextureFilterType	textureFilterType;	//! �e�N�X�`���t�B���^�^�C�v
};
