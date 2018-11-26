/*
	RenderTarget.h
	レンダーターゲットを管理するクラス
	@author Ishibashi Ryuto
	@history
	2018/07/11 初版作成
*/
#pragma once
/*システムヘッダインクルード*/
#include <vector>
#include <memory>
#include <string>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;
class Device;
class RenderTargetBuffer;
class Texture;
class GraphicsCommandList;

class RenderTarget
{
public:
	/// デストラクタ
	~RenderTarget();

	/// @fn Create
	/// レンダーターゲットを生成し、管理するRenderTargetクラスを返す
	/// @note RenderTargetクラスはこのメソッドでのみ生成可能
	/// @param[in] device:				ID3D12デバイス
	/// @param[in] swapChain:			IDXGIスワップチェイン
	/// @param[in] windowWidth;			ウィンドウ幅
	/// @param[in] windowHeight;		ウィンドウ高さ
	/// @param[in] renderTargetsNum:	レンダーターゲットの数
	/// @retval 生成成功時: RenderTargetクラスのshared_ptr, 生成失敗時: nullptr
	static std::shared_ptr<RenderTarget> Create(std::shared_ptr<Device> device, ComPtr<IDXGISwapChain1> swapChain, int renderTargetsNum);

	/// @fn ChangeRenderTarget
	/// レンダーターゲットを変更する
	void ChangeRenderTarget(std::shared_ptr<GraphicsCommandList> commandList, int targetIndex);

	/// @fn FinishRendering
	/// 描画処理を終了する
	void FinishRendering(std::shared_ptr<GraphicsCommandList> commandList);

	/// @fn GetRTVHandle
	/// レンダーターゲットビューのハンドルを取得する
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();

	/// @fn ClearRenderTarget
	void ClearRenderTarget(std::shared_ptr<GraphicsCommandList> commandList);

	/// @brief RenderTargetバッファを取得する
	std::shared_ptr<RenderTargetBuffer> GetRenderTargetBuffer(int targetIndex) const;

	/// @brief レンダーテクスチャを取得する
	std::shared_ptr<Texture> GetRenderTexture(int targetIndex) const;

protected:
	/* 定数定義 */
	const UINT RENDER_TARGET_VIEW_DESCRIPTOR_SIZE;

	/* 変数定義 */
	std::vector<std::shared_ptr<RenderTargetBuffer>> mRenderTargets;
	std::vector<std::shared_ptr<Texture>> mRenderTextures;
	ComPtr<ID3D12DescriptorHeap> mRTVDescHeap;
	int mRenderTargetIndex;

	/* ローカルメソッド */
	/// コンストラクタ
	/// @param[in] device ID3D12デバイス
	RenderTarget(std::shared_ptr<Device> device);

};

