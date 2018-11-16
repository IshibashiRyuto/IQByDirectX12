/*
	@file	DepthBuffer.h
	@brief	DepthBuffer�N���X���L�q����
	@author Ishibashi Ryuto
	@date	2018/07/12	���ō쐬
	@date	2018/11/16	�o�b�t�@�̎擾�ASRV�̐ݒ肨��ю擾���\�b�h��ǉ�
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;

/// @class DepthBuffer
/// 
///	�f�v�X�o�b�t�@�̃��\�[�X��DSV���Ǘ�����N���X
///	(����X�e���V���o�b�t�@���K�v�ɂȂ�����܂��������܂�)
class DepthBuffer
{
public:
	/// �f�X�g���N�^
	~DepthBuffer();

	/// @fn Create
	/// �f�v�X�o�b�t�@��DSV���Ǘ�����N���X�𐶐�����
	/// @note DepthBuffer�N���X�͂��̃��\�b�h�ł̂ݐ����\
	/// @param[in] device:			ID3D12�f�o�C�X
	/// @param[in] windowWidth:		�E�B���h�E��
	/// @param[in] windowHeight:	�E�B���h�E����
	static std::shared_ptr<DepthBuffer> Create(ComPtr<ID3D12Device> device, int windowWidth, int windowHeight);

	/// @fn GetDSVHandle
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle();

	/// @fn ClearDepthBuffer
	void ClearDepthBuffer(ComPtr<ID3D12GraphicsCommandList> commandList);


	/// @brief	�[�x�o�b�t�@���擾����
	ComPtr<ID3D12Resource> GetDepthBufferResource();

	/// @brief	�V�F�[�_���\�[�X�r���[���擾����
	const D3D12_SHADER_RESOURCE_VIEW_DESC& DebugShaderResourceView();
private:
	/*�萔��`*/
	const D3D12_CLEAR_VALUE DEPTH_CLEAR_VALUE;	// �[�x�N���A�̍ۂɎg�p����l
	
	/*�ϐ��錾*/
	ComPtr<ID3D12Resource> mDepthBuffer;		// �[�x�o�b�t�@���\�[�X
	ComPtr<ID3D12DescriptorHeap> mDSVDescHeap;	// �[�x�o�b�t�@�f�X�N���v�^�q�[�v
	D3D12_CPU_DESCRIPTOR_HANDLE mDSVCPUHandle;		// DSV��CPU�n���h��
	D3D12_SHADER_RESOURCE_VIEW_DESC mSRVDesc;	// �V�F�[�_���\�[�X�r���[

	/*���[�J�����\�b�h��`*/
	/// �R���X�g���N�^
	/// @param[in] depthClearValue: �[�x�o�b�t�@���N���A����p�����[�^
	DepthBuffer(const D3D12_CLEAR_VALUE& depthClearValue);

};

