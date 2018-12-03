/*
*	@file	DebugLayer.h
*	@brief	DebugLayer�̐錾���L�q����
*	@author Ishibashi Ryuto
*	@date	2018/09/25	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <string>

/**
*	@enum	OutputDestination
*	@brief	�f�o�b�N�o�͐�
*/
enum class OutputDestination
{
	Console,	//! �R���\�[��
	Window,		//! �|�b�v�A�b�v�E�B���h�E
	Text,		//! �e�L�X�g�f�[�^
};

/**
*	@class	DebugLayer
*	@brief �f�o�b�O�֘A�������܂Ƃ߂��N���X
*/
class DebugLayer
{
public:
	/**
	*	@brief �f�X�g���N�^
	*/
	~DebugLayer();

	/**
	*	@brief �V���O���g���C���X�^���X���擾����
	*/
	static DebugLayer& GetInstance()
	{
		static DebugLayer inst;
		return inst;
	}

	/**
	*	@brief �f�o�b�O���b�Z�[�W���o�͂���
	*
	*	@param[in]	message	: ���b�Z�[�W
	*/
	void PrintDebugMessage(const std::string& message);
	void PrintDebugMessage(const std::wstring& message);

	/**
	*	@brief		�f�o�b�O���b�Z�[�W�̏o�͐��ݒ肷��
	*
	*	@param[in]	OutputDestination	: �o�͐�
	*/
	void SetOutputDestination(OutputDestination outputDestination);

private:
	// �萔��`
	const char* OUTPUT_FILE_NAME{ "DebugInfo.txt" };	//! �f�o�b�N���e�L�X�g�̏o�͐�t�@�C��

	/* �ϐ��錾 */
	OutputDestination mOutputDestination{};				//! �f�o�b�N���̏o�͐�(�f�t�H���g�ŃR���\�[���o��)

	/* ���[�J�����\�b�h��` */

	/**
	*	@brief	���������̋֎~
	*/
	DebugLayer();

	/**
	*	@brief	�R�s�[�R���X�g���N�^��p���ẴR�s�[�֎~
	*/
	DebugLayer(const DebugLayer&) {}

	/**
	*	@brief	�R�s�[���Z�q��p���ẴR�s�[�֎~
	*/
	void operator=(const DebugLayer&){}
};