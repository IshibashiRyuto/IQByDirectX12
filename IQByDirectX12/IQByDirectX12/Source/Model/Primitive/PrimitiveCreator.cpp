#include "PrimitiveCreator.h"
#include "../ModelDataManager.h"
#include "PrimitivePlane.h"
#include "../Model.h"

void PrimitiveCreator::Initialize(std::shared_ptr<Device> device, std::shared_ptr<PipelineStateObject> pipelineState)
{
	mIsInit = true;
	mDevice = device;
	mPipelineState = pipelineState;
}

std::shared_ptr<Model> PrimitiveCreator::CreatePlane(const Math::Vector2 & size, const Primitive::Material & material)
{
	if (!mIsInit)
	{
		return nullptr;
	}
	auto plane = PrimitivePlane::Create(mDevice, size, material, mPipelineState);
	auto modelHandle = ModelDataManager::GetInstance().Regist(plane);

	return Model::Create(modelHandle);
}

PrimitiveCreator::PrimitiveCreator()
{
}


PrimitiveCreator::~PrimitiveCreator()
{
}
