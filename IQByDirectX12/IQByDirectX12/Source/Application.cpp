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
#include "Model/Primitive/PrimitiveCreator.h"

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

	// �e�N�X�`���}�l�[�W���̏�����
	TextureManager::GetInstance().Initialize(mDevice);
	mRTVFormat.push_back(DXGI_FORMAT_R8G8B8A8_UNORM);

	// �R�}���h�L���[�̐���
	mCommandQueue = CommandQueue::Create(mDevice);
	if (!mCommandQueue)
	{
		return false;
	}

	// �X���b�v�`�F�C������
	mSwapChain = SwapChain::Create(mCommandQueue, window);
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
		mDepthBuffer = DepthBuffer::Create(mDevice, windowWidth, windowHeight, L"RenderDepthBuffer", false);
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

	// �y���|���p�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̍쐬
	if (!CreatePeraPipelineStateObject())
	{
		return false;
	}

	if (!CreateSpritePipelineStateObject())
	{
		return false;
	}

	// �v���~�e�B�u�p�p�C�v���C���X�e�[�g�I�u�W�F�N�g�̍쐬
	if (!CreatePrimitivePipelineStateObject())
	{
		return false;
	}

	// �R�}���h���X�g�̍쐬
	if (!CreateCommandList())
	{
		return false;
	}

	// �V���h�E�}�b�v�̍쐬
	CreateShadowMap();


	// �J�����̍쐬
	CreateCamera();

	// �C���X�^���V���O�f�[�^�}�l�[�W���Ƀf�o�C�X��o�^
	InstancingDataManager::GetInstance().SetDevice(mDevice);

	// ���f���f�[�^�Ǎ�
	LoadPMD();
	LoadPMX();
	LoadMotion();

	// �y���|���p�f�[�^�쐬
	_DebugCreatePeraPolyData();

	// �X�v���C�g�f�[�^�쐬

	if (!_DebugCreateSprite())
	{
		return false;
	}

	// ���C�g�쐬
	if (!CreateDirectionalLight())
	{
		return false;
	}

	// �v���~�e�B�u���f���쐬
	CreatePrimitive();

	//	�e�N�X�`�����̍X�V����
	TextureManager::GetInstance().UpdateTextureData();

	return true;
}

