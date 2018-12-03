/**
*	@file	DepthBuffer.h
*	@brief	DepthBuffer�̐錾���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/24	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"

/* �N���X�g�p�錾 */
class DescriptorHeap;
class GraphicsCommandList;

/**
*	@class	DepthBuffer
*	@brief	�[�x�o�b�t�@�N���X
*	
*	�[�x�o�b�t�@���i�[����N���X
*	1�o�b�t�@1�q�[�v�̎d�g�݂��̗p���Ă���
*	���̂��߁A�����Ƀo�b�t�@���\�[�X�̂ق��A�q�[�v��ێ����Ă���
*	(�����݌v�v��������ς��܂�)
*	
*	@note	���ō쐬���A�X�e���V���o�b�t�@�̎g�p��z�肵�Ă��Ȃ����ߒ���
*/
class DepthBuffer :
	public Dx12Resource
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~DepthBuffer();

	/**
	*	@brief	�f�v�X�o�b�t�@���쐬����
	*
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	width			: �o�b�t�@����
	*	@param[in]	height			: �o�b�t�@�c��
	*	@param[in]	isAlignment		: �A���C�������g���s����(�f�t�H���g��true)
	*	@param[in]	depthClearValue	: �[�x�N���A�l(�f�t�H���g��1.0f)
	*
	*	@retval	��������		: DepthBuffer�̃X�}�[�g�|�C���^
	*	@retval	�������s		: nullptr
	*/
	static std::shared_ptr<DepthBuffer> Create(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const std::wstring& bufferName = L"DepthBuffer",
		bool isAlignment = true,
		float depthClearValue = 1.0f);

	/**
	*	@brief	�[�x�o�b�t�@���N���A����
	*	@param[in]	commandList	: �N���A�R�}���h���ڂ���R�}���h���X�g
	*/
	void ClearDepthBuffer(std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@brief	�[�x�o�b�t�@�ւ̏������݂��J�n����
	*
	*	@note	�������݊J�n���ɏ��������s��
	*			�܂��A�R�}���h���X�g�ւ̃Z�b�g�͍s��Ȃ�
	*
	*	@param[in]	commandList	: �������ݑΏۂ̃R�}���h���X�g
	*/
	void BeginWriteDepth(std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@brief	�[�x�o�b�t�@�ւ̏������݂��I������
	*	
	*	@param[in]	commandList	: �������ݑΏۂ̃R�}���h���X�g
	*/
	void EndWriteDepth(std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@biref	�f�v�X�X�e���V���r���[�����擾����
	*	@return	�f�v�X�X�e���V���r���[���
	*/	
	const D3D12_DEPTH_STENCIL_VIEW_DESC& GetDSVDesc() const;

	/**
	*	@brief	�f�v�X�X�e���V���r���[��CPU�A�h���X���擾����
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUHandle() const;
private:
	const D3D12_CLEAR_VALUE CLEAR_VALUE;

	std::shared_ptr<DescriptorHeap> mDepthDescHeap;	//! �[�x�o�b�t�@�p�f�X�N���v�^�q�[�v
	D3D12_DEPTH_STENCIL_VIEW_DESC mDSVDesc;			//! �f�v�X�X�e���V���r���[���

	/**
	*	@brief	2�̏搔�ɃA���C�������g�������l��Ԃ�
	*	@param[in]	�A���C�������g���������l�f�[�^
	*	@return		�A���C�������g���ꂽ���l�f�[�^
	*/
	static unsigned int Alignment(unsigned int num);

	/**
	*	@brief	DSV���\�z����
	*	@param[in]	device	: dx12�f�o�C�X
	*/
	void ConstructDSV(std::shared_ptr<Device> device);

	/**
	*	@brief	�R���X�g���N�^
	*/
	DepthBuffer(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const D3D12_CLEAR_VALUE& clearValue,
		const std::wstring& bufferName,
		HRESULT& result
	);

};

