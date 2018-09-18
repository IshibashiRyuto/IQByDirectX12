#include "ModelLoader.h"



ModelLoader::ModelLoader(std::shared_ptr<Device> device)
	: mDevice(device)
	, mModelDataManager(ModelDataManager::GetInstance())
{
}


ModelLoader::~ModelLoader()
{
}

