#include <iostream>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <D3DCompiler.h>

#include <time.h>

#include "Application.h"
#include "Window.h"
#include "Device.h"
#include "CommandQueue.h"
#include "CommandAllocator.h"
#include "RenderTarget.h"
#include "DepthBuffer.h"
#include "VertexBuffer.h"
#include "Texture/TextureLoader.h"
#include "Texture/Texture.h"
#include "DescriptorHeap.h"
#include "ConstantBuffer.h"
#include "MathConvert.h"
#include "Model/PMDLoader.h"
#include "Model/PMDModelData.h"
#include "Model/PMXLoader.h"
#include "Model/PMXModelData.h"
#include "Texture/TextureManager.h"
#include "Model/InstancingDataManager.h"
#include "RootSignature.h"
#include "Shader.h"
#include "VMDLoader.h"
#include "SwapChain.h"
#include "Animation.h"
#include "GraphicsCommandList.h"
#include "Input/Keyboard.h"
#include "Debug/DebugLayer.h"

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

	// コマンドキューの生成
	mCommandQueue = CommandQueue::Create(mDevice);
	if (!mCommandQueue)
	{
		return false;
	}

	// スワップチェイン生成
	mSwapChain = SwapChain::Create(mCommandQueue, window, RENDER_TARGET_NUM);
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
		mDepthBuffer = DepthBuffer::Create(mDevice->GetDevice().Get(), windowWidth, windowHeight);
		if (!mDepthBuffer)
		{
			return false;
		}
	}
	
	// ルートシグニチャの作成
	//if (!CreateRootSignature())
	if(!_DebugCreatePMDRootSignature())
	{
		return false;
	}

	// シェーダの読み込み
	//if (!ReadShader())
	if(!_DebugReadPMDShader())
	{
		return false;
	}
	
	// パイプラインオブジェクトの作成
	//if (!CreatePipelineState())
	if(!_DebugCreatePMDPipelineState())
	{
		return false;
	}

	// コマンドリストの作成
	if (!CreateCommandList())
	{
		return false;
	}

	// テクスチャ読み込み
	LoadTexture();

	// コンスタントバッファ作成
	if (!CreateConstantBuffer())
	{
		return false;
	}

	// 行列設定
	SetWVPMatrix();

	// インスタンシングデータマネージャにデバイスを登録
	InstancingDataManager::GetInstance().SetDevice(mDevice->GetDevice());

	// モデルデータ読込
	LoadPMD();
	LoadPMX();
	LoadMotion();

	return true;
}

