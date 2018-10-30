#include "PMXModelData.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../DescriptorHeap.h"
#include "../ConstantBuffer.h"
#include "../InstancingDataManager.h"
#include "../InstanceBuffer.h"
#include "../Texture/TextureLoader.h"
#include "../Texture/TextureManager.h"
#include "../Motion/Pose.h"
#include "../Motion/Bone.h"
#include "../Device.h"
#include "../PipelineStateObject.h"
#include <algorithm>

const int MATERIAL_HEAP_STRIDE = 5;		// マテリアル1要素当たりのヒープのストライド数

PMXModelData::PMXModelData(std::shared_ptr<Device> device,
	std::vector<PMX::Vertex> vertexData,
	std::vector<PMX::Index> indexData,
	int materialCount, int boneCount,
	std::shared_ptr<PipelineStateObject> pipelineStateObject)

	: ModelData(VertexBuffer::Create(device, vertexData.data(), vertexData.size(), sizeof(PMX::Vertex)),
		IndexBuffer::Create(device, indexData.data(), indexData.size(), sizeof(PMX::Index)),
		DescriptorHeap::Create(device, materialCount * MATERIAL_HEAP_STRIDE),
		pipelineStateObject)
	, mMaterialDataBuffer(ConstantBuffer::Create(device, sizeof(PMX::Material), materialCount))
	, mBoneMatrixDataBuffer(ConstantBuffer::Create(device, sizeof(Math::Matrix4x4)*boneCount, 1) )
	, mBoneHeap(DescriptorHeap::Create(device, 1))
{
}

PMXModelData::~PMXModelData()
{
}

std::shared_ptr<PMXModelData> PMXModelData::Create(std::shared_ptr<Device> device, const PMX::ModelDataDesc & modelDataDesc, std::shared_ptr<PipelineStateObject> pipelineStateObject)
{
	auto modelData = std::shared_ptr<PMXModelData>
		(
			new PMXModelData(device,
				modelDataDesc.vertices,
				modelDataDesc.indexies,
				(int)modelDataDesc.materials.size(),
				static_cast<int>(modelDataDesc.bones.size()),
				pipelineStateObject)
			);
	modelData->LoadModelTexture(modelDataDesc.textures, modelDataDesc.modelFilePath);
	modelData->SetMaterial(modelDataDesc.materials, modelDataDesc.shareToonTextureIndexies);
	modelData->SetBone(modelDataDesc.bones);
	return modelData;
}

void PMXModelData::Update()
{
	UpdatePose();
}

void PMXModelData::Draw(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList, const InstanceData & instanceData) const
{
	// PSOセット
	graphicsCommandList->SetPipelineState(mPipelineStateObject->GetPipelineStateObject().Get());

	// ボーン情報バインド
	mBoneHeap->BindGraphicsCommandList(graphicsCommandList);
	mBoneHeap->BindRootDescriptorTable(2, 0);
	
	// 頂点情報セット
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	graphicsCommandList->IASetVertexBuffers(0, 2, vbViews);
	graphicsCommandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());


	// マテリアルをセットして描画
	mDescHeap->BindGraphicsCommandList(graphicsCommandList);
	int indexOffset = 0;
	for (int i = 0; i < mMaterialData.size(); ++i)
	{
		mDescHeap->BindRootDescriptorTable(1, i * MATERIAL_HEAP_STRIDE);
		graphicsCommandList->DrawIndexedInstanced(mMaterialData[i].vertsNum, instanceData.nowInstanceCount, indexOffset, 0, 0);
		indexOffset += mMaterialData[i].vertsNum;
	}
}

void PMXModelData::LoadModelTexture(const std::vector<PMX::Texture>& textures, const std::wstring& modelFilePath)
{
	mTextureHandle.resize(textures.size());
	for (unsigned int i = 0; i < mTextureHandle.size(); ++i)
	{
		mTextureHandle[i] = textures[i].textureHandle;
	}
}

