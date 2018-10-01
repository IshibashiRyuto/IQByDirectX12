#include "PMXModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "InstancingDataManager.h"
#include "../InstanceBuffer.h"
#include "../Texture/TextureLoader.h"
#include "../Texture/TextureManager.h"
#include "../Pose.h"
#include "../Bone.h"
#include "../Device.h"

PMXModelData::PMXModelData(std::shared_ptr<Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData, int materialCount, int boneCount)
	: ModelData(VertexBuffer::Create(device->GetDevice(), vertexData.data(), vertexData.size(), sizeof(PMX::Vertex)),
		IndexBuffer::Create(device->GetDevice(), indexData.data(), indexData.size(), sizeof(PMX::Index)),
		DescriptorHeap::Create(device->GetDevice(), 1 + materialCount * 3 + 1))
	, mMaterialDataBuffer(ConstantBuffer::Create(device->GetDevice(), sizeof(PMX::Material), materialCount))
	, mBoneMatrixDataBuffer(ConstantBuffer::Create(device->GetDevice(), sizeof(Math::Matrix4x4)*boneCount, 1) )
	, mTextureLoader(TextureLoader::Create(device))
{
}

PMXModelData::~PMXModelData()
{
}

std::shared_ptr<PMXModelData> PMXModelData::Create(std::shared_ptr<Device> device, std::vector<PMX::Vertex> vertexData, std::vector<PMX::Index> indexData)
{
	return std::shared_ptr<PMXModelData>(new PMXModelData(device, vertexData, indexData, 1, 0));
}

std::shared_ptr<PMXModelData> PMXModelData::Create(std::shared_ptr<Device> device, const PMX::ModelDataDesc & modelDataDesc)
{
	auto modelData = std::shared_ptr<PMXModelData>
		(
			new PMXModelData(device, modelDataDesc.vertices, modelDataDesc.indexies, (int)modelDataDesc.materials.size(), static_cast<int>(modelDataDesc.bones.size()) )
			);
	modelData->LoadModelTexture(modelDataDesc.textures, modelDataDesc.modelFilePath);
	modelData->SetMaterial(modelDataDesc.materials);
	modelData->SetBone(modelDataDesc.bones);
	return modelData;
}

void PMXModelData::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData & instanceData) const
{
	mDescHeap->BindGraphicsCommandList(graphicsCommandList);
	mDescHeap->BindRootDescriptorTable(0, 0);
	mDescHeap->BindRootDescriptorTable(2, 1 + 3 * mMaterialData.size());
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	graphicsCommandList->IASetVertexBuffers(0, 2, vbViews);
	graphicsCommandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());


	// マテリアルをセットして描画
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

void PMXModelData::SetBone(const std::vector<PMX::BoneData>& bones)
{
	mPose = Pose::Create(bones.size());
	for (unsigned int i = 0; i < bones.size(); ++i)
	{
		if (bones[i].parentBoneIndex < bones.size())
		{
			mPose->SetBoneData(bones[i].name, Bone::Create(bones[i].position), i, bones[i].parentBoneIndex);
		}
		else
		{
			mPose->SetBoneData(bones[i].name, Bone::Create(bones[i].position), i);
		}
	}

	// debug
	auto& a = mPose->GetBones();
	a[6]->SetRotation(Math::CreateRotAxisQuaternion(Math::Vector3(0.0f, 0.0f, 1.0f), Math::F_PI / 12.0f) );
	mPose->CalcPose();
	//debugEnd

	std::vector<Math::Matrix4x4> boneMatrixes;
	auto poseBones = mPose->GetBones();

	boneMatrixes.resize(poseBones.size());
	for (unsigned int i = 0; i < boneMatrixes.size(); ++i)
	{
		boneMatrixes[i] = poseBones[i]->GetBoneMatrix();
	}
	mBoneMatrixDataBuffer->SetData(boneMatrixes.data(), sizeof(Math::Matrix4x4) * boneMatrixes.size());
	mDescHeap->SetConstantBufferView(mBoneMatrixDataBuffer->GetConstantBufferView(), 1 + 3 * mMaterialData.size());
}
