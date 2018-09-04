/*
	ModelLoader
	���f�����[�_�[�̃x�[�X
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include <functional>
#include <map>

// ����w�b�_�C���N���[�h
#include "ModelDataManager.h"
#include "Model.h"

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class ModelData;

class ModelLoader
{
public:
	/// �R���X�g���N�^
	ModelLoader(ComPtr<ID3D12Device> mDevice);
	
	/// �f�X�g���N�^
	virtual ~ModelLoader();

	/// @fn LoadModel
	/// ���f�������[�h����
	/// @param[in] filePath	: �t�@�C���p�X
	virtual std::shared_ptr<Model> LoadModel(const std::string& filePath) = 0;

	/// @fn ClearModelData
	/// �ǂݍ��񂾃��f�������폜����
	virtual void ClearModelData() = 0;

protected:
	ComPtr<ID3D12Device> mDevice;			// Dx12�f�o�C�X
	ModelDataManager& mModelDataManager;	// ���f���f�[�^�}�l�[�W���ւ̎Q��
	std::map<std::string, int> mModelHandleManager;	// ���f���n���h���̊Ǘ�
};

