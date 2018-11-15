/*
	@file	PipelineStateObject.h
	@brief	パイプラインステートオブジェクトを管理するクラス
	@author Ishibashi Ryuto
	@dateo
	2018/10/09	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class Device;
class RenderState;
struct ShaderList;
class RootSignature;

class PipelineStateObject
{
public:
	/// @brief	デストラクタ
	~PipelineStateObject();

	/// @brief	パイプラインステートオブジェクトを作成する
	/// @param[in] pipelineStateDesc パイプラインステート情報
	/// @retval 生成成功 : パイプラインステートオブジェクトのスマートポインタ
	/// @retval 生成失敗 : nullptr
	static std::shared_ptr<PipelineStateObject> Create(std::shared_ptr<Device> device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC & pipelineStateDesc);

	/// @brief	パイプラインステートオブジェクトを作成する
	/// @param[in]	device		: dx12デバイス
	/// @param[in]	inputLayout	: 頂点レイアウト
	///	@param[in]	renderState	: レンダリングステート
	/// @param[in]	shaderList	: シェーダリスト
	///	@retval	生成成功	: パイプラインステートオブジェクト
	/// @retval	生成失敗	: nullptr
	static std::shared_ptr<PipelineStateObject> Create(std::shared_ptr<Device> device,
		const std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout,
		std::shared_ptr<RootSignature> rootSignature,
		const RenderState& renderState,
		const ShaderList& shaderList
		);

	/// @brief	パイプラインステートオブジェクトを取得する
	ComPtr<ID3D12PipelineState> GetPipelineStateObject() const;

private:

	ComPtr<ID3D12PipelineState> mPipelineStateObject;	//! PSO実体

	/// @brief	コンストラクタ
	PipelineStateObject(std::shared_ptr<Device> device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC & pipelineStateDesc);
};

