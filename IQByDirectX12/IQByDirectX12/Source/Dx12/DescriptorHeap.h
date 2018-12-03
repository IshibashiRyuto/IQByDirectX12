/*
	@file	DescriptorHeap.h
	@brief	DescriptorHeap�N���X�̐錾���L�q����
	@author	Ishibashi Ryuto
	@date	2018/08/23�ȑO(�ڍוs��)	���ō쐬
	@date	2018/11/24	DSV,RTV�p�q�[�v�̍쐬�ɑΉ�
						DSV,RTV�̃Z�b�g�ɑΉ�
	@date	2018/11/30	BindRootParameter, SetBindHeapIndex��ǉ�
						�����ă��[�g�p�����[�^�̃o�C���h���@��ύX
*/
#pragma once
/*�w�b�_�C���N���[�h*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>


/* �N���X�g�p�錾 */
using Microsoft::WRL::ComPtr;
class Texture;
class RenderTargetTexture;
class Device;
class GraphicsCommandList;

/**
*	@class	DescriptorHeap
*	@brief	�f�X�N���v�^�q�[�v�N���X
*
*	�V�F�[�_�Ƀ��\�[�X�̓ǂݎ����@�������邽�߂̏��ł���
*	Descriptor���i�[����DescriptorHeap�̃��b�p�[�N���X
*	Descriptor�̓f�[�^�̌�����View�Ƃ����`�ŋ�ۉ����Ă���A
*	����View��Heap�Ɋi�[���Ă����A���[�g�p�����[�^�Ƀo�C���h���ēǂݎ���Ă���
*	���̂��߁A�o�C���h��̃��[�g�p�����[�^�̃C���f�b�N�X�������Ƃ��Ď���
*/
class DescriptorHeap
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~DescriptorHeap();

	/**
	*	@brief	�f�B�X�N���v�^�q�[�v�̍쐬
	*
	*	@param[in]	device		: �f�o�C�X
	*	@param[in]	desc		: �f�X�N���v�^�q�[�v�̐ݒ�
	*
	*	@retval		��������	: DescriptorHeap�̃|�C���^
	*	@retval		�������s	: nullptr
	*
	*	@note DescriptorHeap�͂��̃��\�b�h��ʂ��Ă̂ݍ쐬�\
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& desc);

	/**
	*	@brief	�f�B�X�N���v�^�q�[�v�̍쐬
	*
	*	@param[in] device			: �f�o�C�X
	*	@param[in] descriptorNum	: �Z�b�g�\�ȃf�X�N���v�^�̌�
	*
	*	@retval		��������		: DescriptorHeap�̃|�C���^
	*	@retval		�������s		: nullptr
	*
	*	@note DescriptorHeap�͂��̃��\�b�h��ʂ��Ă̂ݍ쐬�\
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, UINT numDescriptors);

	/**
	*	@brief	�f�B�X�N���v�^�q�[�v�̍쐬
	*	
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	type			: �f�B�X�N���v�^�q�[�v�^�C�v
	*	@param[in]	descriptorsNum	: �Z�b�g�\�ȃf�X�N���v�^�̌�
	*	@param[in]	flag			: �q�[�v�t���O
	*
	*	@retval		��������		: DescriptorHeap�̃|�C���^
	*	@retval		�������s		: nullptr
	*/
	static std::shared_ptr<DescriptorHeap> Create(std::shared_ptr<Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, unsigned int descriptorsNum, D3D12_DESCRIPTOR_HEAP_FLAGS flag);

	/**
	*	@brief	�R���X�^���g�o�b�t�@�r���[���Z�b�g����
	*
	*	@param[in] constantBufferView	: �R���X�^���g�o�b�t�@�r���[
	*	@param[in] constantBuffer		: �R���X�^���g�o�b�t�@
	*	@param[in] index				: �q�[�v�̃C���f�b�N�X
	*/
	void SetConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& constantBufferView, UINT index);

	/**
	*	@brief	�V�F�[�_���\�[�X�r���[���Z�b�g����
	*
	*	@param[in]	shaderResourceView	: �V�F�[�_���\�[�X�r���[
	*	@param[in]	shaderResource		: �V�F�[�_���\�[�X
	*	@param[in] index				: �q�[�v�̃C���f�b�N�X
	*/
	void SetShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& shaderResourceView, ComPtr<ID3D12Resource> shaderResource, UINT index);

	/**
	*	@brief	�e�N�X�`�������Z�b�g����
	*
	*	@param[in]	texture		: �e�N�X�`��
	*	@param[in]	index		: �q�[�v�̃C���f�b�N�X
	*/
	void SetTexture(std::shared_ptr<Texture> texture, UINT index);

	/**
	*	@brief	�e�N�X�`�������Z�b�g����
	*
	*	@param[in]	texture		: �e�N�X�`��
	*	@param[in]	index		: �q�[�v�̃C���f�b�N�X
	*/
	void SetTexture(std::shared_ptr<RenderTargetTexture> texture, UINT index);

	/**
	*	@brief	�\�����o�b�t�@�r���[���Z�b�g����
	*
	*	@param[in]	unorderedAccessView	�\�����o�b�t�@�r���[
	*	@param[in]	structuredBuffer	�\�����o�b�t�@
	*	@param[in] index				�q�[�v�̃C���f�b�N�X
	*/
	void SetUnorderedAccessView(const D3D12_UNORDERED_ACCESS_VIEW_DESC& unorderedAccessView, ComPtr<ID3D12Resource> structuredBuffer, UINT index);

	/**
	*	@brief	�f�v�X�X�e���V���r���[���Z�b�g����
	*
	*	@param[in]	depthStencilViewDesc	: �f�v�X�X�e���V���r���[���
	*	@param[in]	depthBuffer				: �[�x�o�b�t�@
	*	@param[in]	index					: �Z�b�g��̃C���f�b�N�X
	*/
	void SetDepthStencilView(const D3D12_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc, ComPtr<ID3D12Resource> resource, UINT index);

	/**
	*	@brief	�f�X�N���v�^�q�[�v���R�}���h���X�g�Ƀo�C���h����
	*
	*	@param[in]	commandList	: �o�C���h�Ώۂ̃R�}���h���X�g
	*/
	void BindGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> commandList);

	/**
	*	@brief	�f�B�X�N���v�^�e�[�u���ƃq�[�v���o�C���h����
	*
	*	@param[in]	rootParamIndex		: ���[�g�p�����[�^�̃C���f�b�N�X
	*	@param[in]	descriptorHeapIndex	:	�q�[�v���̃C���f�b�N�X
	*/
	void BindRootDescriptorTable(unsigned int rootParamIndex, unsigned int descriptorHeapIndex);

	/**
	*	@brief	�q�[�v�����[�g�p�����[�^�Ƀo�C���h����
	*
	*	@param[in]	commandList		: �o�C���h����R�}���h���X�g
	*	@param[in]	rootParamIndex	: �o�C���h���郋�[�g�p�����[�^�C���f�b�N�X
	*
	*	@note		�o�C���h����q�[�v���̃C���f�b�N�X�͎��O�ɐݒ肵�Ă���
	*/
	void BindRootParameter(std::shared_ptr<GraphicsCommandList> commandList, unsigned int rootParamIndex);

	/**
	*	@brief	���[�g�p�����[�^�Ƀo�C���h����A�q�[�v�̃C���f�b�N�X���w�肷��
	*	
	*	@param[in]	heapIndex	: �o�C���h����q�[�v�ʒu
	*/
	void SetBindHeapIndex(unsigned int heapIndex);

	/**
	*	@brief	�w�肵���C���f�b�N�X��GPU�n���h�����擾����
	*
	*	@param[in]	index	: �擾���������n���h���̃C���f�b�N�X
	*
	*	@return	GPU�n���h��
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT index) const;

	/**
	*	@brief	�w�肵���C���f�b�N�X��CPU�n���h�����擾����
	*
	*	@param[in]	index	: �擾�������n���h���̃C���f�b�N�X
	*
	*	@return	CPU�n���h��
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT index) const;
private:
	/*�萔�錾*/
	const UINT HEAP_STRIDE;										//! �q�[�v�������̕�

	/*�ϐ���`*/
	std::shared_ptr<Device>			mDevice;					//! �f�o�C�X
	ComPtr<ID3D12DescriptorHeap>	mDescriptorHeap;			//! �f�B�X�N���v�^�q�[�v�̎���
	UINT							mNumDescriptors;			//! �f�B�X�N���v�^�̐�
	ComPtr<ID3D12GraphicsCommandList>	mGraphicsCommandList;	//!	�`��R�}���h���X�g
	unsigned int mHeapIndex;									//! �Z�b�g����q�[�v�̃C���f�b�N�X

	/*���[�J�����\�b�h*/

	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	heapDesc	: �q�[�v�\�z���
	*	@param[out]	result		: �q�[�v�̍\�z�ɐ���������
	*/
	DescriptorHeap(std::shared_ptr<Device> device, const D3D12_DESCRIPTOR_HEAP_DESC& heapDesc, HRESULT& result);

};

