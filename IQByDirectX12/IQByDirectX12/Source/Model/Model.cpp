#include "Model.h"
#include "ModelDataManager.h"
#include "../Dx12/DescriptorHeap.h"
#include "ModelData.h"
#include "../InstancingDataManager.h"

using namespace Math;

Model::Model()
	: mInstancingDataManager(InstancingDataManager::GetInstance())
{
}

Model::Model(int modelHandle)
	: mModelHandle(modelHandle)
	, mInstancingDataManager(InstancingDataManager::GetInstance())
	, mPosition(0.0f, 0.0f, 0.0f)
	, mRotation(0.0f, 0.0f, 0.0f)
	, mScale(1.0f, 1.0f, 1.0f)
{
	CalcModelMatrix();
}

Model::~Model()
{
}

std::shared_ptr<Model> Model::Create(int modelHandle)
{
	return std::shared_ptr<Model>(new Model(modelHandle));
}

std::shared_ptr<Model> Model::Create(std::shared_ptr<Model> model)
{
	return std::shared_ptr<Model>(new Model(model->mModelHandle));
}

void Model::SetPosition(const Math::Vector3 & position)
{
	mPosition = position;
	CalcModelMatrix();
}

void Model::SetRotation(const Math::Vector3 & rotation)
{
	mRotation = CreateRotXYZQuaternion(rotation);
	CalcModelMatrix();
}

void Model::SetRotation(const Math::Quaternion & rotQuaternion)
{
	mRotation = rotQuaternion;
	CalcModelMatrix();
}

void Model::SetScale(const Math::Vector3 & scale)
{
	mScale = scale;
	CalcModelMatrix();
}

void Model::SetScale(float scale)
{
	SetScale(Vector3(scale, scale, scale));
}

void Model::Draw() const
{
	mInstancingDataManager.SetInstanceData(mModelHandle, (void*)&mModelMatrix, sizeof(Math::Matrix4x4));
}


std::shared_ptr<DescriptorHeap> Model::_DebugGetDescHeap()
{
	auto& dataManager = ModelDataManager::GetInstance();
	auto modelData = dataManager.GetModelData(mModelHandle);
	return modelData->GetDescriptorHeap();
}

std::shared_ptr<Pose> Model::_DebugGetPose()
{
	auto& dataManager = ModelDataManager::GetInstance();
	auto modelData = dataManager.GetModelData(mModelHandle);
	return modelData->_DebugGetPose();
}


void Model::CalcModelMatrix()
{
	mModelMatrix = CreateIdent();
	mModelMatrix *= CreateScaleMatrix(mScale);
	mModelMatrix *= GetMatrixFromQuat(mRotation);
	mModelMatrix *= CreateTranslateMatrix(mPosition);
}
