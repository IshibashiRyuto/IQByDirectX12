/**
*	@file	PrimitiveData.h
*	@brief	�v���~�e�B�u���f���Ɋւ�������L�q����
*	@author	IshibashiRyuto
*	@date	2018/11/28	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "../Math/Math.h"

/**
*	@namespace	Primitive
*	@brief	�v���~�e�B�u�Ɋւ�������܂Ƃ߂����O���
*/
namespace Primitive
{
	/**
	*	@struct	Vertex
	*	@brief	���_���
	*/
	struct Vertex
	{
		Math::Vector3 pos;		//! �ʒu���
		Math::Vector3 normal;	//! �@��
		Math::Vector2 uv;		//! �e�N�X�`���}�b�s���O�pUV
		
		/**
		*	@brief	�R���X�g���N�^
		*/
		Vertex();

		/**
		*	@brief	�R���X�g���N�^
		*	
		*	@param[in]	pos		: �ʒu���
		*	@param[in]	normal	: �@�����
		*	@param[in]	uv		: uv���
		*/
		Vertex(const Math::Vector3& pos, const Math::Vector3& normal, const Math::Vector2 & uv);
	};

	/**
	*	@struct	Material
	*	@brief	�}�e���A�����
	*/
	struct Material
	{
		Math::Vector4 diffuse;	//!	�f�B�t���[�Y
		Math::Vector3 specular; //! �X�y�L�����[�J���[
		float specularity;		//! �X�y�L��������(�ݏ�W��)
		Math::Vector3 ambient;	//! �A���r�G���g�J���[

		/**
		*	@brief	�R���X�g���N�^
		*/
		Material();

		/**
		*	@brief	�R���X�g���N�^
		*
		*	@param[in]	diffuse		: �f�B�t���[�Y�J���[
		*	@param[in]	ambient		: �A���r�G���g�J���[
		*	@param[in]	specular		: �X�y�L�����[�J���[
		*	@param[in]	specularity	: �X�y�L��������(�ݏ�W��)
		*/
		Material(const Math::Vector4& diffuse, const Math::Vector3& ambient, const Math::Vector3& specular, float specularity);
	};

	/**
	*	@typedef	Index
	*	@brief		�C���f�b�N�X�f�[�^
	*/
	typedef short Index;
}