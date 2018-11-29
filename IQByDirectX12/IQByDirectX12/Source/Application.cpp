#include <iostream>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <D3DCompiler.h>

#include <time.h>

#include "Application.h"
#include "Window.h"
#include "Dx12/Device.h"
#include "Dx12/CommandQueue.h"
#include "Dx12/CommandAllocator.h"
#include "Dx12/RenderTarget.h"
#include "Dx12/Buffer/DepthBuffer.h"
#include "Dx12/Buffer/VertexBuffer.h"
#include "Texture/TextureLoader.h"
#include "Dx12/Buffer/Texture.h"
#include "Dx12/DescriptorHeap.h"
#include "Dx12/ConstantBuffer.h"
#include "Model/PMD/PMDLoader.h"
#include "Model/PMD/PMDModelData.h"
#include "Model/PMX/PMXLoader.h"
#include "Model/PMX/PMXModelData.h"
#include "Texture/TextureManager.h"
#include "InstancingDataManager.h"
#include "Dx12/RootSignature.h"
#include "Shader.h"
#include "Motion/VMDLoader.h"
#include "Dx12/SwapChain.h"
#include "Motion/Animation.h"
#include "Dx12/GraphicsCommandList.h"
#include "Input/Keyboard.h"
#include "Debug/DebugLayer.h"
#include "Camera/Camera.h"
#include "Camera/Dx12Camera.h"
#include "Dx12/PipelineStateObject.h"
#include "Dx12/Buffer/RenderTargetBuffer.h"
#include "Dx12/RenderState.h"
#include "Dx12/ShaderList.h"
#include "Sprite/Sprite.h"
#include "Sprite/SpriteDataManager.h"

// ライブラリリンク
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dCompiler.lib")

Application::Application()
{
}


Application::~Application()
{
}

bool Application::Initialize(const Window & window)
{
	auto wrc = window.GetWindowRect();
	mWindowWidth = wrc.right - wrc.left;
	mWindowHeight = wrc.bottom - wrc.top;

	mKeyboard = Keyboard::Create();

#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug> debugController;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
		if (SUCCEEDED(result))
		{
			debugController->EnableDebugLayer();
		}
		debugController = nullptr;
	}
#endif

	// デバイスの生成
	mDevice = Device::Create();
	if (!mDevice)
	{
		return false;
	}

	// テクスチャマネージャの初期化
	TextureManager::GetInstance().Initialize(mDevice);

	// コマンドキューの生成
	mCommandQueue = CommandQueue::Create(mDevice);
	if (!mCommandQueue)
	{
		return false;
	}

	// スワップチェイン生成
	mSwapChain = SwapChain::Create(mCommandQueue, window);
	if (!mSwapChain)
	{
		return false;
	}

	// レンダーターゲット生成
	mRenderTarget = RenderTarget::Create(mDevice, mSwapChain->GetSwapChain(), RENDER_TARGET_NUM);
	if (!mRenderTarget)
	{
		return false;
	}

	// 深度バッファの作成
	{
		auto windowRect = window.GetWindowRect();
		int windowWidth = windowRect.right - windowRect.left;
		int windowHeight = windowRect.bottom - windowRect.top;
		mDepthBuffer = DepthBuffer::Create(mDevice, windowWidth, windowHeight, L"RenderDepthBuffer", false);
		if (!mDepthBuffer)
		{
			return false;
		}
	}
	
	// PMDパイプラインステートオブジェクトの作成
	if (!CreatePMDPipelineStateObject())
	{
		return false;
	}

	// PMXパイプラインステートオブジェクトの作成
	if (!CreatePMXPipelineStateObject())
	{
		return false;
	}

	// ペラポリ用パイプラインステートオブジェクトの作成
	if (!CreatePeraPipelineStateObject())
	{
		return false;
	}

	if (!CreateSpritePipelineStateObject())
	{
		return false;
	}

	// コマンドリストの作成
	if (!CreateCommandList())
	{
		return false;
	}


	// カメラの作成
	CreateCamera();

	// インスタンシングデータマネージャにデバイスを登録
	InstancingDataManager::GetInstance().SetDevice(mDevice);

	// モデルデータ読込
	LoadPMD();
	LoadPMX();
	LoadMotion();

	// ペラポリ用データ作成
	_DebugCreatePeraPolyData();

	// スプライトデータ作成

	if (!_DebugCreateSprite())
	{
		return false;
	}


	if (!CreateDirectionalLight())
	{
		return false;
	}

	//	テクスチャ情報の更新する
	TextureManager::GetInstance().UpdateTextureData();

	return true;
}

