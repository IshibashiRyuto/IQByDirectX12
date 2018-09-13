/*
	Bone.h
	�{�[�������i�[����N���X
	@author Ishibashi Ryuto
	@date 
	2018/09/11	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h

// ����w�b�_�C���N���[�h
#include "Math/Math.h"

class Bone
{
public:
	/// �R���X�g���N�^
	Bone(const Math::Vector3 & headPosition);

	/// �f�X�g���N�^
	~Bone();

	/// �{�[���̉�]��ݒ肷��
	void SetRotation(const Math::Quaternion& rotation);
	void SetRotation(const Math::Vector3& rotation);
	void SetRotation(float rotX, float rotY, float rotZ);

	/// �{�[������]������
	void Rotate(const Math::Matrix4x4& matrix);

	/// �{�[���̉�]�����擾����
	const Math::Quaternion& GetRotation() const;

	/// �{�[����]�s����擾����
	const Math::Matrix4x4 GetBoneMatrix() const;
private:
	Math::Vector3 mOriginHeadPosition;		// �{�[�������w�b�h�ʒu
	Math::Vector3 mRotatedHeadPosition;		// ��]�����̃{�[���w�b�h�ʒu
	Math::Quaternion mRotation;				// �{�[���̉�]��
};

