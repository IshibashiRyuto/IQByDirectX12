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
class RenderTargetBuffer;
class Texture;
class GraphicsCommandList;

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
	/// @param[in] windowWidth;			�E�B���h�E��
	/// @param[in] windowHeight;		�E�B���h�E����
	/// @param[in] renderTargetsNum:	�����_�[�^�[�Q�b�g�̐�
	/// @retval ����������: RenderTarget�N���X��shared_ptr, �������s��: nullptr
	static std::shared_ptr<RenderTarget> Create(std::shared_ptr<Device> device, ComPtr<IDXGISwapChain1> swapChain, int renderTargetsNum);

	/// @fn ChangeRenderTarget
	/// �����_�[�^�[�Q�b�g��ύX����
	void ChangeRenderTarget(std::shared_ptr<GraphicsCommandList> commandList, int targetIndex);

	/// @fn FinishRendering
	/// �`�揈�����I������
	void FinishRendering(std::shared_ptr<GraphicsCommandList> commandList);

	/// @fn GetRTVHandle
	/// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾����
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();

	/// @fn ClearRenderTarget
	void ClearRenderTarget(std::shared_ptr<GraphicsCommandList> commandList);

	/// @brief RenderTarget�o�b�t�@���擾����
	std::shared_ptr<RenderTargetBuffer> GetRenderTargetBuffer(int targetIndex) const;

	/// @brief �����_�[�e�N�X�`�����擾����
	std::shared_ptr<Texture> GetRenderTexture(int targetIndex) const;

protected:
	/* �萔��` */
	const UINT RENDER_TARGET_VIEW_DESCRIPTOR_SIZE;

	/* �ϐ���` */
	std::vector<std::shared_ptr<RenderTargetBuffer>> mRenderTargets;
	std::vector<std::shared_ptr<Texture>> mRenderTextures;
	ComPtr<ID3D12DescriptorHeap> mRTVDescHeap;
	int mRenderTargetIndex;

	/* ���[�J�����\�b�h */
	/// �R���X�g���N�^
	/// @param[in] device ID3D12�f�o�C�X
	RenderTarget(std::shared_ptr<Device> device);

};

