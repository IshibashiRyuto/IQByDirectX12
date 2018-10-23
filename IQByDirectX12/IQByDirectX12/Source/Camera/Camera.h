/*
	@file Camera.h
	@brief �Q�[�����̃J���������i�[����
	@author Ishibashi Ryuto
	@date 2018/10/21	���ō쐬
*/
#pragma once
/* �V�X�e���w�b�_�C���N���[�h */
#include <memory>
#include <map>
#include <functional>

/* ����w�b�_�C���N���[�h */
#include "../Math/Math.h"

enum class ProjectionType
{
	Perspective,
	Orthographic
};

/// @brief �ˉe�s��̐ݒ�p�����[�^
struct ProjectionParam
{
	float width;
	float height;
	float fov;
	float nearZ;
	float farZ;
};

/// @brief �J���������Ǘ�����
class Camera
{
public:
	/// @brief	�R���X�g���N�^
	Camera(const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam);

	/// @brief	�f�X�g���N�^
	~Camera();

	/// @brief	�J�����N���X�𐶐�����
	/// @note	�J�����N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// @param[in]	pos			: �����ʒu
	/// @param[in]	direction	: ��������
	/// @param[in]	projType	: �ˉe���@
	/// @param[in]	projParam	: �ˉe���@�̃p�����[�^
	static std::shared_ptr<Camera> Create(const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam);

	/// @brief	�J�����̒����_���w�肷��
	/// @param[in]	targetPos :�J�����̒����_
	void SetTargetPos(const Math::Vector3& targetPos);

	/// @brief	�J�����̎��_(�ʒu)���w�肷��
	/// @param[in]	pos	: �J�����̈ʒu
	void SetPos(const Math::Vector3& pos);

	/// @brief	�J�����̕������w�肷��
	/// @param[in]	direction	: �J�����̌���
	void SetDirection(const Math::Vector3& direction);

	/// @brief	�J�����̕������w�肷��
	/// @param[in]	direction	: z������������̃J�����̉�]
	void SetDirection(const Math::Quaternion& direction);

	/// @brief	�J�����̎ˉe���@���w�肷��
	void SetProjectionType(ProjectionType projType, const ProjectionParam& projParam);

	/// @brief	�J��������]������
	/// @param[in]	rotation	: ��]��
	void Rotate(const Math::Quaternion& rotation);

	/// @brief	�J�������A�b�p�[�x�N�g���ƂƂ��ɉ�]������
	void RotateWithUpper(const Math::Quaternion& rotation);

	/// @brief	�J�������ړ�������
	/// @param[in]	movement	: �ړ���
	void Move(const Math::Vector3& movement);

	/// @brief	�J�����s����X�V����
	void UpdateMatrix();

	/// @brief	�r���[�s����擾����
	const Math::Matrix4x4& GetViewMatrix() const;
	
	/// @brief	�v���W�F�N�V�����s����擾����
	const Math::Matrix4x4& GetProjMatrix() const;

private:
	/* �ϐ��錾 */
	Math::Vector3 mPosition;		//! �J�����̈ʒu
	Math::Quaternion mDirection;	//! �J�����̌���
	Math::Vector3 mUpper;			//! �J�����̏�����x�N�g��
	Math::Matrix4x4 mViewMat;		//! �r���[�s��
	Math::Matrix4x4 mProjMat;		//! �v���W�F�N�V�����s��
	std::map<ProjectionType, std::function<Math::Matrix4x4(const ProjectionParam&)>> CreateProjMat;	//! �ˉe�s�񐶐����s���֐�

	/* ���[�J�����\�b�h��` */
	
	/// @brief	�������e�s����쐬����
	static Math::Matrix4x4 CreatePerspectiveMatrix(const ProjectionParam& projParam);

	/// @brief	���s���e�s����쐬����
	static Math::Matrix4x4 CreateOrthographicMatrix(const ProjectionParam& projParam);

	/// @brief	�r���[�s����X�V����
	void UpdateViewMat();
};

