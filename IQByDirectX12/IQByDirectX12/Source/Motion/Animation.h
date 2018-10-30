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
#include <vector>

// ����w�b�_�C���N���[�h
#include "../Math/Math.h"

// �N���X�g�p�錾
class Bone;
class Pose;

struct KeyFrameData
{
	std::shared_ptr<Bone> bone;
	Math::Vector2 bazieCtrlPoint1;
	Math::Vector2 bazieCtrlPoint2;
};

class Animation
{
public:
	Animation();
	~Animation();

	/// @brief �A�j���[�V�������̐���
	/// @note Animation�N���X�̃C���X�^���X�͂��̃��\�b�h��ʂ��Ă̂ݐ���
	/// @param[in] frameCount : �A�j���[�V�����̑��t���[����
	/// @retval	�������� : �C���X�^���X�̃X�}�[�g�|�C���^
	/// @retval �������s : nullptr
	static std::shared_ptr<Animation> Create();

	/// @brief		�A�j���[�V��������ǉ�����
	/// @param[in]	boneName		: �{�[����
	/// @param[in]	frame			: �L�[�t���[��
	/// @param[in]	keyFrameData	: �L�[�t���[���̃A�j���[�V�����f�[�^
	void AddKeyFrameData(const std::wstring& boneName, int frame, const KeyFrameData& keyFrameData);

	/// @brief �w�肵���t���[���̃��[�V���������f���̎p�����Ɋi�[����
	/// @param[in]	frame	: ���[�V�����t���[��
	/// @param[out] pose	: �p�����
	/// @note �ďo�����A�p���v�Z�܂ōs��
	void SetPose(int frame, std::shared_ptr<Pose> pose);

	///	@brief	�A�j���[�V�����̎��Ԃ��擾����
	int GetDuration() const;

private:
	int mDuration;														//! �t���[���J�E���g
	std::map<std::wstring, std::map< int, KeyFrameData> > mAnimationData;	//! �A�j���[�V�����f�[�^

	float GetBezierCurveYWithNewton(float x, const Math::Vector2& ctrlPoint1, const Math::Vector2& ctrlPoint2, int n = 16);

	float GetBezierCurveYWithBisection(float x, const Math::Vector2& ctrlPoint1, const Math::Vector2& ctrlPoint2, int n = 8);
};

