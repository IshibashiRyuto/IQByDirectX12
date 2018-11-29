/**
*	@file	IndexBuffer.h
*	@brief	IndexBuffer�N���X�̒�`���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/22	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"

/**
*	@class	IndexBuffer
*	@brief	���_�C���f�b�N�X�����i�[����
*/
class IndexBuffer :
	public Dx12Resource
{
public:

	/**
	*	@biref	�f�X�g���N�^
	*/
	~IndexBuffer();

	/**
	*	@biref	���_�C���f�b�N�X�o�b�t�@�̍쐬
	*	
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	pIndexResource	: �C���f�b�N�X���\�[�X�ւ̃|�C���^
	*	@param[in]	indexDataSize	: �C���f�b�N�X�f�[�^�̃T�C�Y(sizeof�Ŏ擾�����l��n��)
	*	@param[in]	indexCount		: �C���f�b�N�X�̐�
	*	@param[in]	bufferName		: �o�b�t�@��
	*
	*	@retval		��������	: IndexBuffer�̃X�}�[�g�|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<Device>device,
		const void* pVertexIndexes,
		size_t indexSize,
		size_t indexCount,
		const std::wstring& bufferName = L""
	);

	/**
	*	@biref	�C���f�b�N�X�o�b�t�@�r���[���擾����
	*
	*	@retval	�C���f�b�N�X�o�b�t�@�r���[
	*/
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const;

	/**
	*	@brief		�C���f�b�N�X�f�[�^����������
	*
	*	@param[in]	pIndexData		: �C���f�b�N�X�f�[�^�ւ̃|�C���^
	*	@param[in]	indexDataSize	: �C���f�b�N�X�f�[�^1������̃T�C�Y
	*	@param[in]	dataCount		: �������ރf�[�^�̐�
	*	@param[in]	offsetIndex		: �������ݐ�̃I�t�Z�b�g
	*
	*	�������ݐ�A�h���X�̐擪����(indexDataSize * offsetIndex)�����ꂽ�ꏊ��
	*	(pIndexData)����(pIndexData + indexDataSize * dataCount)�͈̔͂̃f�[�^����������
	*/
	void WriteIndexData(const void* pIndexData, size_t indexDataSize, size_t dataCount, unsigned int offsetIndex = 0);

	/**
	*	@brief	�C���f�b�N�X�����擾����
	*
	*	@return �C���f�b�N�X��
	*/
	unsigned int GetIndexCount() const;

private:
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;	//! ���_�C���f�b�N�X�o�b�t�@�r���[
	unsigned int mIndexCount;

	/**
	*	@brief	�C���f�b�N�X�o�b�t�@�r���[���\�z����
	*
	*	@param[in]	indexDataSize	: �C���f�b�N�X�f�[�^1������̃T�C�Y
	*	@param[in]	indexCount		: �C���f�b�N�X�̐�
	*	@param[out]	result			: �\�z����
	*/
	void ConstructIndexBufferView(size_t indexDataSize, size_t indexCount, HRESULT& result);

	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	indexDataSize	: �C���f�b�N�X�f�[�^�̃T�C�Y
	*	@param[in]	indexCount		: �C���f�b�N�X�̐�
	*	@param[in]	bufferName		: �o�b�t�@��
	*	@param[out]	result			: �o�b�t�@�̐�������
	*/
	IndexBuffer(
		std::shared_ptr<Device> device,
		size_t indexDataSize,
		size_t indexCount,
		const std::wstring& bufferName,
		HRESULT& result
	);
};