void Application::Render()
{

	mKeyboard->UpdateKeyState();

	ModelMove();

	CameraMove();

	//LightMove();

	// �R�}���h���X�g������
	mCommandList->Reset();
	(*mCommandList)->SetGraphicsRootSignature(mRootSignature->GetRootSignature().Get());



	/* �V���h�E�}�b�v�p�`��͈� */
	D3D12_VIEWPORT svp = { 0.0f,0.0f, (FLOAT)1024, (FLOAT)1024, 0.0f,1.0f };
	D3D12_RECT src = { 0,0,1024, 1024 };

	(*mCommandList)->RSSetViewports(1, &svp);
	(*mCommandList)->RSSetScissorRects(1, &src);



	/* 1�p�X��(�[�x�`��) */
	{
		mRenderTarget->ChangeRenderTarget(mCommandList, 2);

		// ���f���̎Q�Ɛ[�x�}�b�v�ύX
		mShadowMapHeap->SetBindHeapIndex(0);

		auto dsvHandle = mShadowDepth->GetDSVCPUHandle();

		(*mCommandList)->OMSetRenderTargets(0, nullptr, false, &dsvHandle);

		mShadowDepth->BeginWriteDepth(mCommandList);

		// �g�|���W�Z�b�g
		(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �J�����Z�b�g
		mDirectionalLight->BindDescriptorHeap(mRootSignature, 0);
		mDirectionalLight->BindDescriptorHeap(mPrimitiveRootSignature, 0);
		// ���f���`��
		ModelDataManager::GetInstance().DrawShadow(mCommandList,false);

		//�@1�p�X�ڕ`��I������
		mShadowDepth->EndWriteDepth(mCommandList);
	}

	// �`��͈͐ݒ�
	D3D12_VIEWPORT vp = { 0.0f,0.0f, (FLOAT)mWindowWidth, (FLOAT)mWindowHeight, 0.0f,1.0f };
	D3D12_RECT rc = { 0,0,mWindowWidth, mWindowHeight };
	(*mCommandList)->RSSetViewports(1, &vp);
	(*mCommandList)->RSSetScissorRects(1, &rc);

	/* 2�p�X��(���f���`��) */
	{
		// ���f���̎Q�Ɛ[�x�}�b�v�ύX
		mShadowMapHeap->SetBindHeapIndex(1);

		// �`���ύX����
		//int backBuffer = mSwapChain->GetBackBufferIndex();
		mRenderTarget->ChangeRenderTarget(mCommandList, 3);
		mRenderTarget->ClearRenderTarget(mCommandList);
		mRenderTarget->ChangeRenderTarget(mCommandList, 2);

		auto rtvHandle = mRenderTarget->GetRTVHandle();
		auto dsvHandle = mDepthBuffer->GetDSVCPUHandle();

		(*mCommandList)->OMSetRenderTargets(2, &rtvHandle, true, &dsvHandle);

		mRenderTarget->ClearRenderTarget(mCommandList);
		mDepthBuffer->BeginWriteDepth(mCommandList);

		// �g�|���W�Z�b�g
		(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �J�����Z�b�g
		mDx12Camera->BindDescriptorHeap(mRootSignature, 0);
		mDx12Camera->BindDescriptorHeap(mPrimitiveRootSignature, 0);

		// ���f���`��
		ModelDataManager::GetInstance().Draw(mCommandList);

		//�@2�p�X�ڕ`��I������
		mRenderTarget->FinishRendering(mCommandList);
		mDepthBuffer->EndWriteDepth(mCommandList);

	}
	/* 2�p�X�ڕ`��I�� */

	/* 3�p�X��(�|�X�g�G�t�F�N�g) */
	{
		// �`���ύX
		int backBuffer = mSwapChain->GetBackBufferIndex();
		mRenderTarget->ChangeRenderTarget(mCommandList, backBuffer);

		auto rtvHandle = mRenderTarget->GetRTVHandle();

		(*mCommandList)->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		mRenderTarget->ClearRenderTarget(mCommandList);

		// �p�C�v���C���A���[�g�V�O�l�`���ύX
		(*mCommandList)->SetPipelineState(mPeraPipelineState->GetPipelineStateObject().Get());
		(*mCommandList)->SetGraphicsRootSignature(mPeraRootSignature->GetRootSignature().Get());

		//�g�|���W�Z�b�g
		(*mCommandList)->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �f�[�^�Z�b�g
		mPeraDescHeap->BindGraphicsCommandList(mCommandList->GetCommandList());
		mPeraDescHeap->BindRootDescriptorTable(0, 0);

		// �y���|���̕`��
		(*mCommandList)->IASetVertexBuffers(0, 1, &mPeraVert->GetVertexBufferView());
		(*mCommandList)->DrawInstanced(4, 1, 0, 0);

		// �X�v���C�g�̕`��
		//SpriteDataManager::GetInstance().Draw(mCommandList);


		//�@3�p�X�ڕ`��I������
		mRenderTarget->FinishRendering(mCommandList);
	}
	/* 3�p�X�ڕ`��I�� */

	//�`��I������
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
	if (!mRootSignature)
	{
		if (!CreateRootSignature())
		{
			return false;
		}
	}

	if (!_DebugReadPMDShader())
	{
		return false;
	}

	return _DebugCreatePMDPipelineState();
}

bool Application::CreatePMXPipelineStateObject()
{
	if (!mRootSignature)
	{
		if (!CreateRootSignature())
		{
			return false;
		}
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

bool Application::CreatePrimitivePipelineStateObject()
{
	if (!CreatePrimitiveRootSignature())
	{
		return false;
	}

	if (!ReadPrimitiveShader())
	{
		return false;
	}

	return CreatePrimitivePipelineState();
}

bool Application::CreateRootSignature()
{
	mRootSignature = RootSignature::Create();
	int cbvIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int light = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int boneMatrixBufferIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int materialIndex = mRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);

	mRootSignature->AddDescriptorRange(cbvIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mRootSignature->AddDescriptorRange(light, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	mRootSignature->AddDescriptorRange(boneMatrixBufferIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3);
	mRootSignature->AddDescriptorRange(materialIndex, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0);
	
	return mRootSignature->ConstructRootSignature(mDevice->GetDevice());
}

bool Application::CreatePeraRootSignature()
{
	mPeraRootSignature = RootSignature::Create();
	int idx = mPeraRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_PIXEL);
	mPeraRootSignature->AddDescriptorRange(idx, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0);

	return mPeraRootSignature->ConstructRootSignature(mDevice->GetDevice());
}

bool Application::CreatePrimitiveRootSignature()
{
	mPrimitiveRootSignature = RootSignature::Create();
	int camera = mPrimitiveRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int light = mPrimitiveRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int shadowMap = mPrimitiveRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	int material = mPrimitiveRootSignature->AddRootParameter(D3D12_SHADER_VISIBILITY_ALL);
	mPrimitiveRootSignature->AddDescriptorRange(camera, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	mPrimitiveRootSignature->AddDescriptorRange(light, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
	mPrimitiveRootSignature->AddDescriptorRange(shadowMap, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	mPrimitiveRootSignature->AddDescriptorRange(material, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
	return mPrimitiveRootSignature->ConstructRootSignature(mDevice->GetDevice());
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

	mPixelShaderClass = Shader::Create(L"Resource/Shader/Bloom.hlsl", "PSMain", "ps_5_0");


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

bool Application::ReadPrimitiveShader()
{
	mVertexShaderClass = Shader::Create(L"Resource/Shader/3DPrimitiveShader.hlsl", "VSMain", "vs_5_0");

	mPixelShaderClass = Shader::Create(L"Resource/Shader/3DPrimitiveShader.hlsl", "PSMain", "ps_5_0");

	if (!mVertexShaderClass || !mPixelShaderClass)
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

	ShaderList shaderList;
	shaderList.VS = mVertexShaderClass;
	shaderList.PS = mPixelShaderClass;
	
	RenderState renderState;
	renderState.alphaBlendType = AlphaBlendType::Blend;
	renderState.depthTest = true;
	renderState.depthWrite = true;
	renderState.cullingType = CullingType::Double;
	renderState.depthFunc = D3D12_COMPARISON_FUNC_LESS;

	mPMXPipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mRootSignature, renderState, shaderList, mRTVFormat);
	mPMXShadowPSO = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mRootSignature, renderState, shaderList, std::vector<DXGI_FORMAT>());

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

	ShaderList shaderList;

	shaderList.VS = mVertexShaderClass;
	shaderList.PS = mPixelShaderClass;
	
	RenderState renderState;
	renderState.alphaBlendType = AlphaBlendType::Opacity;
	renderState.depthTest = true;
	renderState.depthWrite = true;
	renderState.cullingType = CullingType::Double;
	renderState.depthFunc = D3D12_COMPARISON_FUNC_LESS;

	mPMDPipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mRootSignature,renderState, shaderList, mRTVFormat);

	mPMDShadowPSO = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mRootSignature, renderState, shaderList, std::vector<DXGI_FORMAT>());
	if (!mPMDPipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
}

bool Application::CreatePeraPipelineState()
{
	// ���_����`
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

	mPeraPipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mPeraRootSignature, renderState, shaderList, mRTVFormat);
	if (!mPeraPipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}
	return true;
}

bool Application::CreateSpritePipelineState()
{// ���_����`
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

	mSpritePipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mPeraRootSignature, renderState, shaderList, mRTVFormat);
	if (!mSpritePipelineState)
	{
		DebugLayer::GetInstance().PrintDebugMessage("Failed Create PipelineObject.\n");
		return false;
	}

	return true;
}

bool Application::CreatePrimitivePipelineState()
{
	{
		mInputLayoutDescs.clear();
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "POSITION"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "NORMAL"			, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		mInputLayoutDescs.push_back(D3D12_INPUT_ELEMENT_DESC{ "TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

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

	mPrimitivePipelineState = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mPrimitiveRootSignature, renderState, shaderList, mRTVFormat);
	mPrimitiveShadowPSO = PipelineStateObject::Create(mDevice, mInputLayoutDescs, mPrimitiveRootSignature, renderState, shaderList, std::vector<DXGI_FORMAT>());
	if (!mPrimitivePipelineState)
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
	mDx12Camera->BindDescriptorHeap(mRootSignature, 0);
	mDx12Camera->BindDescriptorHeap(mPrimitiveRootSignature, 0);
}

void Application::LoadPMD()
{
	mModelLoader = PMDLoader::Create(mDevice, "Resource/Model/Toon");
	//mModelData = mModelLoader->LoadModel("Resource/Model/����얲/reimu_G02.pmd", mPMDPipelineState);
	//mModelData = mModelLoader->LoadModel("Resource/Model/�����~�N.pmd", mPMDPipelineState, mPMDShadowPSO, mRootSignature);
	//mModelData = mModelLoader->LoadModel("Resource/Model/��ߔe��v1.0/��ߔe��v1.pmd", mPMDPipelineState, mPMDShadowPSO, mRootSignature);
	//mModelData = mModelLoader->LoadModel("Resource/Model/MMD_Default/�����~�Nmetal.pmd", mPMDPipelineState, mPMDShadowPSO, mRootSignature);
	mModelData = mModelLoader->LoadModel("Resource/Model/hibari/�_��Ver1.10.pmd", mPMDPipelineState, mPMDShadowPSO, mRootSignature);
	if (!mModelData)
	{
		return;
	}

}

void Application::LoadPMX()
{
	mPMXModelLoader = PMXLoader::Create(mDevice, "Resource/Model/Toon");


	mInstancingTestModels.resize(1);
	float x = -10.0f;
	float z = 0.0f;
	srand((unsigned int)time(0));
	for (auto &model : mInstancingTestModels)
	{
		//model = mPMXModelLoader->LoadModel("Resource/Model/Mirai_Akari_v1.0/MiraiAkari_v1.0.pmx", mPMXPipelineState, mPMXShadowPSO, mRootSignature);
		model = mPMXModelLoader->LoadModel("Resource/Model/KizunaAI_ver1.01/kizunaai/kizunaai.pmx", mPMXPipelineState, mPMXShadowPSO, mRootSignature);
		//model = mPMXModelLoader->LoadModel("Resource/Model/�t�F�l�b�N/�t�F�l�b�N.pmx", mPMXPipelineState, mPMXShadowPSO, mRootSignature);
		//model = mPMXModelLoader->LoadModel("Resource/Model/TokinoSora_mmd_v.1.3/TokinoSora_2017.pmx", mPMXPipelineState, mPMXShadowPSO, mRootSignature);
	}
}

void Application::LoadMotion()
{
	auto loader = VMDLoader::Create();
	//mAnimationData =loader->Load("Resource/Motion/�o�C�o�C.vmd");
	//mAnimationData =loader->Load("Resource/Motion/�r�P��.vmd");
	//mAnimationData =loader->Load("Resource/Motion/�������[�v���[�V�����f��161025/10_�`���R���[�g�f�B�X�R���ۂ�.vmd");
	//mAnimationData = loader->Load("Resource/Motion/�������[�v���[�V�����f��161025/01_�W�����v�蔏�q01.vmd");
	mAnimationData = loader->Load("Resource/Model/����얲/���[�V����/���S�R���_���X.vmd");
	//mAnimationData = loader->Load("Resource/Motion/swing2.vmd");
	//mAnimationData = loader->Load("Resource/Motion/charge.vmd");
	//mAnimationData = loader->Load("Resource/Motion/Koikaze_V2/Koikaze_V2.vmd");
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
	auto lumaTexture = mRenderTarget->GetRenderTexture(3);
	mPeraDescHeap->SetTexture(lumaTexture, 1);
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
	projParam.width = 128.f;
	projParam.height = 128.f;
	mDirectionalLight = Dx12Camera::Create(Math::Vector3(100, 100, -300), Math::Vector3(-1.f, -1.f, 3.f), ProjectionType::Orthographic, projParam, mDevice);
	if (!mDirectionalLight)
	{
		false;
	}

	mDirectionalLight->UpdateMatrix();
	mDirectionalLight->BindDescriptorHeap(mPrimitiveRootSignature, 1);
	mDirectionalLight->BindDescriptorHeap(mRootSignature, 1);

	return true;
}

void Application::CreatePrimitive()
{
	PrimitiveCreator::GetInstance().Initialize(mDevice, mPrimitivePipelineState, mPrimitiveShadowPSO, mPrimitiveRootSignature);
	mPlane = PrimitiveCreator::GetInstance().CreatePlane(Math::Vector2(100.f, 100.f));
}

void Application::CreateShadowMap()
{
	mShadowDepth = DepthBuffer::Create(mDevice, 1024, 1024, L"ShadowMap");
	mDepthTexture = Texture::Create(mShadowDepth);
	mShadowMapHeap = DescriptorHeap::Create(mDevice, 2);
	mShadowMapHeap->SetBindHeapIndex(0);
	mShadowMapHeap->SetTexture(TextureManager::GetInstance().GetTexture(TextureManager::WHITE_TEXTURE), 0);
	mShadowMapHeap->SetTexture(mDepthTexture, 1);
	mPrimitiveRootSignature->SetBindDescriptorHeap(2, mShadowMapHeap);
}

void Application::ModelMove()
{
	static float t = 0;

	static Math::Quaternion rot = Math::CreateRotXYZQuaternion(Math::Vector3(0.f, 0.f, 0.f));
	static Math::Vector3 rotAxis(1.f, 0.f, 0.f);
	static Math::Vector3 pos(0.0f, 0.0f, 0.0f);
	static float speed = 0.05f;


	if (mKeyboard->IsKeyDown(VirtualKeyIndex::C))
	{
		rot *= Math::CreateRotAxisQuaternion(rotAxis, 0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::V))
	{
		rot *= Math::CreateRotAxisQuaternion(rotAxis, -0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Z))
	{
		rot *= Math::CreateRotAxisQuaternion(Math::Vector3(0.f, 1.f, 0.f), 0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::X))
	{
		rot *= Math::CreateRotAxisQuaternion(Math::Vector3(0.f, 1.f, 0.f), -0.03f);
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::W))
	{
		pos.y += speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::S))
	{
		pos.y -= speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::A))
	{
		pos.x -= speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::D))
	{
		pos.x += speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Q))
	{
		pos.z -= speed;
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::E))
	{
		pos.z += speed;
	}

	static bool isStop = false;

	if (mKeyboard->IsKeyTrigger(VirtualKeyIndex::Space))
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
		mAnimationData->SetPose(static_cast<int>(t), mInstancingTestModels[0]->_DebugGetPose());
		
	}
	int i = 0;
	// PMX Instance Draw
	for (auto model : mInstancingTestModels)
	{
		{
			model->SetRotation(rot);
			model->SetPosition(pos + Math::Vector3(-10.f * (i % 5), 0.f, 10.f * (i / 5)));
			model->Draw();
		}
		++i;
	}

	// PMD Draw
	mModelData->SetRotation(rot);
	mModelData->SetPosition(pos + Math::Vector3(10.0f, 0.0f, 0.0f));
	mModelData->Draw();

	mPlane->Draw();
}

