/*
	@file	Device.h
	@brief	Deviceの宣言を記述する
	@author Ishibashi Ryuto
	@date	2018/07/11 初版作成
*/
#pragma once
/* ヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;

/**
*	@class	Device
*	@brief	ID3D12Deviceのラッパークラス
*
*	D3D12Device実体を格納する
*	実体はComPtrで格納されており、ComPtrでのやり取りのみを許可している
*	(生ポインタはComPtrから取得する。直接取得することは許さない)
*/
class Device
{
public:

	/**
	*	@brief	デストラクタ
	*/
	~Device();

	/**
	*	@brief	デバイスを作成する
	*
	*	@retval デバイス作成成功時	: Deviceのスマートポインタ
	*	@retval	デバイス作成失敗時	: nullptr
	*
	*	@note	デバイスはこのメソッドで生成する。Deviceの実体をそのまま持つことはできない。
	*/
	static std::shared_ptr<Device> Create();

	/**
	*	デバイスのComPtrを取得する
	*	@retval ComPtr(ID3D12Device) デバイスのComPtr
	*/
	ComPtr<ID3D12Device> GetDevice() const;

	/**
	*	デバイスを作成したフィーチャーレベルを取得する
	*/
	D3D_FEATURE_LEVEL GetFeatureLevel() const;

	/**
	*	@brief デバイスへアクセスするオペレータ
	*/
	ID3D12Device * const operator->();

private:
	/**
	*	@brief	コンストラクタ
	*/
	Device();

	D3D_FEATURE_LEVEL mFeatureLevel;	//! デバイスフューチャーレベル
	ComPtr<ID3D12Device> mDevice;		//!	デバイス実体
};