void Application::Render()
{
	UpdateMatrix();

	////debug
	static float t = 0;

	static Math::Quaternion rot = Math::CreateRotXYZQuaternion(Math::Vector3(0.f,0.f,0.f));
	static Math::Vector3 rotAxis(1.f, 0.f, 0.f);
	static Math::Vector3 pos(0.0f, 0.0f, 0.0f);
	static float speed = 0.05f;

	mKeyboard->UpdateKeyState();



	if (mKeyboard->IsKeyDown(VirtualKeyIndex::W))
	{
		rot *= Math::CreateRotAxisQuaternion(rotAxis, 0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::S))
	{
		rot *= Math::CreateRotAxisQuaternion(rotAxis, -0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::A))
	{
		rot *= Math::CreateRotAxisQuaternion(Math::Vector3(0.f, 1.f, 0.f), 0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::D))
	{
		rot *= Math::CreateRotAxisQuaternion(Math::Vector3(0.f, 1.f, 0.f), -0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad8))
	{
		pos.y += speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad2))
	{
		pos.y -= speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad4))
	{
		pos.x -= speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad6))
	{
		pos.x += speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad7))
	{
		pos.z -= speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad9))
	{
		pos.z += speed;
	}

	static bool isStop = true;
	
	if (mKeyboard->IsKeyTrigger(VirtualKeyIndex::Z))
	{
		isStop = !isStop;
	}

	if (!isStop)
	{
		
		t += 0.5f;
		if (t > mAnimationData->GetDuration())
		{
			t = 0;
		}
	}
	int i = 0;
	// PMX Instance Draw
	for (auto model : mInstancingTestModels)
	{
		if (i++ == 5)
		{
			model->SetRotation(rot);
			model->SetPosition(pos);
			mAnimationData->SetPose(static_cast<int>(t), model->_DebugGetPose());
			//mAnimationData->SetPose(50, model->_DebugGetPose());
			model->Draw();
		}
	}

	// PMD Draw
	mModelData->SetRotation(rot);
	mModelData->SetPosition(pos + Math::Vector3(10.0f, 0.0f,0.0f));
	//mAnimationData->SetPose(t, mModelData->_DebugGetPose());
	mModelData->Draw();
	// endDebug

	mSprite->Draw();

	// コマンドリスト初期化
	mCommandList->Reset();
	(*mCommandList)->SetGraphicsRootSignature(mRootSignature->GetRootSignature().Get());



	/* シャドウマップ生成 */
	D3D12_VIEWPORT svp = { 0.0f,0.0f, (FLOAT)1024, (FLOAT)1024, 0.0f,1.0f };
	D3D12_RECT src = { 0,0,1024, 1024 };

	(*mCommandList)->RSSetViewports(1, &svp);
	(*mCommandList)->RSSetScissorRects(1, &src);


	// 描画範囲設定
	D3D12_VIEWPORT vp = { 0.0f,0.0f, (FLOAT)mWindowWidth, (FLOAT)mWindowHeight, 0.0f,1.0f };
	D3D12_RECT rc = { 0,0,mWindowWidth, mWindowHeight };
	(*mCommandList)->RSSetViewports(1, &vp);
	(*mCommandList)->RSSetScissorRects(1, &rc);

	/* 1パス目描画 */

	// 描画先変更処理
	//int backBuffer = mSwapChain->GetBackBufferIndex();
	mRenderTarget->ChangeRenderTarget(mCommandList, 2);

	auto rtvHandle = mRenderTarget->GetRTVHandle();
	auto dsvHandle = mDepthBuffer->GetDSVCPUHandle();
	
	(*mCommandList)->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	mRenderTarget->ClearRenderTarget(mCommandList);
	mDepthBuffer->BeginWriteDepth(mCommandList);


	// トポロジセット
	(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// カメラセット
	mDx12Camera->SetCameraData(mCommandList, 0);

	// モデル描画
	ModelDataManager::GetInstance().Draw(mCommandList->GetCommandList());

	//　1パス目描画終了処理
	mRenderTarget->FinishRendering(mCommandList);
	mDepthBuffer->EndWriteDepth(mCommandList);


	/* 1パス目描画終了 */
	//デプスバッファをデプス書き込み状態から読み取り状態に移行
	/*
	(*mCommandList)->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition( mDepthBuffer->GetDepthBufferResource().Get(),
		D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_DEPTH_READ));*/
	
	/* 2パス目描画 */

	// 描画先変更
	int backBuffer = mSwapChain->GetBackBufferIndex();
	mRenderTarget->ChangeRenderTarget(mCommandList, backBuffer);

	rtvHandle = mRenderTarget->GetRTVHandle();

	(*mCommandList)->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	mRenderTarget->ClearRenderTarget(mCommandList);

	// パイプライン、ルートシグネチャ変更
	(*mCommandList)->SetPipelineState(mPeraPipelineState->GetPipelineStateObject().Get());
	(*mCommandList)->SetGraphicsRootSignature(mPeraRootSignature->GetRootSignature().Get());

	//トポロジセット
	(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// データセット
	mPeraDescHeap->BindGraphicsCommandList(mCommandList->GetCommandList());
	mPeraDescHeap->BindRootDescriptorTable(0, 0);
	mPeraDescHeap->BindRootDescriptorTable(1, 1);
	
	// ペラポリの描画
	(*mCommandList)->IASetVertexBuffers(0, 1, &mPeraVert->GetVertexBufferView());
	(*mCommandList)->DrawInstanced(4, 1, 0, 0);

	// スプライトの描画
	//SpriteDataManager::GetInstance().Draw(mCommandList);


	//　2パス目描画終了処理
	mRenderTarget->FinishRendering(mCommandList);

	/* 2パス目描画終了 */

	//デプスバッファをデプス読み取り状態から書き込み状態に移行
	
	/*(*mCommandList)->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(mDepthBuffer->GetDepthBufferResource().Get(),
		D3D12_RESOURCE_STATE_DEPTH_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));*/

	//描画終了処理
	mCommandList->Close();

	// 描画コマンド実行
	ID3D12CommandList* commandLists[] = { mCommandList->GetCommandList().Get() };
	mCommandQueue->ExecuteCommandList(_countof(commandLists), commandLists);

	mCommandQueue->Signal();

	// 画面スワップ
	mSwapChain->Swap();
}

void Application::Terminate()
{
}

bool Application::CreatePMDPipelineStateObject()
{
	if (!_DebugCreatePMDRootSignature())
	{
		return false;
	}

	if (!_DebugReadPMDShader())
	{
		return false;
	}

	return _DebugCreatePMDPipelineState();
}

bool Application::CreatePMXPipelineStateObject()
{
	if (!CreateRootSignature())
	{
		return false;
	}
	if (!ReadShader())
	{
		return false;
	}

	return CreatePipelineState();
}

bool Application::CreatePeraPipelineStateObject()
{
	if (!CreatePeraRootSignature())
	{
		return false;
	}
	if (!ReadPeraShader())
	{
		return false;
	}
	return CreatePeraPipelineState();
}

bool Application::CreateSpritePipelineStateObject()
{
	if (!CreatePeraRootSignature())
	{
		return false;
	}
	if (!ReadSpriteShader())
	{
		return false;
	}
	return CreateSpritePipelineState();
}

bool Application::CreateRootSignature()
{
	mRootSignature = RootSignature::Create();
	int cbvIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int materialIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int boneMatrixBufferIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);

	mRootSignature->AddDescriptorRange(cbvIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);
	mRootSignature->AddDescriptorRange(boneMatrixBufferIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
	
	return mRootSignature->ConstructRootSignature(mDevice->GetDevice());
}

bool Application::_DebugCreatePMDRootSignature()
{
	mRootSignature = RootSignature::Create();
	int cbvIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int materialIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int boneIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	mRootSignature->AddDescriptorRange(cbvIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);
	mRootSignature->AddDescriptorRange(boneIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);

	return mRootSignature->ConstructRootSignature(mDevice->GetDevice());
}

bool Application::CreatePeraRootSignature()
{
	mPeraRootSignature = RootSignature::Create();
	int idx = mPeraRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_PIXEL);
	int idx2 = mPeraRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_PIXEL);
	mPeraRootSignature->AddDescriptorRange(idx, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	mPeraRootSignature->AddDescriptorRange(idx2, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	return mPeraRootSignature->ConstructRootSignature(mDevice->GetDevice());
}


bool Application::ReadShader()
{
	mVertexShaderClass = Shader::Create(L"Resource/Shader/PMXModelShader.hlsl", "VSMain", "vs_5_0");

	mPixelShaderClass = Shader::Create(L"Resource/Shader/PMXModelShader.hlsl", "PSMain", "ps_5_0");

	return (mVertexShaderClass && mPixelShaderClass);
}

bool Application::_DebugReadPMDShader()
{
	mVertexShaderClass = Shader::Create(L"Resource/Shader/PMDModelShader.hlsl", "VSMain", "vs_5_0");

	mPixelShaderClass = Shader::Create(L"Resource/Shader/PMDModelShader.hlsl", "PSMain", "ps_5_0");



	if (! (mVertexShaderClass && mPixelShaderClass) )
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Read Shader.\n");
		return false;
	}

	return true;
}

