#include "PMDModelData.h"
#include "../Dx12/Buffer/VertexBuffer.h"
#include "../Dx12/Buffer/IndexBuffer.h"
#include "../Dx12/DescriptorHeap.h"
#include "../Dx12/ConstantBuffer.h"
#include "../Dx12/Buffer/InstanceDataBuffer.h"
#include "../InstancingDataManager.h"
#include "../Dx12/Device.h"
#include "../Texture//TextureLoader.h"
#include "../Texture/TextureManager.h"
#include "../Motion/Pose.h"
#include "../Motion/Bone.h"
#include "../ConvertString.h"
#include "../Dx12/PipelineStateObject.h"

PMDModelData::PMDModelData(std::shared_ptr<Device> device,
	const PMDModelInfo& modelInfo,
	const std::vector<int> shareToonTextureIndex,
	std::shared_ptr<PipelineStateObject> pipelineStateObject)

	: ModelData(VertexBuffer::Create(device,(void*)modelInfo.vertexData.data(), sizeof(PMDVertex), modelInfo.vertexData.size()),
		IndexBuffer::Create(device, (void*)modelInfo.indexData.data(), sizeof(short), modelInfo.indexData.size()),
		DescriptorHeap::Create(device, (int)modelInfo.materials.size() * MATERIAL_SHADER_RESOURCE_NUM),
		pipelineStateObject)
	, mTextureLoader(TextureLoader::Create(device))
{
	SetVertexData(modelInfo.vertexData);
	SetIndexData(modelInfo.indexData);
	SetMaterialData(device, modelInfo.materials, modelInfo.modelPath, shareToonTextureIndex);
	SetBoneData(device, modelInfo.boneData);
}

PMDModelData::~PMDModelData()
{
}

std::shared_ptr<PMDModelData> PMDModelData::Create(std::shared_ptr<Device> device,
	const PMDModelInfo& modelInfo,
	const std::vector<int> shareToonTextureIndex,
	std::shared_ptr<PipelineStateObject> pipelineStateObject)
{
	auto model = std::shared_ptr<PMDModelData>(new PMDModelData(device, modelInfo, shareToonTextureIndex, pipelineStateObject));
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

void PMDModelData::SetMaterialData(std::shared_ptr<Device> device, const std::vector<PMDMaterial>& materials, const std::string& modelPath, const std::vector<int>& shareToonTextureIndex)
{
	mMaterialCount = (unsigned int)materials.size();
	mMaterials = materials;
	mMaterialData = ConstantBuffer::Create(device, sizeof(PMDShaderMaterialData), mMaterialCount);
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		PMDShaderMaterialData data;
		data.diffuseColor = Math::Vector4(materials[i].diffuseColor.x,
			materials[i].diffuseColor.y,
			materials[i].diffuseColor.z,
			materials[i].alpha);
		data.specularColor = materials[i].specularColor;
		data.specularity = materials[i].specularity;
		data.ambientColor = materials[i].ambientColor;
		data.isUseTexture = (strcmp(materials[i].textureFileName, "") == 0) ? 0 : 1;
		mMaterialData->SetData(&data, sizeof(PMDShaderMaterialData), i);
		mDescHeap->SetConstantBufferView(mMaterialData->GetConstantBufferView(i), i * MATERIAL_SHADER_RESOURCE_NUM);
		std::shared_ptr<Texture> surfaceTexture;
		std::shared_ptr<Texture> addSphereTexture;
		std::shared_ptr<Texture> mulSphereTexture;
		surfaceTexture = TextureManager::GetInstance().GetTexture(TextureManager::WHITE_TEXTURE);
		addSphereTexture = TextureManager::GetInstance().GetTexture(TextureManager::BLACK_TEXTURE);
		mulSphereTexture = TextureManager::GetInstance().GetTexture(TextureManager::WHITE_TEXTURE);
		if (data.isUseTexture != 0)
		{
			std::string texturePath(materials[i].textureFileName);
			auto t = texturePath.find('*');
			if (t == std::string::npos)
			{
				texturePath = modelPath.substr(0, max(modelPath.find_last_of('/') + 1, modelPath.find_last_of('\\') + 1)) + texturePath;
				auto textureHandle = mTextureLoader->Load(texturePath);


				if (texturePath.substr(texturePath.rfind('.') + 1, texturePath.size() - 1) == "sph")
				{
					mulSphereTexture = TextureManager::GetInstance().GetTexture(textureHandle);
				}
				else if (texturePath.substr(texturePath.rfind('.') + 1, texturePath.size() - 1) == "spa")
				{
					addSphereTexture = TextureManager::GetInstance().GetTexture(textureHandle);
				}
				else
				{
					surfaceTexture = TextureManager::GetInstance().GetTexture(textureHandle);
				}
			}
			else
			{
				auto surfaceTexturePath = modelPath.substr(0, max(modelPath.find_last_of('/') + 1, modelPath.find_last_of('\\') + 1));
				auto sphereTexturePath = surfaceTexturePath;
				surfaceTexturePath += texturePath.substr(0, t);
				sphereTexturePath += texturePath.substr(t + 1, texturePath.size() - 1);
				int surfaceTextureHandle = mTextureLoader->Load(surfaceTexturePath);
				int sphereTextureHandle = mTextureLoader->Load(sphereTexturePath);
				surfaceTexture = TextureManager::GetInstance().GetTexture(surfaceTextureHandle);
				if (texturePath.substr(texturePath.rfind('.') + 1, texturePath.size() - 1) == "spa")
				{
					addSphereTexture = TextureManager::GetInstance().GetTexture(sphereTextureHandle);
				}
				else
				{
					mulSphereTexture = TextureManager::GetInstance().GetTexture(sphereTextureHandle);
				}
			}
		}

		unsigned char toonIndex = materials[i].toonIndex + 1;

		auto toonTexture = TextureManager::GetInstance().GetTexture( shareToonTextureIndex[ static_cast<int>(toonIndex)] );

		mDescHeap->SetTexture(surfaceTexture, i * MATERIAL_SHADER_RESOURCE_NUM + 1);
		mDescHeap->SetTexture(addSphereTexture, i * MATERIAL_SHADER_RESOURCE_NUM + 2);
		mDescHeap->SetTexture(mulSphereTexture, i * MATERIAL_SHADER_RESOURCE_NUM + 3);
		mDescHeap->SetTexture(toonTexture, i * MATERIAL_SHADER_RESOURCE_NUM + 4);
	}
}

