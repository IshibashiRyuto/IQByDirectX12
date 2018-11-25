/**
*	@file	Texture.h
*	@biref	�e�N�X�`���N���X�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/23	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"
#include <DirectXTex.h>

/* �N���X�g�p�錾 */
class DepthBuffer;
class RenderTargetBuffer;

class Texture :
	public Dx12Resource
{
public:
	/**
	*	@biref		�R���X�g���N�^
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	metaData	: �e�N�X�`�����^�f�[�^
	*	@param[in]	textureName	: �e�N�X�`����
	*	
	*	@param[out]	result		: ���\�[�X�̍쐬����
	*/
	Texture(std::shared_ptr<Device> device, const DirectX::TexMetadata& metaData, const std::wstring& textureName, HRESULT& result);

	/**
	*	@brief		�R���X�g���N�^
	*
	*	@param[in]	�\�z�ς݃��\�[�X�f�[�^
	*/
	Texture(ComPtr<ID3D12Resource> resource);

	/**
	*	@brief	�f�X�g���N�^
	*/
	~Texture();

	/**
	*	@brief	�e�N�X�`�����쐬����
	*
	*	pparam[in]	device		: dx12�f�o�C�X
	*	@param[in]	metaData	: �e�N�X�`�����^�f�[�^(DirectXTex)
	*	@param[in]	texImage	: �e�N�X�`���C���[�W�f�[�^(DirectXTex)
	*	@param[in]	textureName	: �e�N�X�`����
	*	@retval		��������	: Texture�̃X�}�[�g�|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<Device> device, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& texImage, const std::wstring& textureName);

	/**
	*	@brief	�e�N�X�`�����쐬����
	*
	*	@param[in]	depthBuffer	: �f�v�X�o�b�t�@
	*	@return		Texture�̃X�}�[�g�|�C���^
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<DepthBuffer> depthBuffer);

	/**
	*	@brief	�e�N�X�`�����쐬����
	*
	*	@param[in]	renderTargetBuffer	: �����_�[�^�[�Q�b�g�o�b�t�@
	*	@return		Texture�̃X�}�[�g�|�C���^
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<RenderTargetBuffer> renderTargetBuffer);

	/**
	*	@biref	�e�N�X�`��������������
	*
	*	@param[in]	metaData	: ���^�f�[�^
	*	@param[in]	texImage	: �e�N�X�`���C���[�W���
	*/
	void WriteTextureData(const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& texImage);

	/**
	*	@brief	SRV�����擾����
	*/
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceViewDesc() const;

protected:
	D3D12_SHADER_RESOURCE_VIEW_DESC mShaderResourceViewDesc;	//! �V�F�[�_���\�[�X�r���[

	/**
	*	@brief	SRV�����\�z����
	*	
	*	@note	���\�[�X�t�H�[�}�b�g���^�Ȃ��̏ꍇ�A������float�t�H�[�}�b�g�ɕϊ�����
	*/
	void ConstructSRVDesc();
};