bool Application::ReadPeraShader()
{
	mVertexShaderClass = Shader::Create(L"Resource/Shader/pera.hlsl", "VSMain", "vs_5_0");

	mPixelShaderClass = Shader::Create(L"Resource/Shader/Distortion.hlsl", "PSMain", "ps_5_0");


	if (!mVertexShaderClass || !mPixelShaderClass)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Read Shader.\n");
		return false;
	}

	return true;
}

bool Application::ReadSpriteShader()
{
	mVertexShaderClass = Shader::Create(L"Resource/Shader/Sprite.hlsl", "VSMain", "vs_5_0");

	mPixelShaderClass = Shader::Create(L"Resource/Shader/Sprite.hlsl", "PSMain", "ps_5_0");


	if (!mVertexShaderClass || !mPixelShaderClass)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Read Shader.\n");
		return false;
	}

	return true;
}

bool Application::CreatePipelineState()
{
	// 頂点情報定義
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "APPEND_UV"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "APPEND_UV"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "APPEND_UV"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "APPEND_UV"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "DEFORM_TYPE"	, 0, DXGI_FORMAT_R32_UINT			, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "BONE_INDEX"	, 0, DXGI_FORMAT_R32G32B32A32_UINT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "BONE_WEIGHT"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
	}

	ShaderList shaderList;
	shaderList.VS = mVertexShaderClass;
	shaderList.PS = mPixelShaderClass;
	
	RenderState renderState;
	renderState.alphaBlendType = AlphaBlendType::Blend;
	renderState.depthTest = true;
	renderState.depthWrite = true;
	renderState.cullingType = CullingType::Double;
	renderState.depthFunc = D3D12_COMPARISON_FUNC_LESS;

	mPMXPipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mRootSignature, renderState, shaderList);
	if (!mPMXPipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
}

