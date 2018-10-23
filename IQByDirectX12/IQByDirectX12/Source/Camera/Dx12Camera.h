#pragma once
#include "Camera.h"

/* �N���X�g�p�錾 */
class Device;
class ConstantBuffer;
class DescriptorHeap;
class GraphicsCommandList;

/// @brief Dx12�Ŏg�p���邽�߂̃J�����N���X
///	���g�Œ萔�o�b�t�@�A�q�[�v�������A
/// �`�掞�̃q�[�v�Z�b�g�܂ōs��
class Dx12Camera :
	public Camera
{
public:
	/// @brief �R���X�g���N�^
	Dx12Camera(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/// @brief �f�X�g���N�^
	~Dx12Camera();

	/// @brief dx12�J�����N���X�𐶐�����
	/// @param[in]	pos			: �����ʒu
	/// @param[in]	direction	: ��������
	/// @param[in]	projType	: �ˉe���@
	/// @param[in]	projParam	: �ˉe���@�̃p�����[�^
	/// @param[in]	device		: dx12�f�o�C�X
	static std::shared_ptr<Dx12Camera> Create(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/// @brief	�s������X�V����
	void UpdateMatrix();

	/// @brief ���[���h�ϊ��s����Z�b�g����
	/// @param[in]	worldMat	: ���[���h�ϊ��s��
	void SetWorldMatrix(const Math::Matrix4x4& worldMat);

	/// @brief �o�b�t�@�����R�}���h�ɏ悹��
	/// @param[in]	commandList		: �R�}���h���X�g
	/// @param[in]	rootParamIndex	: �o�b�t�@���Z�b�g���郋�[�g�p�����[�^�̃C���f�b�N�X
	void SetCameraData(std::shared_ptr<GraphicsCommandList> commandList, int rootParamIndex);

private:
	Math::Matrix4x4 mWorldMatrix;	//! World�s��
	std::shared_ptr<ConstantBuffer> mCameraDataBuffer;	//! �J�������i�[�o�b�t�@
	std::shared_ptr<DescriptorHeap> mCameraHeap;	//! �J�������q�[�v
	
	/// @brief �V�F�[�_�ɓn���J�������
	struct CameraData
	{
		Math::Matrix4x4 wvpMat;		//! WVP�ϊ��s��
		Math::Matrix4x4 worldMat;	//! World�s��
		Math::Matrix4x4 viewMat;	//! View�s��
		Math::Matrix4x4 projMat;	//! Projection�s��
		Math::Vector3	eyePos;		//! ���_
		Math::Vector3	eyeDir;		//! �����x�N�g��
		Math::Vector3	upper;		//! ������x�N�g��
		Math::Vector3	right;		//! �E�����x�N�g��
	};
};

