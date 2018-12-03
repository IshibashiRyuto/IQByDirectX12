/*
	@file	ConstantBuffer.h
	@brief	ConstantBuffer�̐錾���L�q����
	@author Ishibashi Ryuto
	@date	2018/07/31	���ō쐬
	@date	2018/12/03	CreateConstantBufferView�̕Ԃ�l��bool->void�ɕύX(�K���������邽�߈Ӗ����Ȃ�����)
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*�N���X�O���錾*/
using Microsoft::WRL::ComPtr;
class Device;

/**
*	@class	ConstantBuffer 
*	@brief	�萔�o�b�t�@�N���X
*
*	�V�F�[�_���\�[�X�Ƃ��ēn���萔�o�b�t�@�N���X
*	��̃o�b�t�@�̕ʗ̈���Q�Ƃ��ăf�[�^���󂯓n�����Ƃ��\
*/
class ConstantBuffer
{
public:
	/**
	*	@brief	�f�X�g���N�^
	*/
	~ConstantBuffer();

	/**
	*	@brief	�萔�o�b�t�@���쐬����
	*
	*	@param[in] device		: Device�N���X
	*	@param[in] bufferSize	: �o�b�t�@�̃T�C�Y
	*
	*	@retval ����������		: ConstantBuffer��shared_ptr
	*	@retval �������s��		: nullptr
	*
	*	@note ConstantBuffer�N���X�͂��̃N���X��p���Ă̂ݐ����\
	*/
	static std::shared_ptr<ConstantBuffer> Create(std::shared_ptr<Device> device, UINT elementSize, UINT elementCount = 1);

	/**
	*	@brief	�萔�o�b�t�@�Ƀf�[�^���Z�b�g����
	*
	*	@param[in] data �f�[�^�̃|�C���^
	*	@param[in] elementSize �f�[�^�T�C�Y
	*	@param[in] elementNum ���Ԗڂ̗v�f���X�V���邩
	*
	*	@retval �f�[�^�T�C�Y���o�b�t�@�T�C�Y���傫���ꍇ: true
	*	@retval �f�[�^�T�C�Y���o�b�t�@�T�C�Y���������ꍇ: false
	*/
	bool SetData(const void* data, UINT elementSize, UINT elementIndex = 0);

	/**
	*	@brief	�R���X�^���g�o�b�t�@�r���[���擾����
	*	@retval �R���X�^���g�o�b�t�@�r���[
	*/
	const D3D12_CONSTANT_BUFFER_VIEW_DESC & GetConstantBufferView(UINT elementIndex = 0);

private:
	ComPtr<ID3D12Resource> mBuffer;				//! �o�b�t�@����
	UINT mElementSize;							//! 1�v�f������̃o�b�t�@�T�C�Y
	UINT mElementCount;							//! �v�f��
	D3D12_CONSTANT_BUFFER_VIEW_DESC mCBVDesc;	//! �R���X�^���g�o�b�t�@�r���[
	char* mBufAddress;

	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	elementSize		: 1�v�f������̃f�[�^�T�C�Y
	*	@param[in]	elementCount	: �v�f��
	*/
	ConstantBuffer(UINT elementSize, UINT elementCount);

	/**
	*	@brief	�R���X�^���g�o�b�t�@�̎��̂��쐬����
	*	@param[in]	device		: ID3D12Device��ComPtr
	*	@param[in]	bufferSize	: �o�b�t�@�̃T�C�Y
	*	@retval �쐬������: true
	*	@retval �쐬���s��: false
	*/
	bool CreateConstantBuffer(const ComPtr<ID3D12Device> device);

	/**
	*	@brief	�R���X�^���g�o�b�t�@�r���[�̍쐬���s��
	*/
	void CreateConstantBufferView();
};

