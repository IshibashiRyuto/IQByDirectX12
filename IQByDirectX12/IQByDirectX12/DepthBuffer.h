/*
	DepthBuffer.h
	�f�v�X�o�b�t�@�̃��\�[�X��DSV���Ǘ�����N���X
	(����X�e���V���o�b�t�@���K�v�ɂȂ�����܂��������܂�)
	@author Ishibashi Ryuto
	@history
	2018/07/12 ���ō쐬
*/
#pragma once
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;

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

private:
	/*�萔��`*/
	const D3D12_CLEAR_VALUE DEPTH_CLEAR_VALUE;	// �[�x�N���A�̍ۂɎg�p����l
	
	/*�ϐ��錾*/
	ComPtr<ID3D12Resource> mDepthBuffer;		// �[�x�o�b�t�@���\�[�X
	ComPtr<ID3D12DescriptorHeap> mDSVDescHeap;	// �[�x�o�b�t�@�f�X�N���v�^�q�[�v
	D3D12_CPU_DESCRIPTOR_HANDLE mDSVCPUHandle;		// DSV��CPU�n���h��

	/*���[�J�����\�b�h��`*/
	/// �R���X�g���N�^
	/// @param[in] depthClearValue: �[�x�o�b�t�@���N���A����p�����[�^
	DepthBuffer(const D3D12_CLEAR_VALUE& depthClearValue);

};

