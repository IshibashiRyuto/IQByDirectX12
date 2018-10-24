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
#include "../../Math/Math.h"
#include "../ModelData.h"

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class ConstantBuffer;
class Device;
class TextureLoader;


/*
*		PMD情報定義
*		参考: https://blog.goo.ne.jp/torisu_tetosuki
*/

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

struct PMDShaderMaterialData
{
	Math::Vector4 diffuseColor;
	float specularity;
	Math::Vector3 specularColor;
	Math::Vector3 ambientColor;
	int isUseTexture;
	int sphereFlag;				// スフィアフラグ ... 0: 加算スフィア, 1: 乗算スフィア
};

struct PMDBone
{
	char boneName[20];
	unsigned short parentBoneIndex;
	unsigned short tailBoneIndex;
	unsigned char boneType;
	unsigned short ikParentBoneIndex;
	Math::Vector3 headPos;
};

#pragma pack()

struct PMDModelInfo
{
	std::string					modelPath;
	std::vector<PMDVertex>		vertexData;
	std::vector<unsigned short> indexData;
	std::vector<PMDMaterial>	materials;
	std::vector<PMDBone>		boneData;
};

class PMDModelData : public ModelData
{
public:
	PMDModelData(std::shared_ptr<Device> device,
		const PMDModelInfo& modelInfo,
		const std::vector<int> shareToonTextureIndex,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);
	~PMDModelData();
	
	/// @brief	PMDモデルを生成する
	/// @param[in]	device					: dx12デバイス
	/// @param[in]	modelInfo				: モデル情報
	/// @param[in]	shareToonTextureIndex	: 共有テクスチャトゥーンのインデックス
	/// @param[in]	pipelineStateObject		: パイプラインステートオブジェクト
	static std::shared_ptr<PMDModelData> Create(std::shared_ptr<Device> device,
		const PMDModelInfo& modelInfo,
		const std::vector<int> shareToonTextureIndex,
		std::shared_ptr<PipelineStateObject> pipelineStateObject);

	void SetVertexData(const std::vector<PMDVertex>& vertexData);

	void SetIndexData(const std::vector<unsigned short>& indexData);

	void SetMaterialData(std::shared_ptr<Device> device, const std::vector<PMDMaterial>& materials, const std::string& modelPath, const std::vector<int>& shareToonTextureIndex);

	void SetBoneData(std::shared_ptr<Device> device, const std::vector<PMDBone>& mBone);

	/// @brief		描画処理
	/// @param[in]	commandList		: 描画対象コマンドリスト
	/// @param[in]	instanceData	: インスタンス情報
	void Draw(ComPtr<ID3D12GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	static const int MATERIAL_SHADER_RESOURCE_NUM = 5;

	unsigned int					mVertexCount;	// 頂点数
	std::vector<PMDVertex>			mVertex;		// 頂点データ
	unsigned int					mIndexCount;	// 総インデックス数
	std::vector<unsigned short>		mIndex;			// インデックスデータ
	unsigned int					mMaterialCount;	// マテリアル数
	std::vector<PMDMaterial>		mMaterials;		// マテリアルデータ
	std::vector<PMDBone>			mBoneData;		// ボーン情報
	std::shared_ptr<ConstantBuffer> mMaterialData;	// マテリアルデータ用定数バッファ
	std::shared_ptr<TextureLoader>	mTextureLoader;	// モデルテクスチャローダ
	std::shared_ptr<DescriptorHeap> mBoneHeap;		// ボーン情報用ヒープ
	std::shared_ptr<ConstantBuffer> mBoneMatrixBuffer;	// ボーン情報用バッファ
};
