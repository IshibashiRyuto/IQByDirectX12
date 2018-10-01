/*
*	@file VMDLoader.h
*	@brief VMD�f�[�^��ǂݍ��ރN���X
*	@author Ishibashi Ryuto
*	@date
*	2018/09/25	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <string>

//�N���X�g�p�錾
class VMDData;
class Animation;


class VMDLoader
{
public:
	VMDLoader();
	~VMDLoader();

	/// @brief VMDLoader�C���X�^���X�𐶐�����
	/// @note ���̃N���X�̃C���X�^���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	static std::shared_ptr<VMDLoader> Create();

	/// @brief VMDData�����[�h����
	/// @param[in] filePath	:	���[�h����t�@�C���̃p�X
	/// @retval	�ǂݍ��ݐ���	: �A�j���[�V�����̃X�}�[�g�|�C���^
	/// @retval �ǂݍ��ݎ��s	: nullptr
	std::shared_ptr<Animation> Load(const std::string& filePath);
};

