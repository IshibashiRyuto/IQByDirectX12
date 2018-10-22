/*
	@file Camera.h
	@brief �Q�[�����̃J���������i�[����
	@author Ishibashi Ryuto
	@date 2018/10/21	���ō쐬
*/
#pragma once

/* ����w�b�_�C���N���[�h */
#include "../Math/Math.h"

enum class ProjectionType
{
	Perspective,
	Orthographic,
};

/// @brief �ˉe�s��̐ݒ�p�����[�^
struct ProjectionParam
{
	float near;
	float far;
	float aspect;
	float fov;
};

/// @brief �J���������Ǘ�����
class Camera
{
public:
	Camera();
	~Camera();

	/// @brief �J�����̒����_���w�肷��
	/// @param[in]	targetPos :�J�����̒����_
	void SetTargetPos(const Math::Vector3& targetPos);

	/// @brief �J�����̎��_(�ʒu)���w�肷��
	/// @param[in]	pos	: �J�����̈ʒu
	void SetPos(const Math::Vector3& pos);

	/// @brief �J�����̕������w�肷��
	/// @param[in]	direction	: �J�����̌���
	void SetDirection(const Math::Vector3& direction);

	/// @brief �J�����̕������w�肷��
	/// @param[in]	direction	: z������������̃J�����̉�]
	void SetDirection(const Math::Quaternion& direction);

	/// @brief �J�����̎ˉe���@���w�肷��
	void SetProjection(ProjectionType projType, const ProjectionParam& projParam);

private:
	Math::Vector3 mPosition;		//! �J�����̈ʒu
	Math::Vector3 mDirection;		//! �J�����̌���
	Math::Matrix4x4 mViewMat;		//! �r���[�s��
	Math::Matrix4x4 mProjMat;		//! �v���W�F�N�V�����s��
};

