/*
	Model.h
	�Q�[�����̃��f�����Ɉ������̊Ǘ��ƕ`�擙���s��
	@author Ishibashi Ryuto
	@date
	2018/09/03	���ō쐬
	2018/11/28	����n���h���������f���̍쐬���O������s����悤�ɂ���
				���f���̕������s�����\�b�h��ǉ�
				(�����������\�b�h�͓��ꃂ�f���ł��ʃC���X�^���X����)
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>

// ����w�b�_�C���N���[�h
#include "../Math/Math.h"

// �N���X�g�p�錾
class ModelDataManager;
class CommandList;
/*Debug*/
class DescriptorHeap;
class Pose;
/*DebugEnd*/
using Microsoft::WRL::ComPtr;
class InstancingDataManager;

class Model
{
public:
	/// �R���X�g���N�^
	Model();
	Model(int modelHandle);

	/// �f�X�g���N�^
	~Model();

	/// @fn Create
	/// ���f���N���X�𐶐�����
	/// @param[in] modelHandle	: ���f���n���h��
	/// @retval ���f���̃X�}�[�g�|�C���^
	static std::shared_ptr<Model> Create(int modelHandle);

	/**
	*	@brief	���f���N���X�𐶐�����
	*
	*	@param[in]	model	: �����̃��f���f�[�^
	*/
	static std::shared_ptr<Model> Create(std::shared_ptr<Model> model);

	/**
	*	@brief	���f���𕡐�����
	*
	*	@param[in]	model	: �������郂�f���f�[�^
	*/

	/// @fn SetPosition
	/// �ʒu���w�肷��
	/// @param[in] position : ���f���z�u�ʒu
	void SetPosition(const Math::Vector3& position);

	/// @fn SetRotation
	/// ���f���̉�]���w�肷��
	/// @param[in] rotation : �e����]�p�x
	void SetRotation(const Math::Vector3& rotation);

	/// @fn SetRotation
	/// ���f���̉�]���w�肷��
	/// @param[in] rotQuaternion : �e����]�p�x(quaternion)
	void SetRotation(const Math::Quaternion& rotQuaternion);

	/// @fn SetScale
	/// ���f���̊g�k�����w�肷��
	/// @param scale : ���f���X�P�[��
	void SetScale(const Math::Vector3& scale);
	void SetScale(float scale);

	/// @fn Draw
	/// ���f���̕`�揈�����s��
	void Draw() const;

	std::shared_ptr<DescriptorHeap> _DebugGetDescHeap();

	std::shared_ptr<Pose> _DebugGetPose();

private:
	// �ϐ��錾
	Math::Vector3		mPosition;		// �ʒu
	Math::Quaternion	mRotation;		// ��]
	Math::Vector3		mScale;			// �g�k
	Math::Matrix4x4		mModelMatrix;	// ���f���s��
	int					mModelHandle;	// ���f���n���h��
	InstancingDataManager& mInstancingDataManager;// �C���X�^���V���O�f�[�^�̃}�l�[�W��

	// ���[�J�����\�b�h��`
	/// @fn CalcModelMatrix
	/// ���݂̈ʒu�A��]�A�g�k���烂�f���s����v�Z����
	void CalcModelMatrix();
};

