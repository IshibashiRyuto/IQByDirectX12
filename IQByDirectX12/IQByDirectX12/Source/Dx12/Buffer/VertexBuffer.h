/**
*	@file	VertexBuffer_Def.h
*	@brief	���_�o�b�t�@�N���X�̒�`���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/21	���ō쐬
*/

#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"
#include <vector>

/**
*	@class	VertexBuffer
*	@brief	���_�o�b�t�@�N���X
*
*	���_�o�b�t�@�����i�[����N���X
*	�e���v���[�g�ɂ������������A���̉����Ȃ����
*	���̃e���v���[�g�N���X�Ɋi�[�ł��Ȃ��č����ʂ��������̂�
*	�f�[�^�̊i�[�̓��[�U�Ǘ��ɔC����悤�ɂ��܂�	
*/
class VertexBuffer :
	public Dx12Resource
{
public:

	/**
	*	@brief	�f�X�g���N�^
	*/
	virtual ~VertexBuffer();

	/**
	*	@brief	���_�o�b�t�@�𐶐�����
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	pVertices	: ���_���
	*	@param[in]	vertexSize	: 1���_������̃T�C�Y(sizeof�Ŏ擾)
	*	@param[in]	vertexCount	: ���_��
	*	@param[in]	bufferName	: �o�b�t�@��
	*
	*	@retval		��������	: VertexBuffer�̃X�}�[�g�|�C���^
	*	@retval		�������s	: nullptr
	*/
	static std::shared_ptr<VertexBuffer> Create
	(
		std::shared_ptr<Device> device,
		const void* pVertices,
		size_t vertexSize,
		size_t vertexCount,
		const std::wstring& bufferName = L""
	);

	/**
	*	@brief	���_�o�b�t�@�r���[���擾����
	*
	*	@return	���_�o�b�t�@�r���[�ւ̎Q��
	*/
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const;

	/**
	*	@brief		�o�b�t�@�ɏ�����������
	*
	*	@param[in]	data			: �������ރf�[�^
	*	@param[in]	dataSize		: �������ރf�[�^�̃T�C�Y
	*	@param[in]	dataCount		: �������ރf�[�^�̐�
	*	@param[in]	dataIndexOffset	: �������ݐ�̃I�t�Z�b�g�C���f�b�N�X(dataSize�Q��)
	*
	*	�o�b�t�@�̐擪����(dataSize * dataIndexOffset)���i�񂾈ʒu��
	*	data��dataSize����������
	*/
	void WriteVertexBuffer(const void* data, size_t dataSize, size_t dataCount, unsigned int dataIndexOffset = 0);
protected:
	/* �ϐ��錾 */
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;	//! ���_�o�b�t�@�r���[

	/**
	*	@brief	���_�o�b�t�@�r���[���\�z����
	*
	*	@param[in]	dataSize	: �f�[�^�T�C�Y
	*	@param[in]	dataCount	: �f�[�^��
	*/
	void ConstructVertexBufferView(size_t dataSize, size_t dataCount);


	/**
	*	@brief	�R���X�g���N�^
	*
	*	@param[in]	device		: �f�o�C�X
	*	@param[in]	dataSize	: �f�[�^�T�C�Y
	*	@param[in]	dataCount	: �f�[�^��
	*	@param[in]	bufferName	: �o�b�t�@��
	*	@param[out]	result		: �o�b�t�@�̍쐬����
	*/
	VertexBuffer(std::shared_ptr<Device> device,
		size_t dataSize,
		size_t dataCount,
		const std::wstring& bufferName,
		HRESULT& result);
};

