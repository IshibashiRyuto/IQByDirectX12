/*
	CommandAllocator.h
	�R�}���h�A���P�[�^�̎��̂��Ǘ�����N���X
	@author Ishibashi Ryuto
	@history
	2018/07/11 ���ō쐬
*/
#pragma once
/* �V�X�e���w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;

class CommandAllocator
{
public:

	/// �f�X�g���N�^
	~CommandAllocator();

	/// @fn Create
	/// �R�}���h�A���P�[�^�𐶐�����
	/// @note CommandAllocator�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// ���̂𒼐ڎ����Ƃ͂ł��Ȃ��B
	/// @param[in] device			D3D12�f�o�C�X
	/// @param[in] commandListType	�R�}���h���X�g�̎��
	/// @retval ����������:CommandAllocator��shared_ptr, �������s��: nullptr
	static std::shared_ptr<CommandAllocator> Create(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE commandListType);

	/// @fn Get
	/// �R�}���h�A���P�[�^��ComPtr���擾����
	/// @retval ID3D12CommandAllocator��ComPtr
	ComPtr<ID3D12CommandAllocator> Get() const;

private:
	/* �ϐ��錾 */
	// �R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;

	/* ���[�J�����\�b�h */
	/// �R���X�g���N�^
	CommandAllocator();
};

