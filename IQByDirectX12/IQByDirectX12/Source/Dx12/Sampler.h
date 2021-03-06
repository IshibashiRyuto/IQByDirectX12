/*
*	@file Sampler.h
*	@brief 動的サンプラクラスを記述したファイル
*	@author Ishibashi Ryuto
*	@date 
*	2018/09/27	初版作成
*/
#pragma once
// システムヘッダインクルード
#include <memory>
#include <map>
#include <d3d12.h>

// 自作ヘッダインクルード
#include "RenderState.h"

// クラス使用宣言
class Device;
class DescriptorHeap;


class Sampler
{
public:
	/// @brief コンストラクタ
	Sampler();

	/// @brief デストラクタ
	~Sampler();

	/// @brief Samplerインスタンスを生成する
	/// @note Samplerクラスインスタンスはこのメソッドでのみ生成可能
	std::shared_ptr<Sampler> Create(const std::shared_ptr<Device> device);
private:
	std::shared_ptr<DescriptorHeap> mSamplerDescriptorHeap;			//! サンプラーを格納するディスクリプタヒープ
	std::map<TextureFilterType, std::map<TextureWrapType, D3D12_GPU_DESCRIPTOR_HANDLE>> mSampler;
};

