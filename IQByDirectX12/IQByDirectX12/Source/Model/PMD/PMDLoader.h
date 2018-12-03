/**
*	@file	PMDLoader.h
*	@brief	PMDLoaderの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/08/28	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include <memory>
#include "../ModelLoader.h"
#include "PMDModelData.h"

/**
*	@class	PMDLoader
*	@brief	PMDモデルの読込を管理する
*
*	PMDモデル読込を管理するクラス
*	読み込み済みデータのインデックスをこのクラスが保持し、
*	このクラスのデストラクタで登録を解除して
*	データを解放する
*/
class PMDLoader : public ModelLoader
{
public:
	/**
	*	@brief	デストラクタ
	*/
	~PMDLoader();

	/**
	*	@brief	PMDLoaderを作成する
	*
	*	@param[in]	device				: dx12デバイス
	*	@param[in]	shareToonFolderPath	: 共有Toonのフォルダパス
	*
	*	@retval		生成成功	: PMDLoaderのスマートポインタ
	*	@retval		生成失敗	: nullptr
	*/
	static std::shared_ptr<PMDLoader> Create(std::shared_ptr<Device> device, const std::string& shareToonFolderPath);

	/**
	*	@brief	モデルをロードする
	*
	*	@param[in]	filePath		: ファイルパス
	*	@param[in]	pso				: モデル描画用PSO
	*	@param[in]	shadowPSO		: シャドウマップ描画用PSO
	*	@param[in]	rootSignature	: ルートシグネチャ
	*/
	std::shared_ptr<Model> LoadModel(const std::string& filePath, std::shared_ptr<PipelineStateObject> pso,
		std::shared_ptr<PipelineStateObject> shadowPSO, std::shared_ptr<RootSignature> rootSignature);

	/**
	*	@brief	読み込んだモデル情報を削除する
	*/
	void ClearModelData();

private:
	/**
	*	@brief	コンストラクタ
	*/
	PMDLoader(std::shared_ptr<Device> device);

	/**
	*	@brief	コピーコンストラクタでのコピー禁止
	*/
	PMDLoader(const PMDLoader&);

	/**
	*	@brief	コピー演算子でのコピー禁止
	*/
	void operator=(const PMDLoader&) {}

	const int SHARE_TOON_NUM = 11;			//! 共有Toonの数
	const std::string SHARE_TOON_PATH[11]	//! 共有Toonの画像名
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
	};		

	std::vector<int> mShareToonTextureHandle;	//! 共有Toonテスクチャハンドル
	
	/**
	*	@brief	共有Toonをロードする
	*	
	*	@param[in]	共有Toonのあるフォルダパス
	*/
	void LoadShareToon(const std::string& toonFolderPath);

	/**
	*	@brief	Toonデータをロードする
	*
	*	@param[in]	fp					: ファイルポインタ
	*	@param[in]	toonTextureHandle	: 
	*/
	void LoadToonData(FILE *fp, const std::string& modelPath, std::vector<int>& toonTextureHandle);
};

