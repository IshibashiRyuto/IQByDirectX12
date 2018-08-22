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
	/// �R���X�g���N�^
	ConstantBuffer();

	/// �f�X�g���N�^
	~ConstantBuffer();


	/// @fn Create
	/// �萔�o�b�t�@���쐬����
	/// @note ConstantBuffer�N���X�͂��̃N���X��p���Ă̂ݐ����\
	/// @param[in] device ID3D12Device��ComPtr
	/// @param[in] bufferSize	�o�b�t�@�̃T�C�Y
	/// @retval ����������: ConstantBuffer��shared_ptr
	/// @retval �������s��: nullptr
	static std::shared_ptr<ConstantBuffer> Create(const ComPtr<ID3D12Device> device, UINT bufferSize);


private:
	ComPtr<ID3D12Resource> mBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC mCBVDesc;
};