void Application::Render()
{

	////debug
	static float t = 0;

	static Math::Quaternion rot = Math::CreateRotXYZQuaternion(Math::Vector3(0.f,0.f,0.f));
	static Math::Vector3 rotAxis(1.f, 0.f, 0.f);

	mKeyboard->UpdateKeyState();

	/*int i = 0;
	for (auto model : mInstancingTestModels)
	{
		if (i++ == 5)
		{
			model->SetRotation(rot);
			mAnimationData->SetPose(static_cast<int>(t), model->_DebugGetPose());
			model->Draw();
		}
	}*/

	if (mKeyboard->IsKeyDown(VirtualKeyIndex::A))
	{
		rot *= Math::CreateRotAxisQuaternion(Math::Vector3(0.f, 1.f, 0.f), 0.05f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::D))
	{
		rot *= Math::CreateRotAxisQuaternion(Math::Vector3(0.f, 1.f, 0.f), -0.05f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::W))
	{
		rot *= Math::CreateRotAxisQuaternion(rotAxis, 0.05f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::S))
	{
		rot *= Math::CreateRotAxisQuaternion(rotAxis, -0.05f);
	}


	t+=0.5f;
	if (t >= 30)
	{
		t = 0;
	}
	mModelData->SetPosition(Math::Vector3(0.0f,0.0f,0.0f));
	//mModelData->SetScale(sin(t / 30.f * Math::F_PI * 2) * 0.1f + 1.0f);
	mModelData->SetRotation(rot);
	mModelData->Draw();
	// endDebug

	// コマンドリスト初期化
	mCommandList->Reset(mPipelineState);
	(*mCommandList)->SetGraphicsRootSignature(mRootSignature->GetRootSignature().Get());

	// 描画範囲設定
	D3D12_VIEWPORT vp = { 0.0f,0.0f, (FLOAT)mWindowWidth, (FLOAT)mWindowHeight, 0.0f,1.0f };
	D3D12_RECT rc = { 0,0,mWindowWidth, mWindowHeight };
	(*mCommandList)->RSSetViewports(1, &vp);
	(*mCommandList)->RSSetScissorRects(1, &rc);

	// 描画先変更処理
	int backBufferIndex = mSwapChain->GetBackBufferIndex();
	
	mRenderTarget->ChangeRenderTarget(mCommandList->GetCommandList(), backBufferIndex);

	auto rtvHandle = mRenderTarget->GetRTVHandle();
	auto dsvHandle = mDepthBuffer->GetDSVHandle();
	
	(*mCommandList)->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	mRenderTarget->ClearRenderTarget(mCommandList->GetCommandList());
	mDepthBuffer->ClearDepthBuffer(mCommandList->GetCommandList());


	// ポリゴン描画
	(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// モデル描画
	ModelDataManager::GetInstance().Draw(mCommandList->GetCommandList());

	//描画終了処理
	mRenderTarget->FinishRendering(mCommandList->GetCommandList());
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

bool Application::CreateRootSignature()
{
	mRootSignature = RootSignature::Create();
	int cbvIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int materialIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int boneMatrixBufferIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);

	mRootSignature->AddDescriptorRange(cbvIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0);
	mRootSignature->AddDescriptorRange(boneMatrixBufferIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
	
	return mRootSignature->ConstructRootSignature(mDevice->GetDevice());
}

bool Application::_DebugCreatePMDRootSignature()
{
	mRootSignature = RootSignature::Create();
	int cbvIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int materialIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);

	mRootSignature->AddDescriptorRange(cbvIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	return mRootSignature->ConstructRootSignature(mDevice->GetDevice());
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

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	gpsDesc.DepthStencilState.DepthEnable = true;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = FALSE;

	gpsDesc.VS = mVertexShaderClass->GetShaderByteCode();
	gpsDesc.PS = mPixelShaderClass->GetShaderByteCode();
	gpsDesc.InputLayout.NumElements = (UINT)mInputLayoutDescs.size();
	gpsDesc.InputLayout.pInputElementDescs = mInputLayoutDescs.data();
	gpsDesc.pRootSignature = mRootSignature->GetRootSignature().Get();

	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleMask = UINT_MAX;

	auto result = (*mDevice)->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	if (FAILED(result))
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
		
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	gpsDesc.DepthStencilState.DepthEnable = true;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = FALSE;

	gpsDesc.VS = mVertexShaderClass->GetShaderByteCode();
	gpsDesc.PS = mPixelShaderClass->GetShaderByteCode();
	gpsDesc.InputLayout.NumElements = (UINT)mInputLayoutDescs.size();
	gpsDesc.InputLayout.pInputElementDescs = mInputLayoutDescs.data();
	gpsDesc.pRootSignature = mRootSignature->GetRootSignature().Get();

	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleMask = UINT_MAX;

	auto result = (*mDevice)->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	if (FAILED(result))
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
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

void Application::LoadTexture()
{
	mTextureLoader = TextureLoader::Create(mDevice);
	mDescriptorHeap = DescriptorHeap::Create(mDevice->GetDevice(), 2);
}

bool Application::CreateConstantBuffer()
{
	mConstantBuffer = ConstantBuffer::Create(mDevice, sizeof(DirectX::XMMATRIX)*4, 1);
	if (mConstantBuffer == nullptr)
	{
		return false;
	}
	return true;
}

void Application::SetWVPMatrix()
{
	if (mConstantBuffer != nullptr)
	{
		mWorldMatrix = Math::CreateIdent();
		mViewMatrix = Math::CreateLookAtMatrix(Math::Vector3(0.0f, 15.0f, -10.0f), Math::Vector3(0.0f, 10.0f, 0.0f), Math::Vector3(0.0f, 1.0f, 0.0f));
		mProjectionMatrix = Math::CreatePerspectiveMatrix((float)mWindowWidth / (float)mWindowHeight, 1.0f, 300.0f, Math::F_PI/2.0f);
		mAffineMatrix = (mWorldMatrix * mViewMatrix) * mProjectionMatrix;
		
		DirectX::XMMATRIX data[4];
		data[0] = ConvertMatrix4x4ToXMMATRIX(mAffineMatrix);
		data[1] = ConvertMatrix4x4ToXMMATRIX(mWorldMatrix);
		data[2] = ConvertMatrix4x4ToXMMATRIX(mViewMatrix);
		data[3] = ConvertMatrix4x4ToXMMATRIX(mProjectionMatrix);


		mConstantBuffer->SetData(data, sizeof(DirectX::XMMATRIX) * 4, 0);
		mDescriptorHeap->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
	}
}

void Application::LoadPMD()
{
	mModelLoader = PMDLoader::Create(mDevice);
	mModelData = mModelLoader->LoadModel("Resource/Model/初音ミク.pmd");
	mModelData->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);

}

void Application::LoadPMX()
{
	mPMXModelLoader = PMXLoader::Create(mDevice);
	mPMXModelData = mPMXModelLoader->LoadModel("Resource/Model/フェネック/フェネック.pmx");
	mPMXModelData->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);


	mInstancingTestModels.resize(100);
	float x = -10.0f;
	float z = 0.0f;
	srand((unsigned int)time(0));
	for (auto &model : mInstancingTestModels)
	{
		model = mPMXModelLoader->LoadModel("Resource/Model/Mirai_Akari_v1.0/MiraiAkari_v1.0.pmx");
		//model = mPMXModelLoader->LoadModel("Resource/Model/KizunaAI_ver1.01/kizunaai/kizunaai.pmx");
		//model = mPMXModelLoader->LoadModel("Resource/Model/フェネック/フェネック.pmx");
		model->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
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
	mAnimationData = loader->Load("Resource/Motion/応援ループモーション素材161025/01_ジャンプ手拍子01.vmd");
}
