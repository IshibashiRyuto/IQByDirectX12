#include "ModelDataManager.h"
#include "../InstancingDataManager.h"
#include "../InstanceBuffer.h"
#include <DirectXMath.h>


void ModelDataManager::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList)
{
	auto instancingDataMap = mInstancingDataManager.GetInstanceDataAll();
	for (auto tData : mData)
	{
		auto modelHandle = tData.first;
		auto modelData = tData.second;
		if (instancingDataMap.find(modelHandle) == instancingDataMap.end()
			|| instancingDataMap[modelHandle].nowInstanceCount <= 0)
		{
			continue;
		}
		modelData->Update();
		modelData->Draw(graphicsCommandList, instancingDataMap[modelHandle]);
	}
	mInstancingDataManager.ResetInstanceDataAll();
}

ModelDataManager::ModelDataManager()
	: mNextHandle(MODEL_DATA_SIGNATURE)
	, mInstancingDataManager(InstancingDataManager::GetInstance())
{
}

ModelDataManager::~ModelDataManager()
{
}

int ModelDataManager::Regist(std::shared_ptr<ModelData> modelData)
{
	int handle = mNextHandle;
	mData[handle] = modelData;
	mInstancingDataManager.ResetMaxInstanceCount(handle, sizeof(DirectX::XMMATRIX), 10000);
	UpdateNextHandle();
	return handle;
}

void ModelDataManager::Erase(int handle)
{
	auto eraseTarget = mData.find(handle);
	if (eraseTarget == mData.end())
	{
		return;
	}
	mData.erase(eraseTarget);
}

std::shared_ptr<ModelData> ModelDataManager::GetModelData(int handle) const
{
	if (!IsExist(handle))
	{
		return nullptr;
	}
	return mData.at(handle);
}

bool ModelDataManager::IsExist(int handle) const
{
	if (mData.find(handle) != mData.end())
	{
		return true;
	}
	return false;
}

void ModelDataManager::UpdateNextHandle()
{
	do
	{
		++mNextHandle;
		if ((mNextHandle >> DATA_SIGNATURE_SHIFT_NUM) > (MODEL_DATA_SIGNATURE >> DATA_SIGNATURE_SHIFT_NUM))
		{
			mNextHandle = MODEL_DATA_SIGNATURE;
		}
	} while (mData.find(mNextHandle) != mData.end());
}