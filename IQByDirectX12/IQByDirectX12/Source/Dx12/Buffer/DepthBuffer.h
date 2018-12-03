/**
*	@file	DepthBuffer.h
*	@brief	DepthBufferの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/11/24	初版作成
*/
#pragma once
/* ヘッダインクルード */
#include "Dx12Resource.h"

/* クラス使用宣言 */
class DescriptorHeap;
class GraphicsCommandList;

/**
*	@class	DepthBuffer
*	@brief	深度バッファクラス
*	
*	深度バッファを格納するクラス
*	1バッファ1ヒープの仕組みを採用している
*	そのため、内部にバッファリソースのほか、ヒープを保持している
*	(いい設計思いついたら変えます)
*	
*	@note	初版作成時、ステンシルバッファの使用を想定していないため注意
*/
class DepthBuffer :
	public Dx12Resource
{
public:
	/**
	*	@brief	デストラクタ
	*/
	~DepthBuffer();

	/**
	*	@brief	デプスバッファを作成する
	*
	*	@param[in]	device			: dx12デバイス
	*	@param[in]	width			: バッファ横幅
	*	@param[in]	height			: バッファ縦幅
	*	@param[in]	isAlignment		: アラインメントを行うか(デフォルトでtrue)
	*	@param[in]	depthClearValue	: 深度クリア値(デフォルトで1.0f)
	*
	*	@retval	生成成功		: DepthBufferのスマートポインタ
	*	@retval	生成失敗		: nullptr
	*/
	static std::shared_ptr<DepthBuffer> Create(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const std::wstring& bufferName = L"DepthBuffer",
		bool isAlignment = true,
		float depthClearValue = 1.0f);

	/**
	*	@brief	深度バッファをクリアする
	*	@param[in]	commandList	: クリアコマンドを載せるコマンドリスト
	*/
	void ClearDepthBuffer(std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@brief	深度バッファへの書き込みを開始する
	*
	*	@note	書き込み開始時に初期化を行う
	*			また、コマンドリストへのセットは行わない
	*
	*	@param[in]	commandList	: 書き込み対象のコマンドリスト
	*/
	void BeginWriteDepth(std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@brief	深度バッファへの書き込みを終了する
	*	
	*	@param[in]	commandList	: 書き込み対象のコマンドリスト
	*/
	void EndWriteDepth(std::shared_ptr<GraphicsCommandList> commandList);

	/**
	*	@biref	デプスステンシルビュー情報を取得する
	*	@return	デプスステンシルビュー情報
	*/	
	const D3D12_DEPTH_STENCIL_VIEW_DESC& GetDSVDesc() const;

	/**
	*	@brief	デプスステンシルビューのCPUアドレスを取得する
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUHandle() const;
private:
	const D3D12_CLEAR_VALUE CLEAR_VALUE;

	std::shared_ptr<DescriptorHeap> mDepthDescHeap;	//! 深度バッファ用デスクリプタヒープ
	D3D12_DEPTH_STENCIL_VIEW_DESC mDSVDesc;			//! デプスステンシルビュー情報

	/**
	*	@brief	2の乗数にアラインメントした数値を返す
	*	@param[in]	アラインメントしたい数値データ
	*	@return		アラインメントされた数値データ
	*/
	static unsigned int Alignment(unsigned int num);

	/**
	*	@brief	DSVを構築する
	*	@param[in]	device	: dx12デバイス
	*/
	void ConstructDSV(std::shared_ptr<Device> device);

	/**
	*	@brief	コンストラクタ
	*/
	DepthBuffer(std::shared_ptr<Device> device,
		unsigned int width,
		unsigned int height,
		const D3D12_CLEAR_VALUE& clearValue,
		const std::wstring& bufferName,
		HRESULT& result
	);

};

