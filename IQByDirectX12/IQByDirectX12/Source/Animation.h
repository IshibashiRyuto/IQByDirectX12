/*
*	@file Animation.h
*	@brief �A�j���[�V�����Ǘ��N���X
*	@author Ishibashi Ryuto
*	@date
*	2018/10/01	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <map>
#include <memory>

// ����w�b�_�C���N���[�h
#include "Math/Math.h"

// �N���X�g�p�錾
class Bone;
class Pose;

struct KeyFrameData
{
	std::shared_ptr<Bone> bone;
};

class Animation
{
public:
	Animation();
	~Animation();

	/// @brief �A�j���[�V�������̐���
	/// @note Animation�N���X�̃C���X�^���X�͂��̃��\�b�h��ʂ��Ă̂ݐ���
	/// @retval	�������� : �C���X�^���X�̃X�}�[�g�|�C���^
	/// @retval �������s : nullptr
	static std::shared_ptr<Animation> Create();

	/// @brief		�A�j���[�V��������ǉ�����
	/// @param[in]	boneName		: �{�[����
	/// @param[in]	frame			: �L�[�t���[��
	/// @param[in]	animationData	: �A�j���[�V�����f�[�^
	void AddKeyFrameData(const std::wstring& boneName, int frame, const KeyFrameData& keyFrameData);

	/// @brief �w�肵���t���[���̃��[�V���������f���̎p�����Ɋi�[����
	/// @param[in]	frame	: ���[�V�����t���[��
	/// @param[out] pose	: �p�����
	/// @note �ďo�����A�p���v�Z�܂ōs��
	void SetPose(int frame, std::shared_ptr<Pose> pose);

private:
	std::map<std::wstring, std::map< int, KeyFrameData> > mAnimationData;	//! �A�j���[�V�����f�[�^
};

