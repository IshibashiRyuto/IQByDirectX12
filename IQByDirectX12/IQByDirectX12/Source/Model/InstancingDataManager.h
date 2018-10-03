/*
	InstansingDataManager.h
	�C���X�^���V���O�ɕK�v�ȏ����Ǘ�����
	@author Ishibashi Ryuto
	@date
	2018/09/03 ���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <map>
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// ����w�b�_�C���N���[�h

// �N���X�g�p�錾
class InstanceBuffer;
using Microsoft::WRL::ComPtr;

// �\���̒�`
struct InstanceData
{
	int maxInstanceCount;
	int nowInstanceCount;
	size_t dataSize;
	std::shared_ptr<InstanceBuffer> instanceBuffer;
};

class InstancingDataManager
{
public:
	// �f�X�g���N�^
	~InstancingDataManager();

	/// @fn GetInstance
	/// �V���O���g���C���X�^���X���擾����
	/// @retval const InstancingDataManager& : InstancingDataManager�N���X�̎Q��
	static InstancingDataManager& GetInstance()
	{
		static InstancingDataManager inst;
		return inst;
	}

	/// @fn SetDevice
	/// �f�o�C�X���Z�b�g����
	/// @param[in] device	: ID3D12�f�o�C�X
	void SetDevice(ComPtr<ID3D12Device> device);

	/// @fn SetInstanceData
	/// �C���X�^���V���O�p�̃f�[�^���Z�b�g����
	/// @param[in] handle	: �C���X�^���V���O�Ώۂ̃n���h��
	/// @param[in] data		: �f�[�^�̃|�C���^
	/// @param[in] size		: �f�[�^�̃T�C�Y
	/// @note �C���X�^���V���O�o�^���Ă��Ȃ��ꍇ�A�f�[�^�̓o�^���s��Ȃ��B
	void SetInstanceData(int handle, void* data, size_t size);

	/// @fn ResetMaxInstanceCount
	/// �w�肵���n���h���̍ő�C���X�^���X�����C������
	/// @param[in] handle			: �C���Ώۂ̃n���h��
	/// @param[in] instanceDataSize : �C���X�^���X�f�[�^1������̃T�C�Y
	/// @param[in] maxInstanceCount	: �ő�C���X�^���X��
	/// @note �`�撆�ɌĂяo�����ꍇ�A�f�[�^���폜���邽�ߒ���
	void ResetMaxInstanceCount(int handle, size_t instanceDataSize, int maxInstanceCount = 1);

	/// @fn ResetInstanceData
	/// �C���X�^���X�f�[�^�����Z�b�g����
	/// @param[in] handle	: ���Z�b�g�Ώۂ̃n���h��
	void ResetInstanceData(int handle);

	/// @fn ResetInstanceDataAll
	/// �o�^����Ă���C���X�^���X�f�[�^���ׂĂ����Z�b�g����
	void ResetInstanceDataAll();

	/// @fn GetInstanceDataAll
	/// �o�^����Ă���C���X�^���X�f�[�^���ׂĂ��擾����
	/// @retval �C���X�^���X�f�[�^�ƃn���h���̃}�b�v
	const std::map<int, InstanceData>& GetInstanceDataAll();

private:
	/* �����o�ϐ���` */
	std::map<int, InstanceData> mInstanceDataMap;		// �C���X�^���V���O�f�[�^�Ǘ��p�}�b�v
	ComPtr<ID3D12Device> mDevice;						// ID3D12�f�o�C�X

	/* ���[�J�����\�b�h��` */
	// �R���X�g���N�^
	InstancingDataManager();
};

