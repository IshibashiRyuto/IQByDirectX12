/**
*	@file	InstanceDataBuffer.h
*	@brief	�C���X�^���V���O�p�f�[�^�o�b�t�@�N���X���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/21	���ō쐬
*/
#pragma once
/* �w�b�_�C���N���[�h */
#include "Dx12Resource.h"

/**
*	@class	InstanceDataBuffer
*	@brief	�C���X�^���V���O�p�f�[�^���i�[����N���X
*
*	�C���X�^���V���O�p�����i�[����N���X
*	�e���v���[�g���������������Ǘ���̖�肪�����������ߒf�O
*/
class InstanceDataBuffer :
	public Dx12Resource
{
public:
	/**
	*	@biref	�R���X�g���N�^
	*	@param[in]	device				: dx12�f�o�C�X
	*	@param[in]	instanceDataSize	: �C���X�^���X�f�[�^�������̃T�C�Y
	*	@param[in]	maxInstanceCount	: �ő�C���X�^���X��
	*	@param[in]	bufferName			: �o�b�t�@��
	*	@param[out]	result				: �o�b�t�@�̐�������
	*/
	InstanceDataBuffer(std::shared_ptr<Device> device,
		size_t instanceDataSize,
		size_t maxInstanceCount,
		const std::wstring& bufferName,
		HRESULT& result);

	/**
	*	@biref	�f�X�g���N�^
	*/
	~InstanceDataBuffer();

	/**
	*	@brief	�C���X�^���X�f�[�^�p�o�b�t�@���쐬����
	*
	*	@param[in]	device				: dx12�f�o�C�X
	*	@param[in]	instanceDataSize	: �C���X�^���X�f�[�^�̃T�C�Y
	*	@param[in]	maxInstanceCount	: �ő�C���X�^���X��
	*	@param[in]	bufferName			: �o�b�t�@��
	*/
	static std::shared_ptr<InstanceDataBuffer> Create(std::shared_ptr<Device> device,
		size_t instanceDataSize,
		size_t maxInstanceCount,
		const std::wstring& bufferName = L"");

	/**
	*	@brief	�C���X�^���X�f�[�^��o�^����
	*	
	*	�C���X�^���X�f�[�^�̓o�^���s���B
	*	�C���X�^���X�f�[�^�̃T�C�Y�́A�o�b�t�@�쐬���̃f�[�^�T�C�Y���Q�Ƃ���
	*	�܂��A�ő�C���X�^���X���𒴂����f�[�^�͔j�������
	*/
	void SetInstanceData(void* pInstanceData, int instanceDataIndex);


	/**
	*	@biref	���_�o�b�t�@�r���[���擾����
	*/
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const;
private:
	/* �ϐ��錾 */
	unsigned int mMaxInstanceCount;				//! �ő�C���X�^���X��
	void* m_pInstanceBuffer;					//! �C���X�^���X�o�b�t�@�̃|�C���^
	size_t mInstanceDataSize;					//! �C���X�^���X�̃f�[�^�T�C�Y
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;	//! ���_�o�b�t�@�r���[

	/**
	*	@brief	���_�o�b�t�@�r���[���\�z����
	*	@param[in]	dataSize	: �f�[�^�T�C�Y
	*	@param[in]	dataCount	: �f�[�^��
	*/
	void ConstructVertexBufferView(size_t dataSize, size_t dataCount);

	/**
	*	@biref	�o�b�t�@���}�b�v����
	*/
	void Map();
};

