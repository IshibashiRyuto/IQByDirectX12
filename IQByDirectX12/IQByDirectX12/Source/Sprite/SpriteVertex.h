/**
*	@file	SpriteVertex.h
*	@brief	�X�v���C�g�̒��_�����L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/20	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "../Math/Math.h"

/**
*	@struct	SpriteVertex
*	@brief	�X�v���C�g�̒��_���
*/
struct SpriteVertex
{
	Math::Vector4 pos;	//! ���_���W
	Math::Vector2 uv;	//! UV���W(�e�N�X�`���\��t���p)
};
