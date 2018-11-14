/*
	@file	DescriptorHeap.h
	@brief	DescriptorHeap�N���X���L�q����
	@author	Ishibashi Ryuto
	@date	2018/??/??	���ō쐬
*/
#pragma once
/*�V�X�e���w�b�_�C���N���[�h*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*����w�b�_�C���N���[�h*/

/*�O���錾*/
using Microsoft::WRL::ComPtr;
class Texture;
class RenderTargetTexture;
class Device;

class DescriptorHeap
{
public:
	/// @brief	�f�X�g���N�^
	~DescriptorHeap();

	/// @brief	�f�B�X�N���v�^�q�[�v�̍쐬
	/// @note DescriptorHeap�͂��̃��\�b�h��ʂ��Ă̂ݍ쐬�\
	/// @param[in]	device		�f�o�C�X
	/// @param[in]	desc		�f�X�N���v�^�q�[�v�̐ݒ�
	/// @retval		��������	DescriptorHeap�̃|�C���^
	/// @retval		�������s	nullptr
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

	/// @brief	�f�B�X�N���v�^�q�[�v�̍쐬
	/// @note DescriptorHeap�͂��̃��\�b�h��ʂ��Ă̂ݍ쐬�\
	/// @param[in] device			�f�o�C�X
	/// @param[in] descriptorNum	�Z�b�g�\�ȃf�X�N���v�^�̌�
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, UINT numDescriptors);

	/// @brief	�R���X�^���g�o�b�t�@�r���[���Z�b�g����
	/// @param[in] constantBufferView	�R���X�^���g�o�b�t�@�r���[
	/// @param[in] constantBuffer		�R���X�^���g�o�b�t�@
	/// @param[in] index				�q�[�v�̃C���f�b�N�X
	void SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferView, UINT index);

	/// @brief	�V�F�[�_���\�[�X�r���[���Z�b�g����
	/// @param[in]	shaderResourceView	�V�F�[�_���\�[�X�r���[
	/// @param[in]	shaderResource		�V�F�[�_���\�[�X
	/// @param[in] index				�q�[�v�̃C���f�b�N�X
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index);

	/// @brief	�e�N�X�`�������Z�b�g����
	/// @param[in]	texture			�e�N�X�`��
	/// @param[in]	index			�q�[�v�̃C���f�b�N�X
	void SetTexture(std::shared_ptr<Texture> texture, UINT index);

	/// @brief	�e�N�X�`�������Z�b�g����
	/// @param[in]	texture			�e�N�X�`��
	/// @param[in]	index			�q�[�v�̃C���f�b�N�X
	void SetTexture(std::shared_ptr<RenderTargetTexture> texture, UINT index);

	/// @brief	�\�����o�b�t�@�r���[���Z�b�g����
	/// @param[in]	unorderedAccessView	�\�����o�b�t�@�r���[
	/// @param[in]	structuredBuffer	�\�����o�b�t�@
	/// @param[in] index				�q�[�v�̃C���f�b�N�X
	void SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index);

	/// @brief	�f�X�N���v�^�q�[�v���R�}���h���X�g�Ƀo�C���h����
	void BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList);


	/// @brief	�f�B�X�N���v�^�e�[�u���ƃq�[�v���o�C���h����
	/// @param[in]	rootParamIndex	: ���[�g�p�����[�^�̃C���f�b�N�X
	/// @param[in]	descriptorHeapIndex	:	�q�[�v���̃C���f�b�N�X
	void BindRootDescriptorTable(int rootParamIndex, int descriptorHeapIndex);

private:
	/*�萔�錾*/
	const UINT HEAP_STRIDE;								//! �q�[�v�������̕�

	/*�ϐ���`*/
	std::shared_ptr<Device>			mDevice;			//! �f�o�C�X
	ComPtr<ID3D12DescriptorHeap>	mDescriptorHeap;	//! �f�B�X�N���v�^�q�[�v�̎���
	UINT							mNumDescriptors;		//! �f�B�X�N���v�^�̐�
	ComPtr<ID3D12GraphicsCommandList>	mGraphicsCommandList;	//!	�`��R�}���h���X�g

	/*���[�J�����\�b�h*/

	/// �R���X�g���N�^
	DescriptorHeap(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc);

};

