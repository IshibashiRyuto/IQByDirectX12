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
class Device;
class TextureLoader;
class PipelineStateObject;
class RootSignature;

class ModelLoader
{
public:
	
	/// �f�X�g���N�^
	virtual ~ModelLoader();

	/// @fn LoadModel
	/// ���f�������[�h����
	/// @param[in] filePath	: �t�@�C���p�X
	virtual std::shared_ptr<Model> LoadModel(const std::string& filePath, std::shared_ptr<PipelineStateObject> pso,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature) = 0;

	/// @fn ClearModelData
	/// �ǂݍ��񂾃��f�������폜����
	virtual void ClearModelData() = 0;

protected:
	/* �ϐ��錾 */
	std::shared_ptr<Device> mDevice;			// Dx12�f�o�C�X
	ModelDataManager& mModelDataManager;	// ���f���f�[�^�}�l�[�W���ւ̎Q��
	std::map<std::string, int> mModelHandleManager;	// ���f���n���h���̊Ǘ�
	std::shared_ptr<TextureLoader> mTextureLoader;		// ���f���e�N�X�`�����[�_
													
	/// �R���X�g���N�^
	ModelLoader(std::shared_ptr<Device> device);
};

