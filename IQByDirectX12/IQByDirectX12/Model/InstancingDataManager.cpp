#include "../VertexBuffer.h"
#include "InstancingDataManager.h"



InstancingDataManager::InstancingDataManager()
{
}


InstancingDataManager::~InstancingDataManager()
{
}

void InstancingDataManager::ResetMaxInstanceCount(int handle, int maxInstanceCount)
{
	if (handle < 0 || maxInstanceCount < 1)
	{
		return;
	}

	mInstanceDataMap[handle].maxInstanceCount = maxInstanceCount;
	mInstanceDataMap[handle].nowInstanceCount = 0;
}
