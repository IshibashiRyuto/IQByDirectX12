#include "Model.h"
#include "ModelDataManager.h"
#include "../DescriptorHeap.h"
#include "ModelData.h"

using namespace Math;

Model::Model()
{
}

Model::Model(int modelHandle)
	: mModelHandle(modelHandle)
{
}

Model::~Model()
{
}

std::shared_ptr<Model> Model::Create(int modelHandle)
{
	return std::shared_ptr<Model>(new Model(modelHandle));
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

void Model::Draw(ComPtr<ID3D12GraphicsCommandList> commandList) const
{
	auto modelData = ModelDataManager::GetInstance().GetModelData(mModelHandle);
	//modelData->GetDescriptorHeap()->BindGraphicsCommandList(commandList);
	commandList->IASetVertexBuffers(0, 1, &modelData->GetVertexBuffer()->GetVertexBufferView());
	commandList->IASetIndexBuffer(&modelData->GetIndexBuffer()->GetIndexBufferView());
	commandList->DrawIndexedInstanced(modelData->GetIndexBuffer()->GetIndexCount(), 1, 0, 0, 0);
}

std::shared_ptr<DescriptorHeap> Model::_DebugGetDescHeap()
{
	auto& dataManager = ModelDataManager::GetInstance();
	auto modelData = dataManager.GetModelData(mModelHandle);
	return modelData->GetDescriptorHeap();
}


void Model::CalcModelMatrix()
{
	mModelMatrix = CreateIdent();
	mModelMatrix *= CreateScaleMatrix(mScale);
	mModelMatrix *= GetMatrixFromQuat(mRotation);
	mModelMatrix *= CreateTranslateMatrix(mPosition);
}
