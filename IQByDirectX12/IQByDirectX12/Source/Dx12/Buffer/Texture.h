/**
*	@file	Texture.h
*	@biref	テクスチャクラスの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/23	初版作成
*			2018/11/27	テクスチャ情報の書き込みを一時リソースに行い、後で本リソースへアップデートする方法へ変更
*/
#pragma once
/* ヘッダインクルード */
#include "Dx12Resource.h"
#include <DirectXTex.h>
#include "../Math/Math.h"

/* クラス使用宣言 */
class DepthBuffer;
class RenderTargetBuffer;

/**
*	@class	Texture
*	@brief	リソースをテクスチャとして扱うクラス
*/
class Texture :
	public Dx12Resource
{
public:
	/**
	*	@brief	デストラクタ
	*/
	~Texture();

	/**
	*	@brief	テクスチャを作成する
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	metaData	: テクスチャメタデータ(DirectXTex)
	*	@param[in]	texImage	: テクスチャイメージデータ(DirectXTex)
	*	@param[in]	textureName	: テクスチャ名
	*	@retval		生成成功	: Textureのスマートポインタ
	*	@retval		生成失敗	: nullptr
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<Device> device, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& texImage, const std::wstring& textureName);

	/**
	*	@brief	空のテクスチャを作成する
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	width		: 画像横幅
	*	@param[in]	height		: 画像縦幅
	*	@param[in]	format		; 画像フォーマット
	*	@param[in]	textureName	: テクスチャ名
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<Device> device, unsigned int width, unsigned int height, DXGI_FORMAT format, const std::wstring& textureName = L"");

	/**
	*	@brief	テクスチャを作成する
	*
	*	@param[in]	depthBuffer	: デプスバッファ
	*	@return		Textureのスマートポインタ
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<DepthBuffer> depthBuffer);

	/**
	*	@brief	テクスチャを作成する
	*
	*	@param[in]	renderTargetBuffer	: レンダーターゲットバッファ
	*	@return		Textureのスマートポインタ
	*/
	static std::shared_ptr<Texture> Create(std::shared_ptr<RenderTargetBuffer> renderTargetBuffer);

	/**
	*	@biref	テクスチャ情報を書き込む
	*
	*	@param[in]	metaData	: メタデータ
	*	@param[in]	texImage	: テクスチャイメージ情報
	*
	*	@note		一時テクスチャに情報を書き込むだけであるため、
	*				元のテクスチャ情報は変更されない
	*				変更のためにはUpdateTextureメソッドを呼び出す必要があるため注意
	*/
	void WriteTextureData(const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& texImage);

	/**
	*	@brief	テクスチャ情報を書き込む
	*
	*	@param[in]	pImage			: イメージデータへのポインタ
	*	@param[in]	width			: 画像幅
	*	@param[in]	height			: 画像高さ
	*	@param[in]	pixelDataSize	: 1ピクセル当たりのデータサイズ
	*
	*	@note		一時テクスチャに情報を書き込むだけであるため、
	*				元のテクスチャ情報は変更されない
	*				変更のためにはUpdateTextureメソッドを呼び出す必要があるため注意
	*/
	void WriteTextureData(void* pImage, unsigned int width, unsigned int height, size_t pixelDataSize);

	/**
	*	@brief	SRV情報を取得する
	*/
	const D3D12_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceViewDesc() const;

	/**
	*	@brief	テクスチャ情報を更新する
	*
	*	@param[in]	コピーメソッドを載せるコマンドリスト
	*/
	void UpdateTexture(std::shared_ptr<GraphicsCommandList> commandList);

protected:
	D3D12_SHADER_RESOURCE_VIEW_DESC mShaderResourceViewDesc;	//! シェーダリソースビュー
	std::shared_ptr<Dx12Resource> mTemporaryResource;			//! データ転送用の一時リソース
	bool mIsUpdate;												//! テクスチャ情報の更新があったか
	const Math::Vector2 TEXTURE_SIZE;							//! テクスチャサイズを格納したボックスデータ
	unsigned int mRowPitch;										//! 書き込みデータのrowPitch

	/**
	*	@brief	SRV情報を構築する
	*	
	*	@note	リソースフォーマットが型なしの場合、同等のfloatフォーマットに変換する
	*/
	void ConstructSRVDesc();


	/**
	*	@biref		コンストラクタ
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	metaData	: テクスチャメタデータ
	*	@param[in]	textureName	: テクスチャ名
	*
	*	@param[out]	result		: リソースの作成結果
	*/
	Texture(std::shared_ptr<Device> device, const DirectX::TexMetadata& metaData, const std::wstring& textureName, HRESULT& result);

	/**
	*	@brief	コンストラクタ
	*
	*	@param[in]	device		: dx12デバイス
	*	@param[in]	width		: 画像幅
	*	@param[in]	height		; 画像高さ
	*	@param[in]	format		: 画像データフォーマット
	*	@param[in]	textureName	: テクスチャ名
	*
	*	@param[out]	result		: リソースの作成結果
	*	
	*	@note	ミップマップテクスチャの作成はできないため注意
	*/
	Texture(std::shared_ptr<Device> device, unsigned int width, unsigned int height, DXGI_FORMAT format, const std::wstring& textureName, HRESULT& result);

	/**
	*	@brief		コンストラクタ
	*
	*	@param[in]	resource	: 構築済みリソースデータ
	*/
	Texture(std::shared_ptr<Dx12Resource> resource);

};

