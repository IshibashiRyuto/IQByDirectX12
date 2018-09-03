#include "Model.h"

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

void Model::Draw() const
{
}

void Model::CalcModelMatrix()
{
	mModelMatrix = CreateIdent();
	mModelMatrix *= CreateScaleMatrix(mScale);
	mModelMatrix *= GetMatrixFromQuat(mRotation);
	mModelMatrix *= CreateTranslateMatrix(mPosition);
}
