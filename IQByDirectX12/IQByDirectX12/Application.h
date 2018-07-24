/*
	Application.h
	アプリケーションのコア部分
	@author Ishibashi Ryuto
	@history
	2018/07/11 初版作成
*/
#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <vector>
#include <wrl.h>
#include <memory>

class Window;
class Device;
class CommandAllocator;
class CommandQueue;
class RenderTarget;
class DepthBuffer;
class VertexBuffer;

using Microsoft::WRL::ComPtr;

struct Vertex
{
	float position[3];
	float normal[3];
	float uv[2];
};

class Application
{
public:
	Application();
	~Application();

	/// @fn Initialize
	/// 初期化処理
	/// @param window ウィンドウクラス
	/// @retval 処理が成功したか
	/// true: 成功, false: 失敗
	bool Initialize(const Window& window);

	/// @fn Render
	/// 描画処理
	void Render();

private:
	/* 定数定義 */
	const int RENDER_TARGET_NUM = 2;

	/* 変数宣言 */
	ComPtr<IDXGIFactory4>				mFactory;
	ComPtr<IDXGISwapChain3>				mSwapChain;
	ComPtr<ID3D12RootSignature>			mRootSignature;
	ComPtr<ID3D12Fence>					mFence;
	ComPtr<ID3DBlob>					mVertexShader;
	ComPtr<ID3DBlob>					mPixelShader;
	ComPtr<ID3D12PipelineState>			mPipelineState;
	ComPtr<ID3D12GraphicsCommandList>	mCommandList;
	std::shared_ptr<Device>				mDevice;
	std::shared_ptr<CommandAllocator>	mCommandAllocator;
	std::shared_ptr<CommandQueue>		mCommandQueue;
	std::shared_ptr<RenderTarget>		mRenderTarget;
	std::shared_ptr<DepthBuffer>		mDepthBuffer;
	std::shared_ptr<VertexBuffer>		mVertexBuffer;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayoutDescs;

	D3D12_STATIC_SAMPLER_DESC mStaticSamplerDesc;
	int mWindowWidth;
	int mWindowHeight;
	int mFenceValue;

	/* ローカルメソッド定義 */

	/// @fn CreateSwapChain
	/// スワップチェインを生成する
	/// @retval true: 生成成功, false: 生成失敗
	bool CreateSwapChain(const Window& window);

	/// @fn CreateRootSignature
	/// ルートシグネチャを生成する
	/// @retval ture: 生成成功, false: 生成失敗
	bool CreateRootSignature();

	/// @fn CreateFence
	/// フェンスを生成する
	/// @retval true: 生成成功, false: 生成失敗
	bool CreateFence();

	/// @fn ReadShader
	/// シェーダを読み込む
	/// @retval true: 読込成功, false: 読込失敗
	bool ReadShader();

	/// @fn CreatePipelineState
	/// パイプラインステートの作成
	/// @retval ture: 生成成功, false: 生成失敗
	bool CreatePipelineState();

	/// @fn CreateCommandList
	/// コマンドリストの生成
	/// @retval true: 生成成功, false: 生成失敗
	bool CreateCommandList();
};

