/*
	@file	CommandAllocator.h
	@author Ishibashi Ryuto
	@date	2018/07/11 ���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <string>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;

/**
*	@class	CommandAllocator
*	@brief	�R�}���h�A���P�[�^�̎��̂��Ǘ�����N���X
*/
class CommandAllocator
{
public:

	/**
	*	@brief	�f�X�g���N�^
	*/
	~CommandAllocator();

	/**
	*	@brief	�R�}���h�A���P�[�^�𐶐�����
	*
	*	@param[in] device			: D3D12�f�o�C�X
	*	@param[in] commandListType	: �R�}���h���X�g�̎��
	*	@param[in] name				: ���O
	*
	*	@retval ����������	: CommandAllocator��shared_ptr,
	*	@retval	�������s��	: nullptr
	*
	*	@note	CommandAllocator�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	*			���̂𒼐ڎ����Ƃ͂ł��Ȃ�
	*/
	static std::shared_ptr<CommandAllocator> Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring& name = L"");

	/**
	*	@brief	�R�}���h�A���P�[�^��ComPtr���擾����
	*	@return ID3D12CommandAllocator��ComPtr
	*/
	ComPtr<ID3D12CommandAllocator> GetAllocator() const;

	/**
	*	@brief	�R�}���h�A���P�[�^���w���I�y���[�^
	*/
	ID3D12CommandAllocator* const operator->() const;

private:
	/* �ϐ��錾 */
	// �R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;	//! �R�}���h�A���P�[�^�̎���

	/* ���[�J�����\�b�h */
	/**
	*	@biref	�R���X�g���N�^
	*/
	CommandAllocator();
};

