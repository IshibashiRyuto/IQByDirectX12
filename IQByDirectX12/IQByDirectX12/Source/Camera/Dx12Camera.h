/**
*	@file	Dx12Camera.h
*	@brief	Dx12Camera�N���X�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/10/23	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Camera.h"

/* �N���X�g�p�錾 */
class Device;
class ConstantBuffer;
class DescriptorHeap;
class GraphicsCommandList;
class RootSignature;

/**
*	@class	Dx12Camera
*	@brief	Dx12�Ŏg�p���邽�߂̃J�����N���X
*			���g�Œ萔�o�b�t�@�A�q�[�v�������A
*			�`�掞�̃q�[�v�Z�b�g�܂ōs��
*/
class Dx12Camera :
	public Camera
{
public:
	/**
	*	@brief �R���X�g���N�^
	*
	*	@param[in]	pos			: �����ʒu���W
	*	@param[in]	direction	: �����J��������
	*	@param[in]	projType	: ���e���@
	*	@param[in]	projParam	: ���e�p�����[�^
	*/
	Dx12Camera(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/**
	*	@brief �f�X�g���N�^
	*/
	~Dx12Camera();

	/**
	*	@brief		dx12�J�����N���X�𐶐�����
	*
	*	@param[in]	pos			: �����ʒu
	*	@param[in]	direction	: ��������
	*	@param[in]	projType	: ���e���@
	*	@param[in]	projParam	: ���e�p�����[�^
	*	@param[in]	device		: dx12�f�o�C�X
	*/
	static std::shared_ptr<Dx12Camera> Create(
		const Math::Vector3& pos,
		const Math::Vector3& direction,
		ProjectionType projType,
		const ProjectionParam& projParam,
		std::shared_ptr<Device> device);

	/**
	*	@brief	�s������X�V����
	*/
	void UpdateMatrix();

	/**
	*	@brief	���[���h�ϊ��s����Z�b�g����
	*
	*	@param[in]	worldMat	: ���[���h�ϊ��s��
	*/
	void SetWorldMatrix(const Math::Matrix4x4& worldMat);

	/**
	*	@brief	�f�X�N���v�^�����[�g�V�O�l�`���ɃZ�b�g����
	*
	*	@param[in]	rootSignature	: �o�C���h�Ώۂ̃��[�g�V�O�l�`��
	*	@param[in]	rootParamIndex	: �o�C���h���郋�[�g�p�����[�^�̃C���f�b�N�X
	*/
	void BindDescriptorHeap(std::shared_ptr<RootSignature> rootSignature, int rootParamIndex);

private:
	Math::Matrix4x4 mWorldMatrix;						//! World�s��
	std::shared_ptr<ConstantBuffer> mCameraDataBuffer;	//! �J�������i�[�萔�o�b�t�@
	std::shared_ptr<DescriptorHeap> mCameraHeap;		//! �J�������i�[�q�[�v
	
	/**
	*	@struct	CameraData
	*	@brief	�V�F�[�_�ɓn���J�������
	*/
	struct CameraData
	{
		Math::Matrix4x4 wvpMat;		//! WVP�ϊ��s��
		Math::Matrix4x4 worldMat;	//! World�s��
		Math::Matrix4x4 viewMat;	//! View�s��
		Math::Matrix4x4 projMat;	//! Projection�s��
		Math::Vector4	eyePos;		//! ���_
		Math::Vector4	eyeDir;		//! �����x�N�g��
		Math::Vector4	upper;		//! ������x�N�g��
		Math::Vector4	right;		//! �E�����x�N�g��
	};
};

