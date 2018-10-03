/*
	DepthBuffer.h
	デプスバッファのリソースとDSVを管理するクラス
	(今後ステンシルバッファが必要になったらまた改訂します)
	@author Ishibashi Ryuto
	@history
	2018/07/12 初版作成
*/
#pragma once
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;

class DepthBuffer
{
public:
	/// デストラクタ
	~DepthBuffer();

	/// @fn Create
	/// デプスバッファとDSVを管理するクラスを生成する
	/// @note DepthBufferクラスはこのメソッドでのみ生成可能
	/// @param[in] device:			ID3D12デバイス
	/// @param[in] windowWidth:		ウィンドウ幅
	/// @param[in] windowHeight:	ウィンドウ高さ
	static std::shared_ptr<DepthBuffer> Create(ComPtr<ID3D12Device> device, int windowWidth, int windowHeight);

	/// @fn GetDSVHandle
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle();

	/// @fn ClearDepthBuffer
	void ClearDepthBuffer(ComPtr<ID3D12GraphicsCommandList> commandList);

private:
	/*定数定義*/
	const D3D12_CLEAR_VALUE DEPTH_CLEAR_VALUE;	// 深度クリアの際に使用する値
	
	/*変数宣言*/
	ComPtr<ID3D12Resource> mDepthBuffer;		// 深度バッファリソース
	ComPtr<ID3D12DescriptorHeap> mDSVDescHeap;	// 深度バッファデスクリプタヒープ
	D3D12_CPU_DESCRIPTOR_HANDLE mDSVCPUHandle;		// DSVのCPUハンドル

	/*ローカルメソッド定義*/
	/// コンストラクタ
	/// @param[in] depthClearValue: 深度バッファをクリアするパラメータ
	DepthBuffer(const D3D12_CLEAR_VALUE& depthClearValue);

};

