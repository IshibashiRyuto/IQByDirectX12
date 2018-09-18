/*
	CommandQueue.h
	�R�}���h�L���[�̎��̂��Ǘ�����N���X
	@author Ishibashi Ryuto
	@history
	2018/07/11 ���ō쐬
	2018/07/30 ExecuteCommandLists, Signal��ǉ�
*/
#pragma once
/* �V�X�e���w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/// @fn Create
	/// �R�}���h�L���[�̍쐬
	/// @param[in] device �f�o�C�X�̃|�C���^
	/// @retval ����������: CommandQueue��shared_ptr, �������s��: nullptr
	static std::shared_ptr<CommandQueue> Create(std::shared_ptr<Device> device);

	/// @fn Create
	/// �R�}���h�L���[�̍쐬
	/// @param[in] device �f�o�C�X�̃|�C���^
	/// @param[in] commandQueueDesc �R�}���h�L���[�̐ݒ荀��
	/// @retval ����������: CommandQueue��shared_ptr, �������s��: nullptr
	static std::shared_ptr<CommandQueue> Create(std::shared_ptr<Device> device, const D3D12_COMMAND_QUEUE_DESC& commandQueueDesc);

	/// @fn Get
	/// �R�}���h�L���[��Com�|�C���^���擾����
	/// @retval ID3D12CommandQueue��ComPtr
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

	/// @fn ExecuteCommandLists
	/// �R�}���h�����s����
	/// @param[in] numCommandLists	:	�R�}���h���X�g�̌�
	/// @param[in] ppCommandLists	:	�R�}���h���X�g�z��ւ̃|�C���^
	void ExecuteCommandList(UINT numCommandLists, ID3D12CommandList *const *ppCommandLists);

	/// @fn Signal
	/// �R�}���h�L���[�̎��s��҂�
	void Signal();

private:
	/* �ϐ��錾 */
	ComPtr<ID3D12CommandQueue> mCommandQueue;	// �R�}���h���X�g�̎���
	UINT mFenceValue;							// �t�F���X�p�����[�^
	ComPtr<ID3D12Fence> mFence;					// �t�F���X�I�u�W�F�N�g
	HANDLE mFenceEvent;							// �t�F���X�C�x���g
};

