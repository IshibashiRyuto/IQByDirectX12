#include "ModelLoader.h"
#include "../Texture/TextureLoader.h"


ModelLoader::ModelLoader(std::shared_ptr<Device> device)
	: mDevice(device)
	, mModelDataManager(ModelDataManager::GetInstance())
	, mTextureLoader(TextureLoader::Create(device))
{
}


ModelLoader::~ModelLoader()
{
}