bool Application::_DebugCreatePMDPipelineState()
{
	// 頂点情報定義
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "BONENO"		, 0, DXGI_FORMAT_R16G16_UINT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "WEIGHT"		, 0, DXGI_FORMAT_R8_UINT			, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
	}

	ShaderList shaderList;

	shaderList.VS = mVertexShaderClass;
	shaderList.PS = mPixelShaderClass;
	
	RenderState renderState;
	renderState.alphaBlendType = AlphaBlendType::Opacity;
	renderState.depthTest = true;
	renderState.depthWrite = true;
	renderState.cullingType = CullingType::Double;
	renderState.depthFunc = D3D12_COMPARISON_FUNC_LESS;

	mPMDPipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mRootSignature,renderState, shaderList);
	if (!mPMDPipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
}

bool Application::CreatePeraPipelineState()
{
	// 頂点情報定義
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}

	ShaderList shaderList;
	shaderList.VS = mVertexShaderClass;
	shaderList.PS = mPixelShaderClass;
	
	RenderState renderState;
	renderState.alphaBlendType = AlphaBlendType::Blend;
	renderState.cullingType = CullingType::Front;
	renderState.depthFunc = D3D12_COMPARISON_FUNC_LESS;
	renderState.depthTest = false;
	renderState.depthWrite = false;

	mPeraPipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mPeraRootSignature, renderState, shaderList);
	if (!mPeraPipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
}

