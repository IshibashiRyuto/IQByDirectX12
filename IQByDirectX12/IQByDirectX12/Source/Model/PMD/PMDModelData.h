/*
	PMDModelData
	@brief		PMD�̃��f�������i�[����
	@param	history
	2018/08/28	���ō쐬
*/
#pragma once
// �V�X�e���w�b�_�C���N���[�h
#include<vector>
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// ����w�b�_�C���N���[�h
#include "../../Math/Math.h"
#include "../ModelData.h"

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class ConstantBuffer;
class Device;
class TextureLoader;


/*
*		PMD����`
*		�Q�l: https://blog.goo.ne.jp/torisu_tetosuki
*/

///	@struct PMDHeader
/// PMD�w�b�_���
struct PMDHeader
{
	float version;			// �o�[�W�������
	char model_name[20];	// ���f����
	char comment[256];		// �R�����g���
};

#pragma pack(1)

/// @struct PMDVertex
/// PMD���_���
struct PMDVertex
{
	Math::Vector3 position;			// ���W���
	Math::Vector3 normal;			// �@�����
	Math::Vector2 uv;				// uv���
	unsigned short boneIndex[2];	// �e�����󂯂�{�[���C���f�b�N�X
	unsigned char boneWeight;		// 1�ڂ̃{�[���ɗ^����e���x(0~100, �{�[��2�̉e���x��100-boneWeight)
	unsigned char edgeFlag;			// 0:�ʏ�, 1:�G�b�W����
};

struct PMDMaterial
{
	Math::Vector3 diffuseColor;		// �����F
	float alpha;					// �����F�̕s�����x
	float specularity;				// �X�y�L�����̋���
	Math::Vector3 specularColor;	// ����F
	Math::Vector3 ambientColor;		// ���F
	unsigned char toonIndex;
	unsigned char edgeFlag;			// �֊s�A�e���g�p���邩
	unsigned int faceVertexCount;	// �ގ����g�p����ʒ��_���X�g�̃f�[�^��
	char textureFileName[20];		// �e�N�X�`���t�@�C����
};

struct PMDShaderMaterialData
{
	Math::Vector4 diffuseColor;
	float specularity;
	Math::Vector3 specularColor;
	Math::Vector3 ambientColor;
	int isUseTexture;
	int sphereFlag;				// �X�t�B�A�t���O ... 0: ���Z�X�t�B�A, 1: ��Z�X�t�B�A
};

struct PMDBone
{
	char boneName[20];
	unsigned int parentBoneIndex;
	unsigned int tailBoneIndex;
	unsigned char boneType;
	unsigned int ikParentBoneIndex;
	Math::Vector3 headPos;
};

#pragma pack()

struct PMDModelInfo
{
	std::string modelPath;
	std::vector<PMDVertex> vertexData;
	std::vector<unsigned short> indexData;
	std::vector<PMDMaterial> materials;
};

class PMDModelData : public ModelData
{
public:
	PMDModelData(std::shared_ptr<Device> device, const PMDModelInfo& modelInfo, const std::vector<int> shareToonTextureIndex);
	~PMDModelData();
	
	static std::shared_ptr<PMDModelData> Create(std::shared_ptr<Device> device,	const PMDModelInfo& modelInfo, const std::vector<int> shareToonTextureIndex);

	void SetVertexData(const std::vector<PMDVertex>& vertexData);

	void SetIndexData(const std::vector<unsigned short>& indexData);

	void SetMaterialData(std::shared_ptr<Device> device, const std::vector<PMDMaterial>& materials, const std::string& modelPath, const std::vector<int>& shareToonTextureIndex);

	void Draw(ComPtr<ID3D12GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	static const int MATERIAL_SHADER_RESOURCE_NUM = 5;

	unsigned int					mVertexCount;	// ���_��
	std::vector<PMDVertex>			mVertex;		// ���_�f�[�^
	unsigned int					mIndexCount;	// ���C���f�b�N�X��
	std::vector<unsigned short>		mIndex;			// �C���f�b�N�X�f�[�^
	unsigned int					mMaterialCount;	// �}�e���A����
	std::vector<PMDMaterial>		mMaterials;		// �}�e���A���f�[�^
	std::shared_ptr<ConstantBuffer> mMaterialData;	// �}�e���A���f�[�^�p�萔�o�b�t�@
	std::shared_ptr<TextureLoader>	mTextureLoader;	// ���f���e�N�X�`�����[�_
};

