/*
	PMDModelData
	@brief		PMDのモデル情報を格納する
	@param	history
	2018/08/28	初版作成
*/
#pragma once
// システムヘッダインクルード
#include<vector>
#include <memory>
#include <wrl.h>
#include <d3d12.h>

// 自作ヘッダインクルード
#include "../Math/Math.h"
#include "ModelData.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;

///	@struct PMDHeader
/// PMDヘッダ情報
struct PMDHeader
{
	float version;			// バージョン情報
	char model_name[20];	// モデル名
	char comment[256];		// コメント情報
};

#pragma pack(1)

/// @struct PMDVertex
/// PMD頂点情報
struct PMDVertex
{
	Math::Vector3 position;			// 座標情報
	Math::Vector3 normal;			// 法線情報
	Math::Vector2 uv;				// uv情報
	unsigned short boneIndex[2];	// 影響を受けるボーンインデックス
	unsigned char boneWeight;		// 1つ目のボーンに与える影響度(0~100, ボーン2の影響度は100-boneWeight)
	unsigned char edgeFlag;			// 0:通常, 1:エッジ無効
};

struct PMDMaterial
{
	Math::Vector3 diffuseColor;		// 減衰色
	float alpha;					// 減衰色の不透明度
	float specularity;				// スペキュラの強さ
	Math::Vector3 specularColor;	// 光沢色
	Math::Vector3 ambientColor;		// 環境色
	unsigned char toonIndex;
	unsigned char edgeFlag;			// 輪郭、影を使用するか
	unsigned int faceVertexCount;	// 材質が使用する面頂点リストのデータ数
	char textureFileName[20];		// テクスチャファイル名
};

#pragma pack()

class PMDModelData : public ModelData
{
public:
	PMDModelData(ComPtr<ID3D12Device> device, const std::vector<PMDVertex>& vertexData, const std::vector<unsigned short>& indexData, const std::vector<PMDMaterial>& materials);
	~PMDModelData();
	
	static std::shared_ptr<PMDModelData> Create(ComPtr<ID3D12Device> device,
		const std::vector<PMDVertex>& vertexData,
		const std::vector<unsigned short>& indexData,
		const std::vector<PMDMaterial>& materials);

	void SetVertexData(const std::vector<PMDVertex>& vertexData);

	void SetIndexData(const std::vector<unsigned short>& indexData);

	void SetMaterialData(const std::vector<PMDMaterial>& materials);

private:
	unsigned int mVertexCount;			// 頂点数
	std::vector<PMDVertex> mVertex;		// 頂点データ
	unsigned int mIndexCount;
	std::vector<unsigned short> mIndex;	// インデックスデータ
	unsigned int mMaterialCount;
	std::vector<PMDMaterial> mMaterials;
};