void Application::CameraMove()
{

	static float speed = 0.5f;

	if(mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad8))
	{
		mDx12Camera->Move(Math::Vector3(0, speed, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad4))
	{
		mDx12Camera->Move(Math::Vector3(-speed, 0, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad2))
	{
		mDx12Camera->Move(Math::Vector3(0, -speed, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad6))
	{
		mDx12Camera->Move(Math::Vector3(speed, 0, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad1))
	{
		mDx12Camera->Move(Math::Vector3(0, 0, speed));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad3))
	{
		mDx12Camera->Move(Math::Vector3(0, 0, -speed));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad9))
	{
		mDx12Camera->Rotate(Math::CreateRotXYZQuaternion(Math::Vector3(0.f, Math::F_PI/ 90.f, 0.f)));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::Numpad7))
	{
		mDx12Camera->Rotate(Math::CreateRotXYZQuaternion(Math::Vector3(0.f, -Math::F_PI / 90.f, 0.f)));
	}
	
	mDx12Camera->UpdateMatrix();
}

void Application::LightMove()
{
	static float speed = 0.5f;

	if (mKeyboard->IsKeyDown(VirtualKeyIndex::I))
	{
		mDirectionalLight->Move(Math::Vector3(0, speed, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::J))
	{
		mDirectionalLight->Move(Math::Vector3(-speed, 0, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::K))
	{
		mDirectionalLight->Move(Math::Vector3(0, -speed, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::L))
	{
		mDirectionalLight->Move(Math::Vector3(speed, 0, 0));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::U))
	{
		mDirectionalLight->Move(Math::Vector3(0, 0, speed));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::O))
	{
		mDirectionalLight->Move(Math::Vector3(0, 0, -speed));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::N))
	{
		mDirectionalLight->Rotate(Math::CreateRotXYZQuaternion(Math::Vector3(0.f, Math::F_PI / 90.f, 0.f)));
	}
	if (mKeyboard->IsKeyDown(VirtualKeyIndex::M))
	{
		mDirectionalLight->Rotate(Math::CreateRotXYZQuaternion(Math::Vector3(0.f, -Math::F_PI / 90.f, 0.f)));
	}

	mDirectionalLight->UpdateMatrix();
}
