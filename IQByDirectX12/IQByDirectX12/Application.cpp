#include <iostream>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_4.h>
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

	// DXGIFactoryの生成
	{
		auto result = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create DXGIFactory." << std::endl;
#endif
			return false;
		}
	}

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

	// コマンドアロケータの生成
	mCommandAllocator = CommandAllocator::Create(mDevice->GetDevice().Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	if (!mCommandAllocator)
	{
		return false;
	}

	// コマンドキューの生成
	mCommandQueue = CommandQueue::Create(mDevice->GetDevice());
	if (!mCommandQueue)
	{
		return false;
	}

	// スワップチェイン生成
	if (!CreateSwapChain(window))
	{
		return false;
	}

	// レンダーターゲット生成
	mRenderTarget = RenderTarget::Create(mDevice->GetDevice(), mSwapChain.Get(), RENDER_TARGET_NUM);
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

	// 静的サンプラの設定
	{
		mStaticSamplerDesc = D3D12_STATIC_SAMPLER_DESC();
		mStaticSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		mStaticSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		mStaticSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		mStaticSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		mStaticSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		mStaticSamplerDesc.MinLOD = 0.0f;
		mStaticSamplerDesc.MipLODBias = 0.0f;
		mStaticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		mStaticSamplerDesc.ShaderRegister = 0;
		mStaticSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		mStaticSamplerDesc.RegisterSpace = 0;
		mStaticSamplerDesc.MaxAnisotropy = 0;
		mStaticSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS;
	}

	// ルートシグニチャの作成
	if (!CreateRootSignature())
	{
		return false;
	}

	// シェーダの読み込み
	if (!ReadShader())
	{
		return false;
	}
	
	// パイプラインオブジェクトの作成
	if (!CreatePipelineState())
	{
		return false;
	}

	// コマンドリストの作成
	if (!CreateCommandList())
	{
		return false;
	}


	// 頂点バッファ作成
	Vertex tmp[6] = 
	{
		{ { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 1.0f } },
		{ { 0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } },
		{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }},
	};

	mVertexBuffer = VertexBuffer::Create(mDevice->GetDevice(), &tmp, _countof(tmp), sizeof(Vertex));

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

	return true;
}

void Application::Render()
{
	for (auto model : mInstancingTestModels)
	{
		model->Draw();
	}

	// コマンドリスト初期化
	mCommandAllocator->Get()->Reset();
	mCommandList->Reset(mCommandAllocator->Get().Get(), mPipelineState.Get());
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	// 描画範囲設定
	D3D12_VIEWPORT vp = { 0.0f,0.0f, (FLOAT)mWindowWidth, (FLOAT)mWindowHeight, 0.0f,1.0f };
	D3D12_RECT rc = { 0,0,mWindowWidth, mWindowHeight };
	mCommandList->RSSetViewports(1, &vp);
	mCommandList->RSSetScissorRects(1, &rc);

	// 描画先変更処理
	int backBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
	mRenderTarget->ChangeRenderTarget(mCommandList, backBufferIndex);
	auto rtvHandle = mRenderTarget->GetRTVHandle();
	auto dsvHandle = mDepthBuffer->GetDSVHandle();
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	mRenderTarget->ClearRenderTarget(mCommandList);
	mDepthBuffer->ClearDepthBuffer(mCommandList);

	//デスクリプタヒープバインド
	mDescriptorHeap->BindGraphicsCommandList(mCommandList);

	// ポリゴン描画
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

	// 板ポリ描画
	
	/*
	mCommandList->IASetVertexBuffers(0, 1, &mVertexBuffer->GetVertexBufferView());
	mCommandList->DrawInstanced(6, 1, 0, 0);
	*/
	// モデル描画
	/*
	mCommandList->IASetVertexBuffers(0, 1, &mPMXModelData->GetVertexBuffer()->GetVertexBufferView());
	mCommandList->IASetIndexBuffer(&mPMXModelData->GetIndexBuffer()->GetIndexBufferView());
	mCommandList->DrawIndexedInstanced(mPMXModelData->GetIndexBuffer()->GetIndexCount(), 1, 0, 0, 0);
	*/
	ModelDataManager::GetInstance().Draw(mCommandList);

	//描画終了処理
	mRenderTarget->FinishRendering(mCommandList);
	mCommandList->Close();

	// 描画コマンド実行
	ID3D12CommandList* commandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandList(_countof(commandLists), commandLists);

	mCommandQueue->Signal();

	// 画面スワップ
	mSwapChain->Present(1, 0);
}

