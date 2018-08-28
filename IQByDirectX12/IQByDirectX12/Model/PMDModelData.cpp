#include "PMDModelData.h"



PMDModelData::PMDModelData()
{
}


PMDModelData::~PMDModelData()
{
}

std::shared_ptr<PMDModelData> PMDModelData::Create( const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials)
{
	auto model = std::shared_ptr<PMDModelData>(new PMDModelData());
	model->SetVertexData(vertexData);
	model->SetIndexData(indexData);
	model->SetMaterialData(materials);
	return model;
}

void PMDModelData::SetVertexData(const std::vector<PMDVertex>& vertexData)
{
	mVertexCount = vertexData.size();
	mVertex = vertexData;
}

void PMDModelData::SetIndexData(const std::vector<unsigned short>& indexData)
{
	mIndexCount = indexData.size();
	mIndex = indexData;
}

void PMDModelData::SetMaterialData(const std::vector<PMDMaterial>& materials)
{
	mMaterialCount = materials.size();
	mMaterials = materials;
}
