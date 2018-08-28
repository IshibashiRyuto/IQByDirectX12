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

// ����w�b�_�C���N���[�h
#include "../Math/Math.h"

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

#pragma pack()

class PMDModelData
{
public:
	PMDModelData();
	~PMDModelData();
	
	static std::shared_ptr<PMDModelData> Create(const std::vector<PMDVertex>& vertexData,
		const std::vector<unsigned short>& indexData,
		const std::vector<PMDMaterial>& materials);

	void SetVertexData(const std::vector<PMDVertex>& vertexData);

	void SetIndexData(const std::vector<unsigned short>& indexData);

	void SetMaterialData(const std::vector<PMDMaterial>& materials);

private:
	unsigned int mVertexCount;			// ���_��
	std::vector<PMDVertex> mVertex;		// ���_�f�[�^
	unsigned int mIndexCount;
	std::vector<unsigned short> mIndex;	// �C���f�b�N�X�f�[�^
	unsigned int mMaterialCount;
	std::vector<PMDMaterial> mMaterials;
};

