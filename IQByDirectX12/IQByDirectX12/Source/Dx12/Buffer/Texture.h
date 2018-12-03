/**
*	@file	Texture.h
*	@biref	�e�N�X�`���N���X�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/23	���ō쐬
*			2018/11/27	�e�N�X�`�����̏������݂��ꎞ���\�[�X�ɍs���A��Ŗ{���\�[�X�փA�b�v�f�[�g������@�֕ύX
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"
#include <DirectXTex.h>
#include "../Math/Math.h"

/* �N���X�g�p�錾 */
class DepthBuffer;
class RenderTargetBuffer;

/**
*	@class	Texture
*	@brief	���\�[�X���e�N�X�`���Ƃ��Ĉ����N���X
*/
class Texture :
	public Dx12Resource
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~Texture();

	/**
	*	@brief	�e�N�X�`�����쐬����
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	metaData	: �e�N�X�`�����^�f�[�^(DirectXTex)
	*	@param[in]	texImage	: �e�N�X�`���C���[�W�f�[�^(DirectXTex)
	*	@param[in]	textureName	: �e�N�X�`����
	*	@retval		��������	: Texture�̃X�}�[�g�|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<Device> device, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& texImage, const std::wstring& textureName);

	/**
	*	@brief	��̃e�N�X�`�����쐬����
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	width		: �摜����
	*	@param[in]	height		: �摜�c��
	*	@param[in]	format		; �摜�t�H�[�}�b�g
	*	@param[in]	textureName	: �e�N�X�`����
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<Device> device, unsigned int width, unsigned int height, DXGI_FORMAT format, const std::wstring& textureName = L"");

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
	*
	*	@note		�ꎞ�e�N�X�`���ɏ����������ނ����ł��邽�߁A
	*				���̃e�N�X�`�����͕ύX����Ȃ�
	*				�ύX�̂��߂ɂ�UpdateTexture���\�b�h���Ăяo���K�v�����邽�ߒ���
	*/
	void WriteTextureData(const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& texImage);

	/**
	*	@brief	�e�N�X�`��������������
	*
	*	@param[in]	pImage			: �C���[�W�f�[�^�ւ̃|�C���^
	*	@param[in]	width			: �摜��
	*	@param[in]	height			: �摜����
	*	@param[in]	pixelDataSize	: 1�s�N�Z��������̃f�[�^�T�C�Y
	*
	*	@note		�ꎞ�e�N�X�`���ɏ����������ނ����ł��邽�߁A
	*				���̃e�N�X�`�����͕ύX����Ȃ�
	*				�ύX�̂��߂ɂ�UpdateTexture���\�b�h���Ăяo���K�v�����邽�ߒ���
	*/
	void WriteTextureData(void* pImage, unsigned int width, unsigned int height, size_t pixelDataSize);

	/**
	*	@brief	SRV�����擾����
	*/
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceViewDesc() const;

	/**
	*	@brief	�e�N�X�`�������X�V����
	*
	*	@param[in]	�R�s�[���\�b�h���ڂ���R�}���h���X�g
	*/
	void UpdateTexture(std::shared_ptr<GraphicsCommandList> commandList);

protected:
	D3D12_SHADER_RESOURCE_VIEW_DESC mShaderResourceViewDesc;	//! �V�F�[�_���\�[�X�r���[
	std::shared_ptr<Dx12Resource> mTemporaryResource;			//! �f�[�^�]���p�̈ꎞ���\�[�X
	bool mIsUpdate;												//! �e�N�X�`�����̍X�V����������
	const Math::Vector2 TEXTURE_SIZE;							//! �e�N�X�`���T�C�Y���i�[�����{�b�N�X�f�[�^
	unsigned int mRowPitch;										//! �������݃f�[�^��rowPitch

	/**
	*	@brief	SRV�����\�z����
	*	
	*	@note	���\�[�X�t�H�[�}�b�g���^�Ȃ��̏ꍇ�A������float�t�H�[�}�b�g�ɕϊ�����
	*/
	void ConstructSRVDesc();


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
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	width		: �摜��
	*	@param[in]	height		; �摜����
	*	@param[in]	format		: �摜�f�[�^�t�H�[�}�b�g
	*	@param[in]	textureName	: �e�N�X�`����
	*
	*	@param[out]	result		: ���\�[�X�̍쐬����
	*	
	*	@note	�~�b�v�}�b�v�e�N�X�`���̍쐬�͂ł��Ȃ����ߒ���
	*/
	Texture(std::shared_ptr<Device> device, unsigned int width, unsigned int height, DXGI_FORMAT format, const std::wstring& textureName, HRESULT& result);

	/**
	*	@brief		�R���X�g���N�^
	*
	*	@param[in]	resource	: �\�z�ς݃��\�[�X�f�[�^
	*/
	Texture(std::shared_ptr<Dx12Resource> resource);

};

