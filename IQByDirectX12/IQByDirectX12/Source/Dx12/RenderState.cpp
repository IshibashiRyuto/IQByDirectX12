#include "RenderState.h"

/* 実体定義 */
bool														RenderState::mIsInitialize{ false };	//! 初期化済みか
std::map<CullingType, D3D12_RASTERIZER_DESC>				RenderState::mRasterizerStates;					//! ラスタライザステート
std::map<bool, std::map<bool, D3D12_DEPTH_STENCIL_DESC>>	RenderState::mDepthStencilStates;	//! デプスステンシルステート
std::map<AlphaBlendType, D3D12_BLEND_DESC>					RenderState::mBlendStates;

D3D12_GRAPHICS_PIPELINE_STATE_DESC RenderState::GetPipelineStateDesc() const
{
	if (!mIsInitialize)
	{
		InitStateTable();
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc{};
	
	psDesc.RasterizerState = mRasterizerStates[cullingType];
	psDesc.DepthStencilState = mDepthStencilStates[depthTest][depthWrite];
	psDesc.BlendState = mBlendStates[alphaBlendType];

	return psDesc;
}

void RenderState::InitStateTable() const
{
	mIsInitialize = true;

	// カリングモードテーブルの作成
	D3D12_CULL_MODE cullTbl[] = 
	{
		D3D12_CULL_MODE_BACK,
		D3D12_CULL_MODE_FRONT,
		D3D12_CULL_MODE_NONE,
	};

	// ラスタライザステートの初期化
	for (int i = 0; i < CULLING_TYPE_COUNT; ++i)
	{
		auto ct = static_cast<CullingType>(i);
		mRasterizerStates[ct] = D3D12_RASTERIZER_DESC{};
		mRasterizerStates[ct].CullMode = cullTbl[i];
		mRasterizerStates[ct].FillMode = D3D12_FILL_MODE_SOLID;
		mRasterizerStates[ct].DepthClipEnable = true;
	}

	// デプスステンシルステートの初期化
	for (int dt = 0; dt < 2; ++dt)
	{
		for (int dw = 0; dw < 2; ++dw)
		{
			bool depthTest = static_cast<bool>(dt);
			bool depthWrite = static_cast<bool>(dw);
			mDepthStencilStates[depthTest][depthWrite] = D3D12_DEPTH_STENCIL_DESC{};
			mDepthStencilStates[depthTest][depthWrite].DepthEnable = depthTest;
			mDepthStencilStates[depthTest][depthWrite].DepthWriteMask = static_cast<D3D12_DEPTH_WRITE_MASK>(depthWrite);
			mDepthStencilStates[depthTest][depthWrite].DepthFunc = depthFunc;
			mDepthStencilStates[depthTest][depthWrite].StencilEnable = false;
		}
	}

	// ブレンドステートの初期化
	for (int i = 0; i < ALPHA_TYPE_COUNT; ++i)
	{
		auto type = static_cast<AlphaBlendType>(i);
		mBlendStates[type] = D3D12_BLEND_DESC{};
		mBlendStates[type].AlphaToCoverageEnable = false;

		for (int j = 0; j < 8; ++j)
		{
			mBlendStates[type].RenderTarget[j].BlendEnable = type != AlphaBlendType::Opacity;
			mBlendStates[type].RenderTarget[j].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			mBlendStates[type].RenderTarget[j].SrcBlendAlpha = D3D12_BLEND_ONE;
			mBlendStates[type].RenderTarget[j].DestBlendAlpha = D3D12_BLEND_ONE;
			mBlendStates[type].RenderTarget[j].BlendOpAlpha = D3D12_BLEND_OP_MAX;
			switch (type)
			{
			case AlphaBlendType::Opacity:
				mBlendStates[type].RenderTarget[j].DestBlend = D3D12_BLEND_ZERO;
				mBlendStates[type].RenderTarget[j].SrcBlend = D3D12_BLEND_ONE;
				mBlendStates[type].RenderTarget[j].BlendOp = D3D12_BLEND_OP_ADD;
				break;
			case AlphaBlendType::Blend:
				mBlendStates[type].RenderTarget[j].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				mBlendStates[type].RenderTarget[j].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				mBlendStates[type].RenderTarget[j].BlendOp = D3D12_BLEND_OP_ADD;
				break;
			case AlphaBlendType::Add:
				mBlendStates[type].RenderTarget[j].DestBlend = D3D12_BLEND_ONE;
				mBlendStates[type].RenderTarget[j].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				mBlendStates[type].RenderTarget[j].BlendOp = D3D12_BLEND_OP_ADD;
				break;
			case AlphaBlendType::Sub:
				mBlendStates[type].RenderTarget[j].DestBlend = D3D12_BLEND_ONE;
				mBlendStates[type].RenderTarget[j].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				mBlendStates[type].RenderTarget[j].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
				break;
			case AlphaBlendType::Mul:
				mBlendStates[type].RenderTarget[j].DestBlend = D3D12_BLEND_SRC_COLOR;
				mBlendStates[type].RenderTarget[j].SrcBlend = D3D12_BLEND_ZERO;
				mBlendStates[type].RenderTarget[j].BlendOp = D3D12_BLEND_OP_ADD;
				break;
			}
		}
	}
}
