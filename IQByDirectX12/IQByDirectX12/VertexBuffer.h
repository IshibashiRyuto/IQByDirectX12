/*
	VertexBuffer.h
	���_�o�b�t�@�N���X
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class VertexBuffer
{
public:
	/// �f�X�g���N�^
	~VertexBuffer();

	/// @fn Create
	/// ���_�o�b�t�@�̐���
	/// @note VertexBuffer�N���X�͂��̃��\�b�h�ɂ���Ă̂ݐ����\
	/// @param[in] device: ID3D12Device
	/// @param[in] pVertexResource: ���_���\�[�X�̃|�C���^
	/// @param[in] vertexCount: ���_��
	/// @param[in] vertexSize: 1���_������̃T�C�Y(sizeof�Ŏ擾)
	/// @retval ��������: VertexBuffer��shared_ptr, �������s: nullptr
	static std::shared_ptr<VertexBuffer> Create(ComPtr<ID3D12Device> device, void* pVertexResource, size_t vertexCount, size_t vertexSize);

	/// @fn GetVertexBufferView
	/// ���_�o�b�t�@�r���[�̎擾
	/// @retval D3D12_VERTEX_BUFFER_VIEW ���_�o�b�t�@�r���[�ւ̎Q��
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView();

private:
	/// �R���X�g���N�^
	VertexBuffer();
	VertexBuffer(const VertexBuffer&);

	ComPtr<ID3D12Resource> mVertexBuffer;			// ���_�o�b�t�@���\�[�X
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;		// ���_�o�b�t�@�r���[
};