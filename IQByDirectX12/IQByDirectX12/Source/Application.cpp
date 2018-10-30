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
#include "Model/PMD/PMDLoader.h"
#include "Model/PMD/PMDModelData.h"
#include "Model/PMX/PMXLoader.h"
#include "Model/PMX/PMXModelData.h"
#include "Texture/TextureManager.h"
#include "Model/InstancingDataManager.h"
#include "RootSignature.h"
#include "Shader.h"
#include "Motion/VMDLoader.h"
#include "SwapChain.h"
#include "Motion/Animation.h"
#include "GraphicsCommandList.h"
#include "Input/Keyboard.h"
#include "Debug/DebugLayer.h"
#include "Camera/Camera.h"
#include "Camera/Dx12Camera.h"
#include "PipelineStateObject.h"

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

	// �f�o�C�X�̐���
	mDevice = Device::Create();
	if (!mDevice)
	{
		return false;
	}

	// �R�}���h�L���[�̐���
	mCommandQueue = CommandQueue::Create(mDevice);
	if (!mCommandQueue)
	{
		return false;
	}

	// �X���b�v�`�F�C������
	mSwapChain = SwapChain::Create(mCommandQueue, window, RENDER_TARGET_NUM);
	if (!mSwapChain)
	{
		return false;
	}

	// �����_�[�^�[�Q�b�g����
	mRenderTarget = RenderTarget::Create(mDevice, mSwapChain->GetSwapChain(), RENDER_TARGET_NUM);
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
	
	// PMD�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̍쐬
	if (!CreatePMDPipelineStateObject())
	{
		return false;
	}

	// PMX�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̍쐬
	if (!CreatePMXPipelineStateObject())
	{
		return false;
	}

	// �R�}���h���X�g�̍쐬
	if (!CreateCommandList())
	{
		return false;
	}

	// �e�N�X�`���}�l�[�W���̔����e�N�X�`���쐬
	TextureManager::GetInstance().CreateWhiteAndBlackTexture(mDevice);

	// �J�����̍쐬
	CreateCamera();

	// �C���X�^���V���O�f�[�^�}�l�[�W���Ƀf�o�C�X��o�^
	InstancingDataManager::GetInstance().SetDevice(mDevice);

	// ���f���f�[�^�Ǎ�
	LoadPMD();
	LoadPMX();
	LoadMotion();

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
	mAnimationData->SetPose(t, mModelData->_DebugGetPose());
	mModelData->Draw();
	// endDebug

	// �R�}���h���X�g������
	mCommandList->Reset(mPipelineState);
	(*mCommandList)->SetGraphicsRootSignature(mRootSignature->GetRootSignature().Get());


	// �`��͈͐ݒ�
	D3D12_VIEWPORT vp = { 0.0f,0.0f, (FLOAT)mWindowWidth, (FLOAT)mWindowHeight, 0.0f,1.0f };
	D3D12_RECT rc = { 0,0,mWindowWidth, mWindowHeight };
	(*mCommandList)->RSSetViewports(1, &vp);
	(*mCommandList)->RSSetScissorRects(1, &rc);

	// �`���ύX����
	int backBufferIndex = mSwapChain->GetBackBufferIndex();
	
	mRenderTarget->ChangeRenderTarget(mCommandList->GetCommandList(), backBufferIndex);

	auto rtvHandle = mRenderTarget->GetRTVHandle();
	auto dsvHandle = mDepthBuffer->GetDSVHandle();
	
	(*mCommandList)->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	mRenderTarget->ClearRenderTarget(mCommandList->GetCommandList());
	mDepthBuffer->ClearDepthBuffer(mCommandList->GetCommandList());


	// �|���S���`��
	(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �萔�o�b�t�@�o�^
	
	mDx12Camera->SetCameraData(mCommandList, 0);

	// ���f���`��
	ModelDataManager::GetInstance().Draw(mCommandList->GetCommandList());

	//�`��I������
	mRenderTarget->FinishRendering(mCommandList->GetCommandList());
	mCommandList->Close();

	// �`��R�}���h���s
	ID3D12CommandList* commandLists[] = { mCommandList->GetCommandList().Get() };
	mCommandQueue->ExecuteCommandList(_countof(commandLists), commandLists);

	mCommandQueue->Signal();

	// ��ʃX���b�v
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
	// ���_����`
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
	for (int i = 0; i < 8; ++i)
	{
		gpsDesc.BlendState.RenderTarget[i].BlendEnable = true;
		gpsDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gpsDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
		gpsDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ONE;
		gpsDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_MAX;

		gpsDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		gpsDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		gpsDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;

	}

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
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleMask = UINT_MAX;

	//auto result = (*mDevice)->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	mPMXPipelineState = PipelineStateObject::Create(mDevice, gpsDesc);
	if (!mPMXPipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
}

bool Application::_DebugCreatePMDPipelineState()
{
	// ���_����`
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

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc{};

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	for (int i = 0; i < 8; ++i)
	{
		gpsDesc.BlendState.RenderTarget[i].BlendEnable = true;
		gpsDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		gpsDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
		gpsDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ONE;
		gpsDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_MAX;

		gpsDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		gpsDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		gpsDesc.BlendState.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
	}

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
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpsDesc.NumRenderTargets = 1;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleMask = UINT_MAX;

	//auto result = (*mDevice)->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&mPipelineState));
	mPMDPipelineState = PipelineStateObject::Create(mDevice, gpsDesc);
	if (!mPMDPipelineState)
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

void Application::CreateCamera()
{
	/// �ˉe�s��̃p�����[�^�ݒ�
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
	//mModelData = mModelLoader->LoadModel("Resource/Model/����얲/reimu_G02.pmd", mPMDPipelineState);
	mModelData = mModelLoader->LoadModel("Resource/Model/�����~�N.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/��ߔe��v1.0/��ߔe��v1.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/MMD_Default/�����~�Nmetal.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/hibari/�_��Ver1.10.pmd", mPMDPipelineState);
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
		//model = mPMXModelLoader->LoadModel("Resource/Model/�t�F�l�b�N/�t�F�l�b�N.pmx", mPMXPipelineState);
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
	//mAnimationData =loader->Load("Resource/Motion/�o�C�o�C.vmd");
	//mAnimationData =loader->Load("Resource/Motion/�r�P��.vmd");
	//mAnimationData =loader->Load("Resource/Motion/�������[�v���[�V�����f��161025/10_�`���R���[�g�f�B�X�R���ۂ�.vmd");
	mAnimationData = loader->Load("Resource/Motion/�������[�v���[�V�����f��161025/01_�W�����v�蔏�q01.vmd");
	//mAnimationData = loader->Load("Resource/Model/����얲/���[�V����/���S�R���_���X.vmd");
	//mAnimationData = loader->Load("Resource/Motion/swing2.vmd");
	//mAnimationData = loader->Load("Resource/Motion/charge.vmd");
}

void Application::UpdateMatrix()
{
	//mDx12Camera->Rotate(Math::CreateRotXYZQuaternion(Math::Vector3(0.0f, Math::F_PI / 60.0f, 0.0f)));
	mDx12Camera->UpdateMatrix();
}