void PMXModelData::SetMaterial(const std::vector<PMX::Material>& materials, const std::vector<int>& shareToonTextureIndexies)
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
		mDescHeap->SetConstantBufferView(mMaterialDataBuffer->GetConstantBufferView(i), i * MATERIAL_HEAP_STRIDE);
		auto normalTexture = TextureManager::GetInstance().GetTexture(mTextureHandle[materials[i].textureIndex]);
		
		auto addSphereTextureIndex = TextureManager::BLACK_TEXTURE;
		auto mulSphereTextureIndex = TextureManager::WHITE_TEXTURE;

		if (materials[i].sphereMode == 1)
		{
			mulSphereTextureIndex = mTextureHandle[materials[i].sphereTextureIndex];
		}
		else if (materials[i].sphereMode == 2)
		{
			addSphereTextureIndex = mTextureHandle[materials[i].sphereTextureIndex];
		}
		auto addSphereTexture = TextureManager::GetInstance().GetTexture(addSphereTextureIndex);
		auto mulSphereTexture = TextureManager::GetInstance().GetTexture(mulSphereTextureIndex);

		auto toonTextureHandle = shareToonTextureIndexies[0];
		if (materials[i].shareToonFlag == 0)
		{
			if (materials[i].toonTextureIndex < mTextureHandle.size())
			{
				toonTextureHandle = mTextureHandle[materials[i].toonTextureIndex];
			}
		}
		else
		{
			toonTextureHandle = shareToonTextureIndexies[materials[i].shareToonTexture + 1];
		}


		auto toonTexture = TextureManager::GetInstance().GetTexture(toonTextureHandle);

		mDescHeap->SetTexture(normalTexture, i * MATERIAL_HEAP_STRIDE + 1);
		mDescHeap->SetTexture(addSphereTexture, i * MATERIAL_HEAP_STRIDE + 2);
		mDescHeap->SetTexture(mulSphereTexture, i * MATERIAL_HEAP_STRIDE + 3);
		mDescHeap->SetTexture(toonTexture, i * MATERIAL_HEAP_STRIDE + 4);
	}
}

void PMXModelData::SetBone(const std::vector<PMX::BoneData>& bones)
{
	mPose = Pose::Create(bones.size());
	for (unsigned int i = 0; i < bones.size(); ++i)
	{
		auto bone = Bone::Create(bones[i].position);


		if (i == 5)
		{
			bone->Rotate(Math::CreateZRotMatrix(0.5f));
		}

		if (bones[i].axisFixed)
		{
			bone->SetLimitAxis(true, bones[i].axisVector);
		}
		if (bones[i].parentBoneIndex < bones.size())
		{
			mPose->SetBoneData(bones[i].name, bone, i, bones[i].parentBoneIndex);
		}
		else
		{
			mPose->SetBoneData(bones[i].name, bone, i);
		}
	}

	std::vector<Math::Matrix4x4> boneMatrixes;
	mPose->CalcPose();
	auto poseBones = mPose->GetBones();

	boneMatrixes.resize(poseBones.size());
	for (unsigned int i = 0; i < boneMatrixes.size(); ++i)
	{
		boneMatrixes[i] = poseBones[i]->GetBoneMatrix();
	}
	mBoneMatrixDataBuffer->SetData(boneMatrixes.data(), static_cast<UINT>( sizeof(Math::Matrix4x4) * boneMatrixes.size()));
	mBoneHeap->SetConstantBufferView(mBoneMatrixDataBuffer->GetConstantBufferView(), 0);
}

void PMXModelData::UpdatePose()
{
	std::vector<Math::Matrix4x4> boneMatrixes;
	auto poseBones = mPose->GetBones();

	boneMatrixes.resize(poseBones.size());
	for (unsigned int i = 0; i < boneMatrixes.size(); ++i)
	{
		boneMatrixes[i] = poseBones[i]->GetBoneMatrix();
	}
	mBoneMatrixDataBuffer->SetData(boneMatrixes.data(), static_cast<UINT>( sizeof(Math::Matrix4x4) * boneMatrixes.size() ) );
}
