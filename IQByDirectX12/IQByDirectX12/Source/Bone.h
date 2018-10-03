/*
	Bone.h
	�{�[�������i�[����N���X
	@author Ishibashi Ryuto
	@date 
	2018/09/11	���ō쐬
	2018/09/14	�R���X�g���N�^���B���Ashared_ptr��Ԃ�Create�֐����쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>

// ����w�b�_�C���N���[�h
#include "Math/Math.h"

class Bone
{
public:

	/// �f�X�g���N�^
	~Bone();

	/// �{�[�����𐶐�����
	static std::shared_ptr<Bone> Create(const Math::Vector3 & headPosition);

	/// �{�[���̉�]��ݒ肷��
	void SetRotation(const Math::Quaternion& rotation);
	void SetRotation(const Math::Vector3& rotation);
	void SetRotation(float rotX, float rotY, float rotZ);

	/// �������̎g�p�ݒ���s��
	void SetLimitAxis(bool isUse, const Math::Vector3& axis);

	/// �{�[������]������
	void Rotate(const Math::Matrix4x4& matrix);

	/// �{�[���̉�]�����擾����
	const Math::Quaternion& GetRotation() const;

	/// �{�[����]�s����擾����
	const Math::Matrix4x4 GetBoneMatrix() const;
private:
	/* �����o�ϐ���` */
	Math::Vector3 mOriginHeadPosition;		// �{�[�������w�b�h�ʒu
	Math::Vector3 mRotatedHeadPosition;		// ��]�����̃{�[���w�b�h�ʒu
	Math::Quaternion mRotation;				// �{�[���̉�]��
	Math::Quaternion mRotatedRotation;		// ��]�����̃{�[����]��
	Math::Matrix4x4 mMatrix;
	Math::Vector3	mLimitAxis;				// ������
	bool			mIsUseLimitAxis;		// �������@�\���g�p���邩
	/* �v���C�x�[�g���\�b�h��` */
	/// �R���X�g���N�^
	Bone(const Math::Vector3 & headPosition);
};

