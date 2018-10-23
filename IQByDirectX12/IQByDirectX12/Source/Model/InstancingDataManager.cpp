#include "../InstanceBuffer.h"
#include "InstancingDataManager.h"



InstancingDataManager::InstancingDataManager()
{
}

InstancingDataManager::~InstancingDataManager()
{
}

void InstancingDataManager::SetDevice(std::shared_ptr<Device> device)
{
	mDevice = device;
}

void InstancingDataManager::SetInstanceData(int handle, void * data, size_t size)
{
	if (mInstanceDataMap.find(handle) == mInstanceDataMap.end()
		|| !mInstanceDataMap[handle].instanceBuffer
		|| mInstanceDataMap[handle].nowInstanceCount >= mInstanceDataMap[handle].maxInstanceCount)
	{
		return;
	}
	mInstanceDataMap[handle].instanceBuffer->SetInstanceData(data, mInstanceDataMap[handle].nowInstanceCount);
	++mInstanceDataMap[handle].nowInstanceCount;
}

void InstancingDataManager::ResetMaxInstanceCount(int handle, size_t instanceDataSize, int maxInstanceCount)
{
	if (!mDevice)
	{
		return;
	}
	if (handle < 0 || maxInstanceCount < 0 || instanceDataSize < 0)
	{
		return;
	}

	mInstanceDataMap[handle].maxInstanceCount = maxInstanceCount;
	mInstanceDataMap[handle].nowInstanceCount = 0;
	mInstanceDataMap[handle].dataSize = instanceDataSize;
	mInstanceDataMap[handle].instanceBuffer = InstanceBuffer::Create(mDevice, instanceDataSize, maxInstanceCount);
}

void InstancingDataManager::ResetInstanceData(int handle)
{
	if (mInstanceDataMap.find(handle) == mInstanceDataMap.end())
	{
		return;
	}
	mInstanceDataMap[handle].nowInstanceCount = 0;
}

void InstancingDataManager::ResetInstanceDataAll()
{
	for (auto& instanceData : mInstanceDataMap)
	{
		instanceData.second.nowInstanceCount = 0;
	}
}

const std::map<int, InstanceData>& InstancingDataManager::GetInstanceDataAll()
{
	return mInstanceDataMap;
}

