/**
*	@file	RenderTargetBuffer.h
*	@brief	RenderTargetBuffer�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/24	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"
#include "../Math/Math.h"

/**
*	@class	RenderTargetBuffer
*	@brief	�����_�[�^�[�Q�b�g�Ƃ��Ďg�p�\�ȃo�b�t�@���쐬����
*/
class RenderTargetBuffer :
	public Dx12Resource
{
public:
	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	width		: ����
	*	@param[in]	height		: �c��
	*	@param[in]	format		: �t�H�[�}�b�g
	*	@param[in]	clearValue	: �N���A�J���[
	*	@param[out] result		: �o�b�t�@�̐�������
	*/
	RenderTargetBuffer(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const std::wstring& bufferName,
		DXGI_FORMAT format,
		const D3D12_CLEAR_VALUE& clearValue,
		HRESULT & result);
	
	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	buffer	: �\�z�ς݃o�b�t�@
	*	@param[in]	state	: ���݂̃o�b�t�@�X�e�[�g
	*/
	RenderTargetBuffer(ComPtr<ID3D12Resource> buffer, D3D12_RESOURCE_STATES state, const D3D12_CLEAR_VALUE& clearValue);

	/**
	*	@brief	�f�X�g���N�^
	*/
	~RenderTargetBuffer();

	/**
	*	@brief	RenderTargetBuffer���쐬����
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	width		: �o�b�t�@����
	*	@param[in]	height		: �o�b�t�@�c��
	*	@param[in]	bufferName	: �o�b�t�@��
	*	@param[in]	format		: �t�H�[�}�b�g
	*	@param[in]	clearColor	: �o�b�t�@�N���A�J���[(�f�t�H���g�ō�)
	*
	*	@retval		��������	: RenderTargetBuffer�̃X�}�[�g�|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<RenderTargetBuffer> Create(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const std::wstring& bufferName = L"",
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		Math::Vector4 clearColor = Math::Vector4(0.f, 0.f, 0.f, 1.f));

	/**
	*	@brief	RenderTargetBuffer���쐬����
	*
	*	@param[in]	buffer	: �\�z�ς݃o�b�t�@
	*	@param[in]	state	: ���݂̃o�b�t�@�X�e�[�g
	*/
	static std::shared_ptr<RenderTargetBuffer> Create(ComPtr<ID3D12Resource> buffer, D3D12_RESOURCE_STATES state);

	/**
	*	@brief	RTV�����擾����
	*/
	const D3D12_RENDER_TARGET_VIEW_DESC & GetRenderTargetViewDesc() const;


	/**
	*	@brief	�N���A�l���擾����
	*	
	*	@return	�����_�[�^�[�Q�b�g�̃N���A�l
	*/
	const D3D12_CLEAR_VALUE& GetClearValue() const;

private:
	D3D12_RENDER_TARGET_VIEW_DESC mRTVDesc;		//! �����_�[�^�[�Q�b�g�r���[���
	const D3D12_CLEAR_VALUE CLEAR_VALUE;		//! �N���A�l

	/**
	*	@brief	�����_�[�^�[�Q�b�g�r���[�����\�z����
	*/
	void ConstructRTVDesc();
};

