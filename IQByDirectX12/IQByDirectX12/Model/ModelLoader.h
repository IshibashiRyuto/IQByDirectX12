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

// ����w�b�_�C���N���[�h

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class ModelData;

class ModelLoader
{
public:
	/// �R���X�g���N�^
	ModelLoader(ComPtr<ID3D12Device> mDevice);
	
	/// �f�X�g���N�^
	~ModelLoader();

	/// @fn LoadModel
	/// ���f�������[�h����
	virtual std::shared_ptr<ModelData> LoadModel(const std::string& filePath) = 0;

protected:
	ComPtr<ID3D12Device> mDevice;
};

