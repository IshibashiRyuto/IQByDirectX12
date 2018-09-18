#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include <memory>

// ����w�b�_�C���N���[�h
#include "ModelLoader.h"
#include "PMDModelData.h"


class PMDLoader : public ModelLoader
{
public:
	~PMDLoader();

	static std::shared_ptr<PMDLoader> Create(std::shared_ptr<Device> device);

	/// @fn LoadModel
	std::shared_ptr<Model> LoadModel(const std::string& filePath);

	/// @fn ClearModelData
	/// �ǂݍ��񂾃��f�������폜����
	void ClearModelData();

private:
	PMDLoader(std::shared_ptr<Device> device);
	PMDLoader(const PMDLoader&);
	void operator=(const PMDLoader&) {}
};

