/*
	InstanceBuffer.h
	�C���X�^���X���i�[�p�̃o�b�t�@
	@author Ishibashi Ryuto
	@date
	2018/09/04	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// ����w�b�_�C���N���[�h

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;

class InstanceBuffer
{
public:
	// �R���X�g���N�^
	InstanceBuffer();

	// �f�X�g���N�^
	~InstanceBuffer();

	/// @fn Create
	/// �C���X�^���X�o�b�t�@���쐬����
	/// @note InstanceBuffer�N���X�͂��̃��\�b�h��ʂ��Ă̂ݐ����\
	/// @param[in] device : ID3D12�f�o�C�X
	/// @param[in] instanceDataSize	: �C���X�^���X�f�[�^�̃T�C�Y
	/// @param[in] maxInstanceCount	: �ő�C���X�^���X��
	static std::shared_ptr<InstanceBuffer> Create(std::shared_ptr<Device> device, size_t instanceDataSize, int maxInstanceCount);

	/// @fn GetVertexBufferView
	/// ���_�o�b�t�@�r���[�̎擾
	/// @retvval D3D12_VERTEX_BUFFER_VIEW ���_�o�b�t�@�r���[�ւ̎Q��
	const D3D12_VERTEX_BUFFER_VIEW & GetVertexBufferView() const;

	/// @fn SetInstanceData
	/// �C���X�^���X�f�[�^���Z�b�g����
	/// @param[in] pInstanceData		: �C���X�^���X�f�[�^�ւ̃|�C���^
	/// @param[in] instanceDataIndex	: ���Ԗڂ̃C���X�^���X�f�[�^���Z�b�g���邩
	/// @note	�C���X�^���X�f�[�^�̃T�C�Y�̓C���X�^���X�o�b�t�@�������̂��̂��Q��
	///			���Ƃ���ύX���邱�Ƃ͕s�\
	void SetInstanceData(void* pInstanceData, int instanceDataIndex);

private:
	int mMaxInstanceCount;							// �ő�C���X�^���X��
	ComPtr<ID3D12Resource> mInstanceBuffer;			// �C���X�^���X�o�b�t�@���\�[�X
	int mInstanceDataSize;							// �C���X�^���X�f�[�^�̃T�C�Y
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;		// ���_�o�b�t�@�r���[
	void* pInstanceBuffer;							// �C���X�^���X�o�b�t�@�̃|�C���^
};