bool Application::CreateSpritePipelineState()
{// 頂点情報定義
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 0, DXGI_FORMAT_R32G32B32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 1, DXGI_FORMAT_R32G32B32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 2, DXGI_FORMAT_R32G32B32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
	}

	ShaderList shaderList;
	shaderList.VS = mVertexShaderClass;
	shaderList.PS = mPixelShaderClass;

	RenderState renderState;
	renderState.alphaBlendType = AlphaBlendType::Opacity;
	renderState.cullingType = CullingType::Double;
	renderState.depthFunc = D3D12_COMPARISON_FUNC_LESS;
	renderState.depthTest = false;
	renderState.depthWrite = false;

	mSpritePipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mPeraRootSignature, renderState, shaderList);
	if (!mSpritePipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}

	return true;
}

bool Application::CreatePrimitivePipelineState()
{
	return false;
}

bool Application::CreateCommandList()
{
	mCommandList = GraphicsCommandList::Create(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT, L"DefaultCommandList");
	if (!mCommandList)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create CommandList.\n");
		return false;
	}
	mCommandList->Close();
	return true;
}

void Application::CreateCamera()
{
	/// 射影行列のパラメータ設定
	ProjectionParam projParam;
	projParam.width = (float)mWindowWidth;
	projParam.height = (float)mWindowHeight;
	projParam.nearZ = 1.0f;
	projParam.farZ = 300.0f;
	projParam.fov = Math::F_PI / 8.0f;

	mDx12Camera = Dx12Camera::Create(Math::Vector3(0.f,30.f,-50.f),
		Math::Vector3(0.0f, 10.0f, 0.0f) - Math::Vector3(0.0f, 30.0f, -50.0f),
		ProjectionType::Perspective,
		projParam, mDevice);
}

void Application::LoadPMD()
{
	mModelLoader = PMDLoader::Create(mDevice, "Resource/Model/Toon");
	//mModelData = mModelLoader->LoadModel("Resource/Model/博麗霊夢/reimu_G02.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/初音ミク.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/我那覇響v1.0/我那覇響v1.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/MMD_Default/初音ミクmetal.pmd", mPMDPipelineState);
	mModelData = mModelLoader->LoadModel("Resource/Model/hibari/雲雀Ver1.10.pmd", mPMDPipelineState);
	if (!mModelData)
	{
		return;
	}

}

