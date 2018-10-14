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
#include <d3d12.h>
#include <wrl.h>

// クラス使用宣言
using Microsoft::WRL::ComPtr;
class Device;

class PipelineStateObject
{
public:
	/// @brief	コンストラクタ
	PipelineStateObject();
	/// @brief	デストラクタ
	~PipelineStateObject();

	/// @brief	パイプラインステートオブジェクトを作成する
	/// @param[in] pipelineStateDesc パイプラインステート情報
	/// @retval 生成成功 : パイプラインステートオブジェクトのスマートポインタ
	/// @retval 生成失敗 : nullptr
	static std::shared_ptr<PipelineStateObject> Create(std::shared_ptr<Device> device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC & pipelineStateDesc);

	/// @brief	パイプラインステートオブジェクトを取得する
	ComPtr<ID3D12PipelineState> GetPipelineStateObject() const;

private:

	ComPtr<ID3D12PipelineState> mPipelineStateObject;

};

