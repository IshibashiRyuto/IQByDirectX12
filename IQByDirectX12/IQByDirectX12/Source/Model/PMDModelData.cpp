#include "PMDModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "../InstanceBuffer.h"
#include "InstancingDataManager.h"
#include "../Device.h"
#include "../Texture//TextureLoader.h"
#include "../Texture/TextureManager.h"

PMDModelData::PMDModelData(std::shared_ptr<Device> device, const PMDModelInfo& modelInfo)
	: ModelData(VertexBuffer::Create(device,(void*)modelInfo.vertexData.data(),modelInfo.vertexData.size(), sizeof(PMDVertex)),
		IndexBuffer::Create(device->GetDevice(), (void*)modelInfo.indexData.data(), modelInfo.indexData.size(), sizeof(short)),
		DescriptorHeap::Create(device->GetDevice(), 1 + (int)modelInfo.materials.size() * 2) )
	, mTextureLoader(TextureLoader::Create(device))
{
	SetVertexData(modelInfo.vertexData);
	SetIndexData(modelInfo.indexData);
	SetMaterialData(device, modelInfo.materials, modelInfo.modelPath);
}

PMDModelData::~PMDModelData()
{
}

std::shared_ptr<PMDModelData> PMDModelData::Create(std::shared_ptr<Device> device,	const PMDModelInfo& modelInfo)
{
	auto model = std::shared_ptr<PMDModelData>(new PMDModelData(device, modelInfo));
	if (model->mVertexBuffer == nullptr || model->mIndexBuffer == nullptr)
	{
		return nullptr;
	}
	return model;
}

void PMDModelData::SetVertexData(const std::vector<PMDVertex>& vertexData)
{
	mVertexCount = (unsigned int)vertexData.size();
	mVertex = vertexData;
}

void PMDModelData::SetIndexData(const std::vector<unsigned short>& indexData)
{
	mIndexCount = (unsigned int)indexData.size();
	mIndex = indexData;
}

void PMDModelData::SetMaterialData(std::shared_ptr<Device> device, const std::vector<PMDMaterial>& materials, const std::string& modelPath)
{
	mMaterialCount = (unsigned int)materials.size();
	mMaterials = materials;
	mMaterialData = ConstantBuffer::Create(device, sizeof(PMDShaderMaterialData), mMaterialCount);
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		PMDShaderMaterialData data;
		data.alpha = materials[i].alpha;
		data.diffuseColor = materials[i].diffuseColor;
		data.specularColor = materials[i].specularColor;
		data.specularity = materials[i].specularity;
		data.ambientColor = materials[i].ambientColor;
		data.isUseTexture = (strcmp(materials[i].textureFileName, "") == 0) ? 0 : 1;
		mMaterialData->SetData(&data, sizeof(PMDShaderMaterialData), i);
		mDescHeap->SetConstantBufferView(mMaterialData->GetConstantBufferView(i), i * MATERIAL_SHADER_RESOURCE_NUM + 1);
		std::shared_ptr<Texture> materialTexture;
		if (data.isUseTexture == 0)
		{
			materialTexture = TextureManager::GetInstance().GetTexture(TextureManager::WHITE_TEXTURE);
		}
		else
		{
			std::string texturePath(materials[i].textureFileName);
			texturePath = modelPath.substr(0, max(modelPath.find_last_of('/') + 1, modelPath.find_last_of('\\') + 1)) + texturePath;
			auto textureHandle = mTextureLoader->Load(texturePath);
			materialTexture = TextureManager::GetInstance().GetTexture(textureHandle);
		}
		mDescHeap->SetTexture(materialTexture, i * MATERIAL_SHADER_RESOURCE_NUM + 1 + 1);
	}
}

void PMDModelData::Draw(ComPtr<ID3D12GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	mDescHeap->BindGraphicsCommandList(commandList);
	mDescHeap->BindRootDescriptorTable(0, 0);
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	commandList->IASetVertexBuffers(0, 2, vbViews);
	commandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	int indexOffset = 0;
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		mDescHeap->BindRootDescriptorTable(1, i * MATERIAL_SHADER_RESOURCE_NUM + 1);
		commandList->DrawIndexedInstanced(mMaterials[i].faceVertexCount, instanceData.nowInstanceCount, indexOffset, 0, 0);
		indexOffset += mMaterials[i].faceVertexCount;
	}
}