void Application::LoadPMX()
{
	mPMXModelLoader = PMXLoader::Create(mDevice, "Resource/Model/Toon");


	mInstancingTestModels.resize(100);
	float x = -10.0f;
	float z = 0.0f;
	srand((unsigned int)time(0));
	for (auto &model : mInstancingTestModels)
	{
		model = mPMXModelLoader->LoadModel("Resource/Model/Mirai_Akari_v1.0/MiraiAkari_v1.0.pmx", mPMXPipelineState);
		//model = mPMXModelLoader->LoadModel("Resource/Model/KizunaAI_ver1.01/kizunaai/kizunaai.pmx", mPMXPipelineState);
		//model = mPMXModelLoader->LoadModel("Resource/Model/フェネック/フェネック.pmx", mPMXPipelineState);
		//model = mPMXModelLoader->LoadModel("Resource/Model/TokinoSora_mmd_v.1.3/TokinoSora_2017.pmx", mPMXPipelineState);
	}

	int modelCount = 0;
	for (int x = 0; z < 10; ++z)
	{
		for (int x = -5; x < 5; ++x)
		{
			mInstancingTestModels[modelCount]->SetPosition(Math::Vector3(x * 15.0f, 0.0f, z*15.0f));
			++modelCount;
		}
	}
}

void Application::LoadMotion()
{
	auto loader = VMDLoader::Create();
	//mAnimationData =loader->Load("Resource/Motion/バイバイ.vmd");
	//mAnimationData =loader->Load("Resource/Motion/腕捻り.vmd");
	//mAnimationData =loader->Load("Resource/Motion/応援ループモーション素材161025/10_チョコレートディスコっぽい.vmd");
	//mAnimationData = loader->Load("Resource/Motion/応援ループモーション素材161025/01_ジャンプ手拍子01.vmd");
	mAnimationData = loader->Load("Resource/Model/博麗霊夢/モーション/ヤゴコロダンス.vmd");
	//mAnimationData = loader->Load("Resource/Motion/swing2.vmd");
	//mAnimationData = loader->Load("Resource/Motion/charge.vmd");
}

void Application::UpdateMatrix()
{
	//mDx12Camera->Rotate(Math::CreateRotXYZQuaternion(Math::Vector3(0.0f, Math::F_PI / 60.0f, 0.0f)));
	mDx12Camera->UpdateMatrix();
}

void Application::_DebugCreatePeraPolyData()
{
	PeraVertex verts[4] = 
	{
		{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f } },
		{ {  1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 1.0f } },
		{ {  1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f } },
	};

	mPeraVert = VertexBuffer::Create(mDevice, verts, sizeof(PeraVertex), 4);

	mPeraDescHeap = DescriptorHeap::Create(mDevice, 2);
	auto rtTexture = mRenderTarget->GetRenderTexture(2);
	mPeraDescHeap->SetTexture(rtTexture, 0);
	mDepthTexture = Texture::Create(mDepthBuffer);
	mPeraDescHeap->SetTexture(mDepthTexture, 1);
}

bool Application::_DebugCreateSprite()
{
	Sprite::SetPipelineStateObject(mSpritePipelineState);
	mTextureLoader = TextureLoader::Create(mDevice);
	if (!mTextureLoader)
	{
		return false;
	}
	mSprite = Sprite::Create(mDevice, TextureManager::GetInstance().GetTexture(mTextureLoader->Load("Resource/Texture/Eevee.png")));
	if (!mSprite)
	{
		return false;
	}

	return true;
}

bool Application::CreateDirectionalLight()
{
	ProjectionParam projParam;
	projParam.nearZ = 0.03f;
	projParam.farZ = 1000.f;
	projParam.width = 512.f;
	projParam.height = 512.f;
	mDirectionalLight = Dx12Camera::Create(Math::Vector3(50.f, 50.f, -50.f), Math::Vector3(-0.41f, -0.82f, 0.41f), ProjectionType::Orthographic, projParam, mDevice);
	if (!mDirectionalLight)
	{
		false;
	}

	mDirectionalLight->UpdateMatrix();

	return true;
}

void Application::CreatePrimitive()
{
}
