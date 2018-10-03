/*
	ConstantBuffer.h
	�萔�o�b�t�@�N���X
	@author Ishibashi Ryuto
	@param history
	2017/07/31	���ō쐬
*/
#pragma once
/*�V�X�e���w�b�_�C���N���[�h*/
#include <d3d12.h>
#include <wrl.h>
#include <memory>

/*����w�b�_�C���N���[�h*/

/*�N���X�O���錾*/
using Microsoft::WRL::ComPtr;

class ConstantBuffer
{
public:
	/// �f�X�g���N�^
	~ConstantBuffer();


	/// @fn Create
	/// �萔�o�b�t�@���쐬����
	/// @note ConstantBuffer�N���X�͂��̃N���X��p���Ă̂ݐ����\
	/// @param[in] device ID3D12Device��ComPtr
	/// @param[in] bufferSize	�o�b�t�@�̃T�C�Y
	/// @retval ����������: ConstantBuffer��shared_ptr
	/// @retval �������s��: nullptr
	static std::shared_ptr<ConstantBuffer> Create(const ComPtr<ID3D12Device> device, UINT elementSize, UINT elementCount = 1);

	/// @fn SetData
	/// �萔�o�b�t�@�Ƀf�[�^���Z�b�g����
	/// @param[in] data �f�[�^�̃|�C���^
	/// @param[in] elementSize �f�[�^�T�C�Y
	/// @param[in] elementNum ���Ԗڂ̗v�f���X�V���邩
	/// @retval �f�[�^�T�C�Y���o�b�t�@�T�C�Y���傫���ꍇ: true
	/// @retval �f�[�^�T�C�Y���o�b�t�@�T�C�Y���������ꍇ: false
	bool SetData(const void* data, UINT elementSize, UINT elementIndex = 0);

	/// @fn GetConstantBufferView
	/// �R���X�^���g�o�b�t�@�r���[���擾����
	/// @retval �R���X�^���g�o�b�t�@�r���[
	const D3D12_CONSTANT_BUFFER_VIEW_DESC & GetConstantBufferView(UINT elementIndex = 0);

private:
	ComPtr<ID3D12Resource> mBuffer;				// �o�b�t�@
	UINT mElementSize;							// 1�v�f������̃o�b�t�@�T�C�Y
	UINT mElementCount;							// �v�f��
	D3D12_CONSTANT_BUFFER_VIEW_DESC mCBVDesc;	// �R���X�^���g�o�b�t�@�r���[
	char* mBufAddress;

	/// �R���X�g���N�^
	ConstantBuffer(UINT elementSize, UINT elementCount);

	/// @fn CreateConstantBuffer
	/// �R���X�^���g�o�b�t�@�̎��̂��쐬����
	/// @param[in] device ID3D12Device��ComPtr
	/// @param[in] bufferSize	�o�b�t�@�̃T�C�Y
	/// @retval �쐬������: true
	/// @retval �쐬���s��: false
	bool CreateConstantBuffer(const ComPtr<ID3D12Device> device);

	/// @fn CreateConstantBufferView
	/// �R���X�^���g�o�b�t�@�r���[�̍쐬���s��
	/// @retval ����������:	true
	/// @retval �������s��: false
	bool CreateConstantBufferView();
};

