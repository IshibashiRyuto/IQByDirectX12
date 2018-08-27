/*
	DescriptorHeap.h
	�f�B�X�N���v�^�q�[�v
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

class DescriptorHeap
{
public:
	/// �f�X�g���N�^
	~DescriptorHeap();

	/// @fn Create
	/// �f�B�X�N���v�^�q�[�v�̍쐬
	/// @note DescriptorHeap�͂��̃��\�b�h��ʂ��Ă̂ݍ쐬�\
	/// @param[in]	device		�f�o�C�X
	/// @param[in]	desc		�f�X�N���v�^�q�[�v�̐ݒ�
	/// @retval		��������	DescriptorHeap�̃|�C���^
	/// @retval		�������s	nullptr
	static std::shared_ptr<DescriptorHeap> Create(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

	/// @fn Create
	/// �f�B�X�N���v�^�q�[�v�̍쐬
	/// @note DescriptorHeap�͂��̃��\�b�h��ʂ��Ă̂ݍ쐬�\
	/// @param[in] device			�f�o�C�X
	/// @param[in] descriptorNum	�Z�b�g�\�ȃf�X�N���v�^�̌�
	static std::shared_ptr<DescriptorHeap> Create(ComPtr<ID3D12Device> device, UINT numDescriptors);

	/// @fn SetConstantBufferView
	/// �R���X�^���g�o�b�t�@�r���[���Z�b�g����
	/// @param[in] constantBufferView	�R���X�^���g�o�b�t�@�r���[
	/// @param[in] constantBuffer		�R���X�^���g�o�b�t�@
	/// @param[in] index				�q�[�v�̃C���f�b�N�X
	void SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferView, UINT index);

	/// @fn SetShaderResourceView
	/// �V�F�[�_���\�[�X�r���[���Z�b�g����
	/// @param[in]	shaderResourceView	�V�F�[�_���\�[�X�r���[
	/// @param[in]	shaderResource		�V�F�[�_���\�[�X
	/// @param[in] index				�q�[�v�̃C���f�b�N�X
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index);

	/// @fn SetTexture
	/// �e�N�X�`�������Z�b�g����
	/// @param[in]	texture			�e�N�X�`��
	/// @param[in]	index			�q�[�v�̃C���f�b�N�X
	void SetTexture(std::shared_ptr<Texture> texture, UINT index);

	/// @fn UnorderedAccessView
	/// �\�����o�b�t�@�r���[���Z�b�g����
	/// @param[in]	unorderedAccessView	�\�����o�b�t�@�r���[
	/// @param[in]	structuredBuffer	�\�����o�b�t�@
	/// @param[in] index				�q�[�v�̃C���f�b�N�X
	void SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index);

	/// @fn BindGraphicsCommandList
	/// �f�X�N���v�^�q�[�v���R�}���h���X�g�Ƀo�C���h����
	void BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList);

private:
	/*�萔�錾*/
	const UINT HEAP_STRIDE;

	/*�ϐ���`*/
	ComPtr<ID3D12Device>			mDevice;			//! �f�o�C�X
	ComPtr<ID3D12DescriptorHeap>	mDescriptorHeap;	//! �f�B�X�N���v�^�q�[�v�̎���
	UINT							mNumDescriptors;		//! �f�B�X�N���v�^�̐�

	/*���[�J�����\�b�h*/

	/// �R���X�g���N�^
	DescriptorHeap(ComPtr<ID3D12Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc);

};

