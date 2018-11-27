#pragma once
#include <cstdio>
#include <string>
#include "../Math/Math.h"

// �V�X�e���w�b�_�C���N���[�h
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// ����w�b�_�C���N���[�h
#include "PMXModelData.h"
#include "ModelLoader.h"

// �N���X�g�p�錾
using Microsoft::WRL::ComPtr;
class Device;
class TextureLoader;

class PMXLoader : public ModelLoader
{
public:
	PMXLoader(std::shared_ptr<Device> device);
	~PMXLoader();


	static std::shared_ptr<PMXLoader> Create(std::shared_ptr<Device> device);

	std::shared_ptr<Model> LoadModel(const std::string& filePath);

	void ClearModelData();

private:
	/* �萔��` */
	const std::string FILE_SIGNATURE = "PMX ";
	const size_t SIGNATURE_SIZE = sizeof(char) * 4;

	/* ���[�J�����\�b�h��` */
	/// @fn ReadTextBuf
	/// ������o�b�t�@�f�[�^���擾����
	/// @param[in] fp: �t�@�C���|�C���^
	/// @note �e�L�X�g�f�[�^��4 + n[Byte]�Ŋi�[����Ă�����̂Ƃ���B
	///		�ŏ���4[Byte]�Ƀo�b�t�@�T�C�Y���A����n[Byte]�ɕ�����f�[�^���i�[����Ă���
	std::string ReadTextBuf(FILE *fp);

	/// @fn ReadTextBufWString
	/// ���C�h������o�b�t�@�f�[�^���擾����
	/// @param[in] fp: �t�@�C���|�C���^
	std::wstring ReadTextBufWString(FILE *fp);

	/// @fn LoadHeader
	/// �w�b�_�f�[�^��ǂݍ���
	/// @param[in] header : PMX�̃w�b�_
	/// @param[in] fp: �t�@�C���|�C���^
	void LoadHeader(PMX::Header& header, FILE* fp);

	/// @fn LoadModelInfo
	/// ���f������ǂݍ���
	/// @param[in] modelInfo	: ���f�����\����
	/// @param[in] fp			: �t�@�C���|�C���^
	void LoadModelInfo(PMX::ModelInfo& modelInfo, FILE* fp);

	/// @fn LoadVertexData
	/// ���_�f�[�^��ǂݍ���
	/// @param[in] vertexData	: ���_�f�[�^�̃x�N�^
	/// @param[in] header		: PMX�t�@�C���w�b�_
	/// @param[in: fp			: �t�@�C���|�C���^
	void LoadVertexData(std::vector<PMX::Vertex>& vertexData, const PMX::Header& header, FILE* fp);

	/// @fn LoadIndexData
	/// ���_�C���f�b�N�X�f�[�^��ǂݍ���
	/// @param[in] indexData	: ���_�C���f�b�N�X�f�[�^�̃x�N�^
	/// @param[in] header		: PMX�t�@�C���w�b�_
	/// @param[in] fp			: �t�@�C���|�C���^
	void LoadIndexData(std::vector<PMX::Index>& indexData, const PMX::Header& header, FILE* fp);

	/// @fn LoadTextureData
	/// �e�N�X�`������ǂݍ���
	/// @param[in] textureData	: �e�N�X�`���f�[�^�̃x�N�^
	/// @param[in] fp			: �t�@�C���|�C���^
	void LoadTextureData(std::vector<PMX::Texture>& textureData, FILE* fp);

	/// @fn LoadMaterial
	/// �}�e���A������ǂݍ���
	/// @param[in] materialData		: �}�e���A���f�[�^�̃x�N�^
	/// @param[in] header			: PMX�t�@�C���w�b�_
	/// @param[in] fp				: �t�@�C���|�C���^
	void LoadMaterial(std::vector<PMX::Material>& materialData, const PMX::Header& header, FILE* fp);

	/// @fn LoadBone
	/// �{�[������ǂݍ���
	/// @param[in]	boneData		: �{�[���f�[�^�z��
	/// @param[in]	boneIndexSize	: �{�[���C���f�b�N�X�̃T�C�Y
	/// @param[in]	fp				: �t�@�C���|�C���^
	void LoadBone(std::vector<PMX::BoneData> & boneData, size_t boneIndexSize, FILE *fp);

	/// @fn LoadMorph
	/// ���[�t����ǂݍ���
	/// @param[in] morphData	: ���[�t�f�[�^�z��
	/// @param[in] header		: PMX�t�@�C���w�b�_
	void LoadMorph(std::vector<PMX::Morph> & morphData, const PMX::Header& header, FILE* fp);

	/// @fn LoadDisplayFrame
	/// �\���g��ǂݍ���
	/// @param[in] displayFrameData	:�\���g�f�[�^�z��
	/// @param[in] boneIndexSize	: �{�[���C���f�b�N�X�̃T�C�Y
	/// @param[in] morphIndexSize	: ���[�t�C���f�b�N�X�̃T�C�Y
	/// @param[in] fp				: �t�@�C���|�C���^
	void LoadDisplayFrame(std::vector<PMX::DisplayFrame> & displayFrameData, size_t boneIndexSize, size_t morphIndexSize, FILE *fp);
};
