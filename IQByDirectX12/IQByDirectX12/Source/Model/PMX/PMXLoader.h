#pragma once
#include <cstdio>
#include <string>
#include "../Math/Math.h"

// システムヘッダインクルード
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード
#include "../ModelLoader.h"
#include "PMXModelData.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class Device;

class PMXLoader : public ModelLoader
{
public:
	PMXLoader(std::shared_ptr<Device> device);
	~PMXLoader();

	///	@brief	PMXローダを取得する
	/// @note	PMXLoaderクラスはこのメソッドを通じてのみ実体化できる
	/// @param[in]	device	: dx12デバイスクラス
	/// @param[in]	shareToonFolderPath	: 共有Toonのフォルダパス
	static std::shared_ptr<PMXLoader> Create(std::shared_ptr<Device> device, const std::string& shareToonFolderPath);

	std::shared_ptr<Model> LoadModel(const std::string& filePath);

	void ClearModelData();

private:
	/* 定数定義 */
	const std::string FILE_SIGNATURE = "PMX ";			//!	ファイルシグネチャ
	const size_t SIGNATURE_SIZE = sizeof(char) * 4;		//!	ファイルシグネチャのサイズ
	const int SHARE_TOON_NUM = 11;						//!	共有トゥーンの数
	const std::string SHARE_TOON_PATH [11]
	{ 
		"toon0.bmp",
		"toon01.bmp",
		"toon02.bmp",
		"toon03.bmp",
		"toon04.bmp",
		"toon05.bmp",
		"toon06.bmp",
		"toon07.bmp",
		"toon08.bmp",
		"toon09.bmp",
		"toon10.bmp",
	};			//! 共有toonのファイル名

	/* 変数宣言 */
	std::vector<int> mShareToonTextureHandle;		// 共有トゥーンテクスチャのハンドル


	/* ローカルメソッド定義 */
	/// @brief	文字列バッファデータを取得する
	/// @param[in] fp: ファイルポインタ
	/// @note テキストデータは4 + n[Byte]で格納されているものとする。
	///		最初の4[Byte]にバッファサイズを、続くn[Byte]に文字列データが格納されている
	std::string ReadTextBuf(FILE *fp);

	/// @brief	ワイド文字列バッファデータを取得する
	/// @param[in] fp: ファイルポインタ
	std::wstring ReadTextBufWString(FILE *fp);

	/// @biref	ヘッダデータを読み込む
	/// @param[in] header : PMXのヘッダ
	/// @param[in] fp: ファイルポインタ
	void LoadHeader(PMX::Header& header, FILE* fp);

	/// @brief	モデル情報を読み込む
	/// @param[in] modelInfo	: モデル情報構造体
	/// @param[in] fp			: ファイルポインタ
	void LoadModelInfo(PMX::ModelInfo& modelInfo, FILE* fp);

	/// @brief	頂点データを読み込む
	/// @param[in] vertexData	: 頂点データのベクタ
	/// @param[in] header		: PMXファイルヘッダ
	/// @param[in: fp			: ファイルポインタ
	void LoadVertexData(std::vector<PMX::Vertex>& vertexData, const PMX::Header& header, FILE* fp);

	/// @brief	頂点インデックスデータを読み込む
	/// @param[in] indexData	: 頂点インデックスデータのベクタ
	/// @param[in] header		: PMXファイルヘッダ
	/// @param[in] fp			: ファイルポインタ
	void LoadIndexData(std::vector<PMX::Index>& indexData, const PMX::Header& header, FILE* fp);

	/// @brief	テクスチャ情報を読み込む
	/// @param[in] textureData	: テクスチャデータのベクタ
	/// @param[in] modelPath	: モデルのファイルパス
	/// @param[in] fp			: ファイルポインタ
	void LoadTextureData(std::vector<PMX::Texture>& textureData, const std::wstring& modelPath, FILE* fp);

	/// @brief	マテリアル情報を読み込む
	/// @param[in] materialData		: マテリアルデータのベクタ
	/// @param[in] header			: PMXファイルヘッダ
	/// @param[in] fp				: ファイルポインタ
	void LoadMaterial(std::vector<PMX::Material>& materialData, const PMX::Header& header, FILE* fp);

	/// @brief	ボーン情報を読み込む
	/// @param[in]	boneData		: ボーンデータ配列
	/// @param[in]	boneIndexSize	: ボーンインデックスのサイズ
	/// @param[in]	fp				: ファイルポインタ
	void LoadBone(std::vector<PMX::BoneData> & boneData, size_t boneIndexSize, FILE *fp);

	/// @brief	モーフ情報を読み込む
	/// @param[in] morphData	: モーフデータ配列
	/// @param[in] header		: PMXファイルヘッダ
	void LoadMorph(std::vector<PMX::Morph> & morphData, const PMX::Header& header, FILE* fp);

	/// @brief	表示枠を読み込む
	/// @param[in] displayFrameData	:表示枠データ配列
	/// @param[in] boneIndexSize	: ボーンインデックスのサイズ
	/// @param[in] morphIndexSize	: モーフインデックスのサイズ
	/// @param[in] fp				: ファイルポインタ
	void LoadDisplayFrame(std::vector<PMX::DisplayFrame> & displayFrameData, size_t boneIndexSize, size_t morphIndexSize, FILE *fp);

	/// @brief	共有トゥーンテクスチャを読み込む
	/// @param[in]	共有トゥーンテクスチャのあるフォルダパス
	void LoadShareToon(const std::string& folderPath);
};

