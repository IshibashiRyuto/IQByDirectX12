///		@fn		GrpahicsCommandList.h
///		@brief	GrahpicsCommandList�̃��b�p�[�N���X
///		@author	Ishibashi Ryuto
///		@date
///		2018/09/18	���ō쐬
#pragma once
/*�V�X�e���w�b�_�C���N���[�h*/
#include <memory>
#include <string>
#include <d3d12.h>
#include <wrl.h>

/*�N���X�g�p�錾*/
class CommandAllocator;
class Device;
using Microsoft::WRL::ComPtr;

/// @brief �O���t�B�b�N�X�R�}���h���X�g�̃��b�p�[�N���X
/// @note	����A���X�g1�ɑ΂��ăA���P�[�^��1�����p�ӂ��Ă��Ȃ����߁A
class GraphicsCommandList
{
public:
	/// @brief �R���X�g���N�^
	GraphicsCommandList(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring & name);

	/// @brief �f�X�g���N�^
	~GraphicsCommandList();

	/// @brief		�I�u�W�F�N�g�̐�������
	/// @note		GraphicsCommandList�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// @param[in]	device			: �f�o�C�X
	/// @param[in]	commandListType : �R�}���h���X�g�^�C�v
	/// @param[in]	name			: �R�}���h���X�g�̖��O
	static std::shared_ptr<GraphicsCommandList> Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring& name = L"");

	/// @brief �R�}���h���X�g���擾����
	ComPtr<ID3D12GraphicsCommandList> GetCommandList();

	/// @brief �R�}���h���X�g�����Z�b�g����
	/// @note �v�R�}���h�N���[�Y & ���s & ���s�҂�
	void Reset();
	void Reset(ComPtr<ID3D12PipelineState> pipelineState);

private:
	/*�ϐ��錾*/
	ComPtr<ID3D12GraphicsCommandList> mCommandList;			// �R�}���h���X�g�̎��̂ւ̃|�C���^
	std::shared_ptr<CommandAllocator> mCommandAllocator;	// �R�}���h�A���P�[�^
};

