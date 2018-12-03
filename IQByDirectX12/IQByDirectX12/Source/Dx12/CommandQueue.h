/*
	@file	CommandQueue.h
	@biref	CommandQueue�̐錾���L�q����
	@author Ishibashi Ryuto
	@date	2018/07/11 ���ō쐬
			2018/07/30 ExecuteCommandLists, Signal��ǉ�
*/
#pragma once
/* �V�X�e���w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <vector>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;
class GraphicsCommandList;

/**
*	@class	CommandQueue
*	@brief	ID3D12CommandQueue�̃��b�p�[�N���X
*
*	�R�}���h�L���[�̃��b�p�[�N���X
*	�R�}���h���X�g�̎��s�A�t�F���X�����������Ă���
*/
class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/**
	*	@brief		�R�}���h�L���[�̍쐬
	*	@param[in]	device	:�f�o�C�X�̃|�C���^
	*	@retval		����������	: CommandQueue��shared_ptr
	*	@retval		�������s��	: nullptr
	*/
	static std::shared_ptr<CommandQueue> Create(std::shared_ptr<Device> device);

	/**
	*	@brief		�R�}���h�L���[�̍쐬
	*	@param[in]	device				: �f�o�C�X�̃|�C���^
	*	@param[in]	commandQueueDesc	: �R�}���h�L���[�̐ݒ荀��
	*	@retval ����������	: CommandQueue��shared_ptr
	*	@retval	�������s��	: nullptr
	*/
	static std::shared_ptr<CommandQueue> Create(std::shared_ptr<Device> device, const D3D12_COMMAND_QUEUE_DESC& commandQueueDesc);

	/*/
	*	@brief	�R�}���h�L���[��Com�|�C���^���擾����
	*
	*	@return ID3D12CommandQueue��ComPtr
	*/
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

	/**
	*	@brief		�R�}���h�����s����
	*	@param[in]	numCommandLists	:	�R�}���h���X�g�̌�
	*	@param[in]	ppCommandLists	:	�R�}���h���X�g�z��ւ̃|�C���^
	*/
	void ExecuteCommandList(UINT numCommandLists, ID3D12CommandList *const *ppCommandLists);

	/**
	*	@brief �R�}���h�����s����
	*	@param[in] commandList		: �R�}���h���X�g�N���X
	*/
	void ExecuteCommandList(const std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@brief �R�}���h�����s����
	*	@param[in] commandLists		: �R�}���h���X�g�N���X�̓��I�z��
	*/
	void ExecuteCommandList(const std::vector<std::shared_ptr<GraphicsCommandList>> & commandLists);

	/**
	*	@brief	�R�}���h�L���[�̎��s��҂�
	*/
	void Signal();

private:
	/* �ϐ��錾 */
	ComPtr<ID3D12CommandQueue> mCommandQueue;	//! �R�}���h�L���[�̎���
	UINT mFenceValue;							//! �t�F���X�p�����[�^
	ComPtr<ID3D12Fence> mFence;					//! �t�F���X�I�u�W�F�N�g
	HANDLE mFenceEvent;							//! �t�F���X�C�x���g
};

