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

class PMXLoader : public ModelLoader
{
public:
	PMXLoader(ComPtr<ID3D12Device> device);
	~PMXLoader();


	static std::shared_ptr<PMXLoader> Create(ComPtr<ID3D12Device> device);

	std::shared_ptr<Model> LoadModel(const std::string& filePath);

	void ClearModelData();

private:
	ComPtr<ID3D12Device> mDevice;
	const std::string FILE_SIGNATURE = "PMX ";
	const size_t SIGNATURE_SIZE = sizeof(char) * 4;

	/// @fn ReadTextBuf
	/// 文字列バッファデータを取得する
	/// @param[in] fp: ファイルポインタ
	/// @note テキストデータは4 + n[Byte]で格納されているものとする。
	///		最初の4[Byte]にバッファサイズを、続くn[Byte]に文字列データが格納されている
	std::string ReadTextBuf(FILE *fp);

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
	/// 
};

