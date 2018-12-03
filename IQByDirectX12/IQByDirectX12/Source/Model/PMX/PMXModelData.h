/*
	@file		PMXModelData.h
	@brief		PMXモデルのデータを格納する
	@date		2018/08/28	初版作成
				2018/09/09	頂点データの構造を変更
				2018/11/27	DrawNoMat追加	
*/
#pragma once
/* ヘッダインクルード */
#include <memory>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "PMXModelDataInfo.h"
#include "../ModelData.h"
#include "../Math/Math.h"
#include "../Motion/Pose.h"

/* クラス使用宣言 */
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;
class TextureLoader;
class Device;
using Microsoft::WRL::ComPtr;

/**
*	@class	PMXModelData
*	@brief	PMXモデルデータ
*/
class PMXModelData : public ModelData
{
public:
	/**
	*	@brief	デストラクタ
	*/
	~PMXModelData();

	/**
	*	@brief		PMXのモデルデータを生成する
	*	@note		PMXModelDataはこのクラスを通じてのみ生成可能
	*	@param[in]	device : ID3D12デバイス
	*	@param[in]	vertexData: 頂点情報
	*	@param[in]	vertIndexData: 頂点インデックス情報
	*	@retval		生成成功: PMXModelDataのスマートポインタ
	*	@retval		生成失敗時: nullptr
	*/
	static std::shared_ptr<PMXModelData> Create(std::shared_ptr<Device> device,
		const PMX::ModelDataDesc& modelDataDesc,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	モデル情報の更新処理
	*/
	void Update();

	/**
	*	@brief	描画処理
	*	@param[in]	graphicsCommandList	: コマンドリスト
	*	@param[in]	instanceData		: インスタンスデータ
	*/
	void Draw(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

	/**
	*	@brief	マテリアルを使わない描画
	*
	*	@param[in]	graphicsCommandList	: コマンドリスト
	*	@param[in]	instanceData		: インスタンスデータ
	*/
	void DrawShadow(std::shared_ptr<GraphicsCommandList> commandList, const InstanceData& instanceData) const;

private:
	/* 定数定義 */

	/* 変数宣言 */
	std::shared_ptr<ConstantBuffer> mMaterialDataBuffer;			//! マテリアルデータを保存する定数バッファ
	std::vector<PMX::MaterialData>	mMaterialData;					//! マテリアルデータ
	std::vector<int>				mTextureHandle;					//! テクスチャハンドル
	std::shared_ptr<DescriptorHeap> mBoneHeap;						//! ボーン情報を格納するヒープ
	std::shared_ptr<ConstantBuffer> mBoneMatrixDataBuffer;			//! ボーン行列情報を保存する定数バッファ

	/* ローカルメソッド定義 */

	/**
	*	@brief		モデルのテクスチャデータをロードする
	*	@param[in]	textures		: テクスチャデータ配列
	*	@param[in]	modelFilePath	: モデルファイルパス
	*/
	void LoadModelTexture(const std::vector<PMX::Texture> & textures, const std::wstring& modelFilePath);

	/**
	*	@brief		マテリアルデータをセットする
	*	@param[in]	materials	: マテリアルデータ配列
	*/
	void SetMaterial(const std::vector<PMX::Material>& materials, const std::vector<int>& shareToonTextureIndexies);

	/**
	*	@brief		ボーン情報をセットする
	*
	*	@param[in]	bones	: ボーン情報
	*/
	void SetBone(const std::vector<PMX::BoneData>& bones);

	/**
	*	@brief	ボーン情報を更新する
	*/
	void UpdatePose();


	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device				: dx12デバイス
	*	@param[in]	vertexData			: 頂点情報
	*	@param[in]	indexData			: インデックスデータ
	*	@param[in]	materialCount		: マテリアル数
	*	@param[in]	boneCount			: ボーン数
	*	@param[in]	pipelineStateObject	: 描画用PSO
	*	@param[in]	shadowPSO			: シャドウ描画用PSO
	*	@param[in]	rootSignature		: ルートシグネチャ
	*/
	PMXModelData(std::shared_ptr<Device> device,
		std::vector<PMX::Vertex> vertexData,
		std::vector<PMX::Index> indexData,
		int materialCount,
		int boneCount,
		std::shared_ptr<PipelineStateObject> pipelineStateObject,
		std::shared_ptr<PipelineStateObject> shadowPSO,
		std::shared_ptr<RootSignature> rootSignature);
};