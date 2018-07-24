/*
	CommandQueue.h
	�R�}���h�L���[�̎��̂��Ǘ�����N���X
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

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/// @fn Create
	/// �R�}���h�L���[�̍쐬
	/// @param[in] device �f�o�C�X�̃|�C���^
	/// @retval ����������: CommandQueue��shared_ptr, �������s��: nullptr
	static std::shared_ptr<CommandQueue> Create(ComPtr<ID3D12Device> device);

	/// @fn Create
	/// �R�}���h�L���[�̍쐬
	/// @param[in] device �f�o�C�X�̃|�C���^
	/// @param[in] commandQueueDesc �R�}���h�L���[�̐ݒ荀��
	/// @retval ����������: CommandQueue��shared_ptr, �������s��: nullptr
	static std::shared_ptr<CommandQueue> Create(ComPtr<ID3D12Device> device, const D3D12_COMMAND_QUEUE_DESC& commandQueueDesc);

	/// @fn Get
	/// �R�}���h�L���[��Com�|�C���^���擾����
	/// @retval ID3D12CommandQueue��ComPtr
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

private:
	/* �ϐ��錾 */
	ComPtr<ID3D12CommandQueue> mCommandQueue;
};

