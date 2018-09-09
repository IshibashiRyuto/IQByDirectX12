#include "PMXModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "InstancingDataManager.h"
#include "../InstanceBuffer.h"
#include "../Texture/TextureLoader.h"
#include "../Texture/TextureManager.h"

PMXModelData::PMXModelData(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData, int materialCount)
	: ModelData(VertexBuffer::Create(device, vertexData.data(), vertexData.size(), sizeof(PMX::Vertex)),
		IndexBuffer::Create(device, indexData.data(), indexData.size(), sizeof(PMX::Index)),
		DescriptorHeap::Create(device, 1 + materialCount * 3))
	, mMaterialDataBuffer(ConstantBuffer::Create(device, sizeof(PMX::Material), materialCount))
	, mTextureLoader(TextureLoader::Create(device))
{
}

PMXModelData::~PMXModelData()
{
}

std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData)
{
	return std::shared_ptr<PMXModelData>(new PMXModelData(device, vertexData, indexData, 1));
}

std::shared_ptr<PMXModelData> PMXModelData::Create(ComPtr<ID3D12Device> device, const PMX::ModelDataDesc & modelDataDesc)
{
	auto modelData = std::shared_ptr<PMXModelData>(new PMXModelData(device, modelDataDesc.vertices, modelDataDesc.indexies, (int)modelDataDesc.materials.size()));
	modelData->LoadModelTexture(modelDataDesc.textures, modelDataDesc.modelFilePath);
	modelData->SetMaterial(modelDataDesc.materials);
	return modelData;
}

void PMXModelData::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData & instanceData) const
{
	mDescHeap->BindGraphicsCommandList(graphicsCommandList);
	mDescHeap->BindRootDescriptorTable(0, 0);
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	graphicsCommandList->IASetVertexBuffers(0, 2, vbViews);
	graphicsCommandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	int indexOffset = 0;
	for (int i = 0; i < mMaterialData.size(); ++i)
	{
		mDescHeap->BindRootDescriptorTable(1, i * 3 + 1);
		graphicsCommandList->DrawIndexedInstanced(mMaterialData[i].vertsNum, instanceData.nowInstanceCount, indexOffset, 0, 0);
		indexOffset += mMaterialData[i].vertsNum;
	}
}

void PMXModelData::LoadModelTexture(const std::vector<PMX::Texture>& textures, const std::wstring& modelFilePath)
{
	mTextureHandle.resize(textures.size());
	for (unsigned int i = 0; i < mTextureHandle.size(); ++i)
	{
		auto modelTexturePath = modelFilePath.substr(0, modelFilePath.find_last_of('/')+1) + textures[i].texturePath;
		mTextureHandle[i] = mTextureLoader->Load(modelTexturePath);
	}
}

void PMXModelData::SetMaterial(const std::vector<PMX::Material>& materials)
{
	mMaterialData.resize(materials.size());
	for (int i = 0; i < materials.size(); ++i)
	{
		mMaterialData[i].diffuse = materials[i].diffuse;
		mMaterialData[i].specular = materials[i].specular;
		mMaterialData[i].specularity = materials[i].specularFactor;
		mMaterialData[i].ambient = materials[i].ambient;
		mMaterialData[i].vertsNum = materials[i].vertNum;
		mMaterialDataBuffer->SetData(&mMaterialData[i], sizeof(PMX::MaterialData), i);
		mDescHeap->SetConstantBufferView(mMaterialDataBuffer->GetConstantBufferView(i), i * 3 + 1);
		auto normalTexture = TextureManager::GetInstance().GetTexture(mTextureHandle[materials[i].textureIndex]);
		mDescHeap->SetTexture(normalTexture, i*3 + 2);
		auto sphereTexture = TextureManager::GetInstance().GetTexture(mTextureHandle[materials[i].textureIndex]);
		mDescHeap->SetTexture(sphereTexture, i * 3 + 3);
	}
}
