/*
	@file	RootSignature.h
	ルートシグネチャ
	@author Ishibashi Ryuto
	@date	2018/09/05	初版作成
			2018/11/29	バインドするDescriptorHeapを保持する設計に変更
*/
#pragma once
// ヘッダインクルード
#include<vector>
#include<memory>
#include<map>
#include<d3d12.h>
#include<wrl.h>

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class DescriptorHeap;
class GraphicsCommandList;

/**
*	@class	RootSignature
*	@brief	ルートシグネチャを管理する
*			生成に必要なパラメータをどんどん追加して、最後に構築する。
*			また、ルートパラメータにディスクリプタをバインドするのもこのクラス
*/
class RootSignature 
{
public:
	/// コンストラクタ
	RootSignature();

	/// デストラクタ
	~RootSignature();

	/**
	*	ルートシグネチャを生成する
	*	@note RootSignatureクラスの実体はこのメソッドでのみ生成可能
	*	@retval		生成成功	: RootSignatureのポインタ
	*	@retval		生成失敗	: nullptr
	*/
	static std::shared_ptr<RootSignature> Create();

	/**
	*	@brief	現在の設定項目でルートシグネチャを構築する
	*	@note	呼び出す度にRootSignatureを構築するため、重め。
	*			基本的には1回だけ呼び出せばいいようにしてください
	*	@param[in] device		: D3D12デバイス
	*	@retval	構築成功		: true
	*	@retval 構築失敗		: false
	*/
	bool ConstructRootSignature(ComPtr<ID3D12Device> device);

	/**
	*	@brief	ディスクリプタレンジを追加する
	*
	*	@param[in] rootParamIndex	: 追加対象ルートパラメータのインデックス
	*	@param[in] descriptorRange	: ディスクリプタレンジ
	*
	*	@note	存在しないルートパラメータを参照した場合、追加は行われないので注意
	*/
	void AddDescriptorRange(int rootParamIndex, const D3D12_DESCRIPTOR_RANGE & descriptorRange);

	/**
	*	@brief	ディスクリプタレンジを追加する
	*
	*	@param[in] rootParamIndex		: 追加対象ルートパラメータのインデックス
	*	@param[in] rangeType			: ディスクリプタの種類
	*	@param[in] descriptorNum		: 読み取るディスクリプタの数
	*	@param[in] baseShaderRegister	: シェーダレジスタの開始番号
	*
	*	@note	存在しないルートパラメータを参照した場合、追加は行われないので注意
	*/
	void AddDescriptorRange(int rootParamIndex,
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister);

	
	/**
	*	ルートパラメータを追加する
	*
	*	@param[in] shaderVisibility	: シェーダ可視性
	*	@param[in] parameterType	: パラメータタイプ(デフォルトでD3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
	*
	*	@note	このメソッドはあくまでルートパラメータを「追加」するだけで、
	*			具体的な設定は行いません。
	*			具体的なディスクリプタレンジはAddDescriptorRangeを使って追加してください
	*
	*	@return	ルートパラメータ番号
	*/
	int AddRootParameter(D3D12_SHADER_VISIBILITY shaderVisibility, D3D12_ROOT_PARAMETER_TYPE paramType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE);

	/**
	*	@brief	ルートシグネチャを取得する
	*
	*	@return	ルートシグネチャのComポインタ
	*/
	ComPtr<ID3D12RootSignature> GetRootSignature() const;

	/**
	*	@brief	ルートパラメータにバインドするデスクリプタヒープを設定する
	*
	*	@param[in]	rootParamIndex		: ルートパラメータのインデックス
	*	@param[in]	descHeap			: セットするデスクリプタヒープ
	*/
	void SetBindDescriptorHeap(unsigned int rootParamIndex, std::shared_ptr<DescriptorHeap> descHeap);

	/**
	*	@brief	ルートパラメータをコマンドリストにセットする
	*
	*	@param[in]	commandList		: ルートパラメータをセットするコマンドリスト
	*/
	void SetRootParameter(std::shared_ptr<GraphicsCommandList> commandList) const;

	/**
	*	@brief	ルートシグネチャをコマンドリストにセットする
	*/
	void SetRootSignature(std::shared_ptr<GraphicsCommandList> commandList) const;

private:
	D3D12_STATIC_SAMPLER_DESC mStaticSamplerDesc;							//! 静的サンプラ
	std::map<int ,std::vector<D3D12_DESCRIPTOR_RANGE> >	mDescriptorRanges;	//! ディスクリプタレンジ
	std::vector<D3D12_ROOT_PARAMETER> mRootParameters;						//! ルートパラメータ
	std::vector<std::shared_ptr<DescriptorHeap>> mDescriptorHeap;			//! ルートパラメータにバインドするデスクリプタヒープ
	ComPtr<ID3D12RootSignature> mRootSignature;								//! ルートシグネチャ群
};