bool Application::CreateSwapChain(const Window& window)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	auto windowRect = window.GetWindowRect();

	swapChainDesc.Width = windowRect.right - windowRect.left;
	swapChainDesc.Height = windowRect.bottom - windowRect.top;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = RENDER_TARGET_NUM;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;

	auto result = mFactory->CreateSwapChainForHwnd(mCommandQueue->GetCommandQueue().Get(),
		window.GetWindowHandle(),
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)(mSwapChain.GetAddressOf()));

	if (FAILED(result))
	{
#ifdef _DEBUG
		std::cout << "Failed Create SwapChain." << std::endl;
		return false;
#endif
	}
	return true;
}

bool Application::CreateRootSignature()
{
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	// ルートパラメータ
	std::vector<D3D12_ROOT_PARAMETER> rootParams;

	// CBV用ディスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE cbvRange;
	cbvRange.NumDescriptors = 1;
	cbvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	cbvRange.BaseShaderRegister = 0;
	cbvRange.RegisterSpace = 0;
	cbvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// SRV用ディスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE srvRange;
	srvRange.NumDescriptors = 1;
	srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRange.BaseShaderRegister = 0;
	srvRange.RegisterSpace = 0;
	srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam;
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.DescriptorTable = { 1, &cbvRange };
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_PARAMETER rootParam2;
	rootParam2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam2.DescriptorTable = { 1, &srvRange };
	rootParam2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParams.resize(2);
	rootParams[0] = rootParam;
	rootParams[1] = rootParam2;

	// ルートシグネチャの設定
	CD3DX12_ROOT_SIGNATURE_DESC rsd{};
	rsd.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rsd.NumParameters = (UINT)rootParams.size();
	rsd.pParameters = rootParams.data();
	rsd.NumStaticSamplers = 1;
	rsd.pStaticSamplers = &mStaticSamplerDesc;

	// シグネチャの作成
	{
		auto result = D3D12SerializeRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create Signautre." << std::endl;
#endif
			return false;
		}
	}

	// ルートシグネチャの作成
	{
		auto result = mDevice->GetDevice().Get()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature));
		if (FAILED(result))
		{
#ifdef _DEBUG
			std::cout << "Failed Create RootSignautre." << std::endl;
#endif
			return false;
		}
	}

	mRootSignatureClass = RootSignature::Create();
	int cbvIndex = mRootSignatureClass->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int srvIndex = mRootSignatureClass->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);

	mRootSignatureClass->AddDescriptorRange(cbvIndex, cbvRange);
	mRootSignatureClass->AddDescriptorRange(srvIndex, srvRange);

	

	return mRootSignatureClass->ConstructRootSignature(mDevice->GetDevice());;
}


bool Application::ReadShader()
{
	//ReadVertexShader
	{
		auto result = D3DCompileFromFile(L"3DPrimitiveShader.hlsl",
			nullptr,
			nullptr,
			"VSMain",
			"vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&mVertexShader,
			nullptr);
		if (FAILED(result))
		{
			std::cout << "Failed Compile Vertex Shader." << std::endl;
			return false;
		}
	}

	//ReadPixelShader
	{
		auto result = D3DCompileFromFile(L"3DPrimitiveShader.hlsl",
			nullptr,
			nullptr,
			"PSMain",
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&mPixelShader,
			nullptr);

		if (FAILED(result))
		{
			std::cout << "Failed Compile Pixel Shader." << std::endl;
			return false;
		}
	}
	return true;
}

