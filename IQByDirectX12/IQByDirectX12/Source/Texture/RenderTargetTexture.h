/*
*	@file	RenderTargetTexture.h
*	@brief	RenderTargetTexture�N���X���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/12	���ō쐬
*/

#pragma once

/* �w�b�_�C���N���[�h */
#include "Texture.h"

/* �N���X�g�p�錾 */
class Device;
class GraphicsCommandList;

/*
	@class	RenderTargetTexture
	@brief	�����_�[�^�[�Q�b�g�Ƃ��Ďg�p�\�ȃe�N�X�`��
*/
class RenderTargetTexture :
	public Texture
{
public:
	///	@brief	�f�X�g���N�^
	~RenderTargetTexture();

	///	@brief �����_�[�^�[�Q�b�g�Ɏw��\�ȃe�N�X�`���𐶐�����
	///	@param[in]	device		: D3D12�f�o�C�X�N���X
	///	@param[in]	width		: �e�N�X�`���̕�
	///	@param[in]	height		: �e�N�X�`���̍���
	///	@param[in]	format		: �����_�[�^�[�Q�b�g�̃t�H�[�}�b�g(�f�t�H���g:DXGI_FORMAT_R8G8B8A8_UNORM)
	///	@retval		��������	: RenderTargetTexture��shared_ptr
	///	@retval		�������s	: nullptr
	static std::shared_ptr<RenderTargetTexture> Create(std::shared_ptr<Device> device, int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	///	@brief �����_�[�^�[�Q�b�g�Ɏw��\�ȃe�N�X�`���𐶐�����
	/// @param[in]	buffer		: �o�b�t�@
	///	@retval		��������	: RenderTargetTexture��shared_ptr
	///	@retval		�������s	: nullptr
	static std::shared_ptr<RenderTargetTexture> Create(ComPtr<ID3D12Resource> buffer);
	
	/// @brief	�����_�[�^�[�Q�b�g�r���[�����擾����
	///	@retval	�����_�[�^�[�Q�b�g�r���[���
	const D3D12_RENDER_TARGET_VIEW_DESC & GetRenderTargetViewDesc() const;

	///	@brief	�����_�[�^�[�Q�b�g�r���[�����Z�b�g����
	void SetRenderTargetViewDesc(const D3D12_RENDER_TARGET_VIEW_DESC &rtvDesc);

	/// @brief	�X�e�[�g��Ԃ�ύX���郊�\�[�X�o���A���擾����
	///	@param[in]	dstState	: �J�ڌ�̃X�e�[�g
	D3D12_RESOURCE_BARRIER GetTransitionBarrier(D3D12_RESOURCE_STATES dstState);

private:
	/* �ϐ��錾 */
	D3D12_RENDER_TARGET_VIEW_DESC mRTVDesc;			//! �����_�[�^�[�Q�b�g�r���[���
	D3D12_RESOURCE_STATES mCurrentResourceState;	//! ���݂̃��\�[�X�X�e�[�g

	///	@brief	�R���X�g���N�^
	RenderTargetTexture(ComPtr<ID3D12Resource> textureData);

	///	@brief	�o�b�t�@��񂩂烌���_�[�^�[�Q�b�g�r���[�����\�z����
	void ConstructRTVDesc();
};

