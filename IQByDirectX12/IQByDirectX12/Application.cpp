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
#include "Shader.h"

// ���C�u���������N
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

	// DXGIFactory�̐���
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

	// �f�o�C�X�̐���
	mDevice = Device::Create();
	if (!mDevice)
	{
		return false;
	}

	// �R�}���h�A���P�[�^�̐���
	mCommandAllocator = CommandAllocator::Create(mDevice->GetDevice().Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	if (!mCommandAllocator)
	{
		return false;
	}

	// �R�}���h�L���[�̐���
	mCommandQueue = CommandQueue::Create(mDevice->GetDevice());
	if (!mCommandQueue)
	{
		return false;
	}

	// �X���b�v�`�F�C������
	if (!CreateSwapChain(window))
	{
		return false;
	}

	// �����_�[�^�[�Q�b�g����
	mRenderTarget = RenderTarget::Create(mDevice->GetDevice(), mSwapChain.Get(), RENDER_TARGET_NUM);
	if (!mRenderTarget)
	{
		return false;
	}

	// �[�x�o�b�t�@�̍쐬
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

	// �ÓI�T���v���̐ݒ�
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

	// ���[�g�V�O�j�`���̍쐬
	if (!CreateRootSignature())
	{
		return false;
	}

	// �V�F�[�_�̓ǂݍ���
	if (!ReadShader())
	{
		return false;
	}
	
	// �p�C�v���C���I�u�W�F�N�g�̍쐬
	if (!CreatePipelineState())
	{
		return false;
	}

	// �R�}���h���X�g�̍쐬
	if (!CreateCommandList())
	{
		return false;
	}

	// �e�N�X�`���ǂݍ���
	LoadTexture();

	// �R���X�^���g�o�b�t�@�쐬
	if (!CreateConstantBuffer())
	{
		return false;
	}

	// �s��ݒ�
	SetWVPMatrix();

	// �C���X�^���V���O�f�[�^�}�l�[�W���Ƀf�o�C�X��o�^
	InstancingDataManager::GetInstance().SetDevice(mDevice->GetDevice());

	// ���f���f�[�^�Ǎ�
	LoadPMD();
	LoadPMX();

	return true;
}

void Application::Render()
{
	//debug
	static float zAngle = 0.0f;
	
	for (auto model : mInstancingTestModels)
	{
		model->SetRotation(Math::Vector3(0.0f, zAngle, 0.0f));
		model->Draw();
	}

	zAngle += 0.01f;

	mModelData->Draw();

	// �R�}���h���X�g������
	mCommandAllocator->Get()->Reset();
	mCommandList->Reset(mCommandAllocator->Get().Get(), mPipelineState.Get());
	mCommandList->SetGraphicsRootSignature(mRootSignature->GetRootSignature().Get());

	// �`��͈͐ݒ�
	D3D12_VIEWPORT vp = { 0.0f,0.0f, (FLOAT)mWindowWidth, (FLOAT)mWindowHeight, 0.0f,1.0f };
	D3D12_RECT rc = { 0,0,mWindowWidth, mWindowHeight };
	mCommandList->RSSetViewports(1, &vp);
	mCommandList->RSSetScissorRects(1, &rc);

	// �`���ύX����
	int backBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
	
	mRenderTarget->ChangeRenderTarget(mCommandList, backBufferIndex);

	auto rtvHandle = mRenderTarget->GetRTVHandle();
	auto dsvHandle = mDepthBuffer->GetDSVHandle();
	
	mCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	mRenderTarget->ClearRenderTarget(mCommandList);
	mDepthBuffer->ClearDepthBuffer(mCommandList);


	// �|���S���`��
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// ���f���`��
	ModelDataManager::GetInstance().Draw(mCommandList);

	//�`��I������
	mRenderTarget->FinishRendering(mCommandList);
	mCommandList->Close();

	// �`��R�}���h���s
	ID3D12CommandList* commandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandList(_countof(commandLists), commandLists);

	mCommandQueue->Signal();

	// ��ʃX���b�v
	mSwapChain->Present(1, 0);
}

void Application::Terminate()
{
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
	mRootSignature = RootSignature::Create();
	int cbvIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int materialIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);

	mRootSignature->AddDescriptorRange(cbvIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0);
	
	return mRootSignature->ConstructRootSignature(mDevice->GetDevice());
}


bool Application::ReadShader()
{
	mVertexShaderClass = Shader::Create(L"PMXModelShader.hlsl", "VSMain", "vs_5_0");

	mPixelShaderClass = Shader::Create(L"PMXModelShader.hlsl", "PSMain", "ps_5_0");

	return true;
}

bool Application::CreatePipelineState()
{
	// ���_����`
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "NORMAL"			, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "INSTANCE_MATRIX"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1,	D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 });
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	gpsDesc.DepthStencilState.DepthEnable = TRUE;
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

	auto device = mDevice->GetDevice().Get();
	auto result = device->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
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
	mConstantBuffer = ConstantBuffer::Create(mDevice->GetDevice(), sizeof(DirectX::XMMATRIX)*4, 1);
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
		mViewMatrix = Math::CreateLookAtMatrix(Math::Vector3(0.0f, 10.0f, -15.0f), Math::Vector3(0.0f, 10.0f, 50.0f), Math::Vector3(0.0f, 1.0f, 0.0f));
		mProjectionMatrix = Math::CreatePerspectiveMatrix((float)mWindowWidth / (float)mWindowHeight, 1.0f, 300.0f, Math::F_PI/2.0f);
		mAffineMatrix = (mWorldMatrix * mViewMatrix) * mProjectionMatrix;
		
		DirectX::XMMATRIX data[4];
		data[0] = ConvertMatrix4x4ToXMMATRIX(mAffineMatrix);
		data[1] = ConvertMatrix4x4ToXMMATRIX(mWorldMatrix);
		data[2] = ConvertMatrix4x4ToXMMATRIX(mViewMatrix);
		data[3] = ConvertMatrix4x4ToXMMATRIX(mProjectionMatrix);


		mConstantBuffer->SetData(data, sizeof(DirectX::XMMATRIX), 0);
		mDescriptorHeap->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);
	}
}

void Application::LoadPMD()
{
	mModelLoader = PMDLoader::Create(mDevice->GetDevice());
	mModelData = mModelLoader->LoadModel("Resource/Model/�����~�N.pmd");
	mModelData->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);

}

void Application::LoadPMX()
{
	mPMXModelLoader = PMXLoader::Create(mDevice->GetDevice());
	mPMXModelData = mPMXModelLoader->LoadModel("Resource/Model/�t�F�l�b�N/�t�F�l�b�N.pmx");
	mPMXModelData->_DebugGetDescHeap()->SetConstantBufferView(mConstantBuffer->GetConstantBufferView(0), 0);


	mInstancingTestModels.resize(100);
	float x = -10.0f;
	float z = 0.0f;
	srand((unsigned int)time(0));
	for (auto &model : mInstancingTestModels)
	{
		model = mPMXModelLoader->LoadModel("Resource/Model/�t�F�l�b�N/�t�F�l�b�N.pmx");
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
