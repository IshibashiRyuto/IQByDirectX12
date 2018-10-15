/*
*	@file DebugLayer.h
*	@brief �f�o�b�O�֘A�������܂Ƃ߂��N���X
*	@author Ishibashi Ryuto
*	@date
*	2018/09/25	���ō쐬
*/
#pragma once
//�V�X�e���w�b�_�C���N���[�h
#include <string>

enum class OutputDestination
{
	Console,
	Window,
	Text,
};

class DebugLayer
{
public:
	/// @brief �f�X�g���N�^
	~DebugLayer();

	/// @brief �V���O���g���C���X�^���X���擾����
	static DebugLayer& GetInstance()
	{
		static DebugLayer inst;
		return inst;
	}

	/// @brief �f�o�b�O���b�Z�[�W���o�͂���
	void PrintDebugMessage(const std::string& message);
	void PrintDebugMessage(const std::wstring& message);

	/// @brief		�f�o�b�O���b�Z�[�W�̏o�͐��ݒ肷��
	/// @param[in]	OutputDestination
	void SetOutputDestination(OutputDestination outputDestination);

private:
	// �萔��`
	const char* OUTPUT_FILE_NAME{ "DebugInfo.txt" };	//! �f�o�b�N���e�L�X�g�̏o�͐�t�@�C��

	// �ϐ��錾
	OutputDestination mOutputDestination{};	//! �f�o�b�N���̏o�͐�(�f�t�H���g�ŃR���\�[���o��)

	// ���[�J�����\�b�h��`
	/// @brief �R���X�g���N�^
	DebugLayer();
};