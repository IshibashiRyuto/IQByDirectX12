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

// ����w�b�_�C���N���[�h

// �N���X�g�p�錾
class VertexBuffer;

// �\���̒�`
struct InstanceData
{
	int maxInstanceCount;
	int nowInstanceCount;
	size_t dataSize;
	std::shared_ptr<VertexBuffer> vertexBuffer;
};

class InstancingDataManager
{
public:
	// �R���X�g���N�^
	InstancingDataManager();

	// �f�X�g���N�^
	~InstancingDataManager();

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
	/// @param[in] maxInstanceCount	: �ő�C���X�^���X��
	/// @note �`�撆�ɌĂяo�����ꍇ�A�f�[�^���폜���邽�ߒ���
	void ResetMaxInstanceCount(int handle, int maxInstanceCount = 1);

	const InstanceData& GetInstanceData(int handle);

private:
	std::map<int, InstanceData> mInstanceDataMap;
};

