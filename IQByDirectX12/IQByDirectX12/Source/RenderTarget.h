/*
	RenderTarget.h
	�����_�[�^�[�Q�b�g���Ǘ�����N���X
	@author Ishibashi Ryuto
	@history
	2018/07/11 ���ō쐬
*/
#pragma once
/*�V�X�e���w�b�_�C���N���[�h*/
#include <vector>
#include <memory>
#include <string>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ComPtr�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;
class RenderTargetTexture;

class RenderTarget
{
public:
	/// �f�X�g���N�^
	~RenderTarget();

	/// @fn Create
	/// �����_�[�^�[�Q�b�g�𐶐����A�Ǘ�����RenderTarget�N���X��Ԃ�
	/// @note RenderTarget�N���X�͂��̃��\�b�h�ł̂ݐ����\
	/// @param[in] device:				ID3D12�f�o�C�X
	/// @param[in] swapChain:			IDXGI�X���b�v�`�F�C��
	/// @param[in] renderTargetsNum:	�����_�[�^�[�Q�b�g�̐�
	/// @retval ����������: RenderTarget�N���X��shared_ptr, �������s��: nullptr
	static std::shared_ptr<RenderTarget> Create(std::shared_ptr<Device> device, ComPtr<IDXGISwapChain1> swapChain, int renderTargetsNum);

	/// @fn ChangeRenderTarget
	/// �����_�[�^�[�Q�b�g��ύX����
	void ChangeRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList, int targetIndex);

	/// @fn FinishRendering
	/// �`�揈�����I������
	void FinishRendering(ComPtr<ID3D12GraphicsCommandList> commandList);

	/// @fn GetRTVHandle
	/// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾����
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();

	/// @fn ClearRenderTarget
	void ClearRenderTarget(ComPtr<ID3D12GraphicsCommandList> commandList);

	/// @brief RenderTargetTexture���擾����
	std::shared_ptr<RenderTargetTexture> GetRenderTargetTexture(int targetIndex);

protected:
	/* �萔��` */
	const UINT RENDER_TARGET_VIEW_DESCRIPTOR_SIZE;

	/* �ϐ���` */
	std::vector<std::shared_ptr<RenderTargetTexture>> mRenderTargets;
	ComPtr<ID3D12DescriptorHeap> mRTVDescHeap;
	int mRenderTargetIndex;

	/* ���[�J�����\�b�h */
	/// �R���X�g���N�^
	/// @param[in] device ID3D12�f�o�C�X
	RenderTarget(std::shared_ptr<Device> device);

};