void PMDModelData::SetBoneData(std::shared_ptr<Device> device, const std::vector<PMDBone>& boneData)
{
	mBoneHeap = DescriptorHeap::Create(device, 1);
	mBoneMatrixBuffer = ConstantBuffer::Create(device, 
		static_cast<UINT>( sizeof(Math::Matrix4x4) * boneData.size() )
		, 1);
	mPose = Pose::Create(boneData.size());
	for (unsigned int i = 0; i < boneData.size(); ++i)
	{
		auto boneName = ConvertStringToWString(std::string(boneData[i].boneName));
		auto bone = Bone::Create(boneData[i].headPos);
		if (boneData[i].parentBoneIndex < boneData.size())
		{
			mPose->SetBoneData(boneName, bone, i, boneData[i].parentBoneIndex);
		}
		else
		{
			mPose->SetBoneData(boneName, bone, i);
		}
	}

	mPose->CalcPose();
	std::vector<Math::Matrix4x4> boneMatrixes;
	auto poseBones = mPose->GetBones();
	boneMatrixes.resize(poseBones.size());
	for (int i = 0; i < boneMatrixes.size(); ++i)
	{
		boneMatrixes[i] = poseBones[i]->GetBoneMatrix();
	}
	mBoneMatrixBuffer->SetData(boneMatrixes.data(), static_cast<UINT>(sizeof(Math::Matrix4x4) * boneMatrixes.size()));
	mBoneHeap->SetConstantBufferView(mBoneMatrixBuffer->GetConstantBufferView(), 0);
}

void PMDModelData::Update()
{
	UpdatePose();
}

void PMDModelData::Draw(ComPtr<ID3D12GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	// psoセット
	commandList->SetPipelineState(mPipelineStateObject->GetPipelineStateObject().Get());

	// 頂点情報セット
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	commandList->IASetVertexBuffers(0, 2, vbViews);
	commandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	// ボーン情報をセット
	mBoneHeap->BindGraphicsCommandList(commandList);
	mBoneHeap->BindRootDescriptorTable(2, 0);

	// マテリアルをセットして描画
	mDescHeap->BindGraphicsCommandList(commandList);
	int indexOffset = 0;
	for (unsigned int i = 0; i < mMaterialCount; ++i)
	{
		mDescHeap->BindRootDescriptorTable(1, i * MATERIAL_SHADER_RESOURCE_NUM);
		commandList->DrawIndexedInstanced(mMaterials[i].faceVertexCount, instanceData.nowInstanceCount, indexOffset, 0, 0);
		indexOffset += mMaterials[i].faceVertexCount;
	}
}

void PMDModelData::DrawNoMat(ComPtr<ID3D12GraphicsCommandList> commandList, const InstanceData & instanceData) const
{
	// psoセット
	commandList->SetPipelineState(mPipelineStateObject->GetPipelineStateObject().Get());

	// 頂点情報セット
	D3D12_VERTEX_BUFFER_VIEW vbViews[2] = { mVertexBuffer->GetVertexBufferView(), instanceData.instanceBuffer->GetVertexBufferView() };
	commandList->IASetVertexBuffers(0, 2, vbViews);
	commandList->IASetIndexBuffer(&mIndexBuffer->GetIndexBufferView());

	// ボーン情報をセット
	mBoneHeap->BindGraphicsCommandList(commandList);
	mBoneHeap->BindRootDescriptorTable(2, 0);

	//	モデル描画
	commandList->DrawIndexedInstanced(mIndexBuffer->GetIndexCount(), instanceData.nowInstanceCount, 0, 0, 0);
}

void PMDModelData::UpdatePose()
{
	std::vector<Math::Matrix4x4> boneMatrixes;
	auto poseBones = mPose->GetBones();

	boneMatrixes.resize(poseBones.size());
	for (unsigned int i = 0; i < boneMatrixes.size(); ++i)
	{
		boneMatrixes[i] = poseBones[i]->GetBoneMatrix();
	}
	mBoneMatrixBuffer->SetData(boneMatrixes.data(), static_cast<UINT>(sizeof(Math::Matrix4x4) * boneMatrixes.size()));
}
