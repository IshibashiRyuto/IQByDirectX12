/*
	ModelDataManager.h
	���f���f�[�^�Ǘ��N���X
	@author Ishibashi Ryuto
	@date
	2018/09/03	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <map>

// ����w�b�_�C���N���[�h
#include "ModelData.h"

class ModelDataManager
{
public:
	~ModelDataManager();

	/// @fn static GetInstance
	/// �V���O���g���C���X�^���X���擾����
	/// @retval ���f���}�l�[�W���̃C���X�^���X
	static ModelDataManager& GetInstance()
	{
		static ModelDataManager inst;
		return inst;
	}

	/// @fn Regist
	/// ���f���f�[�^��o�^���A�Ǘ��n���h����Ԃ�
	/// @param[in] modelData ���f���f�[�^
	/// @retval		���f���f�[�^�̃n���h��
	int Regist(std::shared_ptr<ModelData> modelData);

	/// @fn Erase
	/// �w�肵���n���h���̃��f���f�[�^���폜����
	/// @param[in] handle ���f���n���h��
	void Erase(int handle);
	
	/// @fn GetModelData
	/// ���f���f�[�^���쐬����
	/// @param[in] handle	: ���f���n���h��
	/// @retval ���f���f�[�^
	std::shared_ptr<ModelData> GetModelData(int handle) const;

	/// @fn IsExist
	/// �w�肵���n���h�����w�����f���f�[�^�����݂��邪�m�F����
	/// @param[in] handle : �e�N�X�`���n���h��
	/// @retval ���݂���: true
	/// @retval ���݂��Ȃ�: false
	bool IsExist(int handle) const;

private:
	ModelDataManager();
	ModelDataManager(const ModelDataManager&) {}
	void operator=(const ModelDataManager &){}

	/* �萔��` */
	const int DATA_SIGNATURE_SHIFT_NUM = 24;
	const int MODEL_DATA_SIGNATURE = 0x11 << DATA_SIGNATURE_SHIFT_NUM;

	/* �ϐ��錾 */
	std::map<int, std::shared_ptr<ModelData>> mData;		// �Ǘ��f�[�^
	int mNextHandle;									// ���Ɏg�p����e�N�X�`���n���h��


	/* ���[�J�����\�b�h�錾 */
	/// @fn ���Ɏg�p����n���h����T�����A�X�V����
	void UpdateNextHandle();
};

