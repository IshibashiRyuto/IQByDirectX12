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
#include "Math/Math.h"

class Window;
class Device;
class CommandAllocator;
class CommandQueue;
class RenderTarget;
class DepthBuffer;
class VertexBuffer;
class RootSignature;
class Texture;
class TextureLoader;
class DescriptorHeap;
class ConstantBuffer;
class PMDLoader;
class PMDModelData;
class PMXLoader;
class PMXModelData;
class Model;
class Shader;

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

	/// @fn Terminate
	/// 終了処理
	void Terminate();
private:
	/* 定数定義 */
	const int RENDER_TARGET_NUM = 2;

	/* 変数宣言 */
	ComPtr<IDXGIFactory4>				mFactory;
	ComPtr<IDXGISwapChain3>				mSwapChain;
	ComPtr<ID3DBlob>					mVertexShader;
	ComPtr<ID3DBlob>					mPixelShader;
	ComPtr<ID3D12PipelineState>			mPipelineState;
	ComPtr<ID3D12GraphicsCommandList>	mCommandList;
	std::shared_ptr<Device>				mDevice;
	std::shared_ptr<CommandAllocator>	mCommandAllocator;
	std::shared_ptr<CommandQueue>		mCommandQueue;
	std::shared_ptr<RenderTarget>		mRenderTarget;
	std::shared_ptr<RootSignature>		mRootSignature;
	std::shared_ptr<DepthBuffer>		mDepthBuffer;
	std::shared_ptr<VertexBuffer>		mVertexBuffer;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayoutDescs;
	std::shared_ptr<TextureLoader>		mTextureLoader;
	int									mTextureHandle;
	std::shared_ptr<DescriptorHeap>		mDescriptorHeap;
	std::shared_ptr<ConstantBuffer>		mConstantBuffer;
	std::shared_ptr<Shader>				mPixelShaderClass;
	std::shared_ptr<Shader>				mVertexShaderClass;
	std::shared_ptr<PMDLoader>			mModelLoader;
	std::shared_ptr<Model>				mModelData;
	std::shared_ptr<PMXLoader>			mPMXModelLoader;
	std::shared_ptr<Model>				mPMXModelData;
	std::vector<std::shared_ptr<Model>>	mInstancingTestModels;

	Math::Matrix4x4 mWorldMatrix;
	Math::Matrix4x4 mViewMatrix;
	Math::Matrix4x4 mProjectionMatrix;
	Math::Matrix4x4 mAffineMatrix;


	int mWindowWidth;
	int mWindowHeight;

	/* ローカルメソッド定義 */

	/// @fn CreateSwapChain
	/// スワップチェインを生成する
	/// @retval true: 生成成功, false: 生成失敗
	bool CreateSwapChain(const Window& window);

	/// @fn CreateRootSignature
	/// ルートシグネチャを生成する
	/// @retval ture: 生成成功, false: 生成失敗
	bool CreateRootSignature();

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

	/// @fn LoadTexture
	/// テクスチャロード
	void LoadTexture();

	/// @fn CreateConstantBuffer
	/// コンスタントバッファ生成
	/// @retval 生成成功: true
	/// @retval 生成失敗: false
	bool CreateConstantBuffer();

	/// @fn SetWVPMatrix
	/// WVP行列をコンスタントバッファにセットする
	void SetWVPMatrix();

	/// @fn LoadPMD
	/// PMDモデルデータをロードする
	void LoadPMD();

	/// @fn LoadPMX
	/// PMXモデルデータをロードする
	void LoadPMX();
};

