/*
	@file	PipelineStateObject.h
	@brief	�p�C�v���C���X�e�[�g�I�u�W�F�N�g���Ǘ�����N���X
	@author Ishibashi Ryuto
	@dateo
	2018/10/09	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;

class PipelineStateObject
{
public:
	/// @brief	�R���X�g���N�^
	PipelineStateObject();
	/// @brief	�f�X�g���N�^
	~PipelineStateObject();

	/// @brief	�p�C�v���C���X�e�[�g�I�u�W�F�N�g���쐬����
	/// @param[in] pipelineStateDesc �p�C�v���C���X�e�[�g���
	/// @retval �������� : �p�C�v���C���X�e�[�g�I�u�W�F�N�g�̃X�}�[�g�|�C���^
	/// @retval �������s : nullptr
	static std::shared_ptr<PipelineStateObject> Create(std::shared_ptr<Device> device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC & pipelineStateDesc);

	/// @brief	�p�C�v���C���X�e�[�g�I�u�W�F�N�g���擾����
	ComPtr<ID3D12PipelineState> GetPipelineStateObject() const;

private:

	ComPtr<ID3D12PipelineState> mPipelineStateObject;

};