bool Application::CreatePipelineState()
{
	// 頂点情報定義
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "NORMAL"			, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCORD"			, 0, DXGI_FORMAT_R32G32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	,	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	,	1,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	,	2,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	,	3,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.DepthStencilState.DepthEnable = TRUE;
	gpsDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpsDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpsDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpsDesc.DepthStencilState.StencilEnable = FALSE;
	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(mVertexShader.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(mPixelShader.Get());
	gpsDesc.InputLayout.NumElements = (UINT)mInputLayoutDescs.size();
	gpsDesc.InputLayout.pInputElementDescs = mInputLayoutDescs.data();
	gpsDesc.pRootSignature = mRootSignatureClass->GetRootSignature().Get();
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleMask = UINT_MAX;

	auto result = mDevice->GetDevice().Get()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	if (FAILED(result))
	{
#ifdef _DEBUG
		std::cout << "Failed Create PipelineObject." << std::endl;
#endif
		return false;
	}
	return true;
}

bool Application::CreateCommandList()
{
	auto result = mDevice->GetDevice().Get()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator->Get().Get(), mPipelineState.Get(), IID_PPV_ARGS(&mCommandList));
	if (FAILED(result))
	{
#ifdef _DEBUG
		std::cout << "Failed Create CommandList." << std::endl;
#endif
		return false;
	}

	result = mCommandList->Close(); 
	if (FAILED(result))
	{
#ifdef _DEBUG
		std::cout << "Failed Close CommandList." << std::endl;
#endif
		return false;
	}
	return true;
}

void Application::LoadTexture()
{
	mTextureLoader = TextureLoader::Create(mDevice->GetDevice());
	mTextureHandle = mTextureLoader->Load("Img/test.png");
	mDescriptorHeap = DescriptorHeap::Create(mDevice->GetDevice(), 2);
	mDescriptorHeap->SetTexture(TextureManager::GetInstance().GetTexture(mTextureHandle), 1);
}

bool Application::CreateConstantBuffer()
{
	mConstantBuffer = ConstantBuffer::Create(mDevice->GetDevice(), sizeof(DirectX::XMMATRIX), 1);
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
		mViewMatrix = Math::CreateLookAtMatrix(Math::Vector3(0.0f, 30.0f, -15.0f), Math::Vector3(0.0f, 10.0f, 50.0f), Math::Vector3(0.0f, 1.0f, 0.0f));
		mProjectionMatrix = Math::CreatePerspectiveMatrix((float)mWindowWidth / (float)mWindowHeight, 1.0f, 100.0f, Math::F_PI/2.0f);
		mAffineMatrix = (mWorldMatrix * mViewMatrix) * mProjectionMatrix;
		auto data = ConvertMatrix4x4ToXMMATRIX(mAffineMatrix);
		

		mConstantBuffer->SetData(&data, sizeof(DirectX::XMMATRIX), 0);
		mDescriptorHeap->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
	}
}

void Application::LoadPMD()
{
	mModelLoader = PMDLoader::Create(mDevice->GetDevice());
	mModelData = mModelLoader->LoadModel("Resource/Model/初音ミク.pmd");
	mModelData->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
}

void Application::LoadPMX()
{
	mPMXModelLoader = PMXLoader::Create(mDevice->GetDevice());
	mInstancingTestModels.resize(100);
	mPMXModelData = mPMXModelLoader->LoadModel("Resource/Model/フェネック/フェネック.pmx");
	mPMXModelData->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
	float x = -10.0f;
	float z = 0.0f;
	srand((unsigned int)time(0));
	for (auto &model : mInstancingTestModels)
	{
		model = mPMXModelLoader->LoadModel("Resource/Model/フェネック/フェネック.pmx");
		model->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
	}

	int modelCount = 0;
	for (int x = 0; z < 10; ++z)
	{
		for (int x = -5; x < 5; ++x)
		{
			mInstancingTestModels[modelCount]->SetPosition(Math::Vector3(x * 5.0f, 0.0f, z*5.0f));
			mInstancingTestModels[modelCount]->SetScale((float)(rand() % 10000) / 10000.0f + 0.2f);
			mInstancingTestModels[modelCount]->SetRotation(Math::Vector3(0.0f, (float)(rand() % 10000) / 10000.0f * Math::F_PI *2.0f, 0.0f));
			++modelCount;
		}
	}
}
