#pragma once
#include <cstdio>
#include <string>
#include "../Math/Math.h"

// システムヘッダインクルード
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード
#include "PMXModelData.h"
#include "ModelLoader.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class TextureLoader;

class PMXLoader : public ModelLoader
{
public:
	PMXLoader(ComPtr<ID3D12Device> device);
	~PMXLoader();


	static std::shared_ptr<PMXLoader> Create(ComPtr<ID3D12Device> device);

	std::shared_ptr<Model> LoadModel(const std::string& filePath);

	void ClearModelData();

private:
	/* 定数定義 */
	const std::string FILE_SIGNATURE = "PMX ";
	const size_t SIGNATURE_SIZE = sizeof(char) * 4;

	/* ローカルメソッド定義 */
	/// @fn ReadTextBuf
	/// 文字列バッファデータを取得する
	/// @param[in] fp: ファイルポインタ
	/// @note テキストデータは4 + n[Byte]で格納されているものとする。
	///		最初の4[Byte]にバッファサイズを、続くn[Byte]に文字列データが格納されている
	std::string ReadTextBuf(FILE *fp);

	/// @fn ReadTextBufWString
	/// ワイド文字列バッファデータを取得する
	/// @param[in] fp: ファイルポインタ
	std::wstring ReadTextBufWString(FILE *fp);

	/// @fn LoadHeader
	/// ヘッダデータを読み込む
	/// @param[in] header : PMXのヘッダ
	/// @param[in] fp: ファイルポインタ
	void LoadHeader(PMX::Header& header, FILE* fp);

	/// @fn LoadModelInfo
	/// モデル情報を読み込む
	/// @param[in] modelInfo	: モデル情報構造体
	/// @param[in] fp			: ファイルポインタ
	void LoadModelInfo(PMX::ModelInfo& modelInfo, FILE* fp);

	/// @fn LoadVertexData
	/// 頂点データを読み込む
	/// @param[in] vertexData	: 頂点データのベクタ
	/// @param[in] header		: PMXファイルヘッダ
	/// @param[in: fp			: ファイルポインタ
	void LoadVertexData(std::vector<PMX::Vertex>& vertexData, const PMX::Header& header, FILE* fp);

	/// @fn LoadIndexData
	/// 頂点インデックスデータを読み込む
	/// @param[in] indexData	: 頂点インデックスデータのベクタ
	/// @param[in] header		: PMXファイルヘッダ
	/// @param[in] fp			: ファイルポインタ
	void LoadIndexData(std::vector<PMX::Index>& indexData, const PMX::Header& header, FILE* fp);

	/// @fn LoadTextureData
	/// テクスチャ情報を読み込む
	/// @param[in] textureData	: テクスチャデータのベクタ
	/// @param[in] fp			: ファイルポインタ
	void LoadTextureData(std::vector<PMX::Texture>& textureData, FILE* fp);

	/// @fn LoadMaterial
	/// マテリアル情報を読み込む
	/// @param[in] materialData		: マテリアルデータのベクタ
	/// @param[in] header			: PMXファイルヘッダ
	/// @param[in] fp				: ファイルポインタ
	void LoadMaterial(std::vector<PMX::Material>& materialData, const PMX::Header& header, FILE* fp);

	/// @fn LoadBone
	/// ボーン情報を読み込む
	/// @param[in]	boneData		: ボーンデータ配列
	/// @param[in]	boneIndexSize	: ボーンインデックスのサイズ
	/// @param[in]	fp				: ファイルポインタ
	void LoadBone(std::vector<PMX::BoneData> & boneData, size_t boneIndexSize, FILE *fp);

	/// @fn LoadMorph
	/// モーフ情報を読み込む
	/// @param[in] morphData	: モーフデータ配列
	/// @param[in] header		: PMXファイルヘッダ
	void LoadMorph(std::vector<PMX::Morph> & morphData, const PMX::Header& header, FILE* fp);

	/// @fn LoadDisplayFrame
	/// 表示枠を読み込む
	/// @param[in] displayFrameData	:表示枠データ配列
	/// @param[in] boneIndexSize	: ボーンインデックスのサイズ
	/// @param[in] morphIndexSize	: モーフインデックスのサイズ
	/// @param[in] fp				: ファイルポインタ
	void LoadDisplayFrame(std::vector<PMX::DisplayFrame> & displayFrameData, size_t boneIndexSize, size_t morphIndexSize, FILE *fp);
};

