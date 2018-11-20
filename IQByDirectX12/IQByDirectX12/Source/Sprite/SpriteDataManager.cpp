#include "SpriteDataManager.h"
#include "../InstancingDataManager.h"
#include "../Dx12/GraphicsCommandList.h"
#include "SpriteData.h"


void SpriteDataManager::UpdateNextHandle()
{
	do
	{
		++mNextHandle;
		if ((mNextHandle >> DATA_SIGNATURE_SHIFT_NUM) > (SPRITE_DATA_SIGNATURE >> DATA_SIGNATURE_SHIFT_NUM))
		{
			mNextHandle = SPRITE_DATA_SIGNATURE;
		}
	} while (mData.find(mNextHandle) != mData.end());
}

SpriteDataManager::SpriteDataManager()
	: mNextHandle(SPRITE_DATA_SIGNATURE)
	, mInstancingDataManager(InstancingDataManager::GetInstance())
{
}


SpriteDataManager::SpriteDataManager(const SpriteDataManager&)
	: mInstancingDataManager(InstancingDataManager::GetInstance())
{
}


SpriteDataManager::~SpriteDataManager()
{
}

int SpriteDataManager::Regist(std::shared_ptr<SpriteData> spriteData)
{
	int handle = mNextHandle;
	mData[handle] = spriteData;
	mInstancingDataManager.ResetMaxInstanceCount(handle, sizeof(Math::Matrix3x3), 10000);
	UpdateNextHandle();
	return handle;
}

void SpriteDataManager::Erase(int handle)
{
	auto eraseTarget = mData.find(handle);
	if (eraseTarget == mData.end())
	{
		return;
	}
	mData.erase(eraseTarget);
}

bool SpriteDataManager::IsExist(int handle) const
{
	if (mData.find(handle) != mData.end())
	{
		return true;
	}
	return false;
}

void SpriteDataManager::Draw(std::shared_ptr<GraphicsCommandList> commandList)
{
	auto instanceDataMap = mInstancingDataManager.GetInstanceDataAll();
	for (auto tData : mData)
	{
		auto spriteHandle = tData.first;
		auto spriteData = tData.second;
		if (instanceDataMap.find(spriteHandle) == instanceDataMap.end())
		{
			continue;
		}
		spriteData->Update();
		spriteData->Draw(commandList, instanceDataMap[spriteHandle]);
		mInstancingDataManager.ResetInstanceData(spriteHandle);
	}
}
