/*
	@file	CommandAllocator.h
	@author Ishibashi Ryuto
	@date	2018/07/11 初版作成
*/
#pragma once
/* ヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <string>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;
class Device;

/**
*	@class	CommandAllocator
*	@brief	コマンドアロケータの実体を管理するクラス
*/
class CommandAllocator
{
public:

	/**
	*	@brief	デストラクタ
	*/
	~CommandAllocator();

	/**
	*	@brief	コマンドアロケータを生成する
	*
	*	@param[in] device			: D3D12デバイス
	*	@param[in] commandListType	: コマンドリストの種類
	*	@param[in] name				: 名前
	*
	*	@retval 生成成功時	: CommandAllocatorのshared_ptr,
	*	@retval	生成失敗時	: nullptr
	*
	*	@note	CommandAllocatorクラスはこのメソッドを通じてのみ生成可能
	*			実体を直接持つことはできない
	*/
	static std::shared_ptr<CommandAllocator> Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring& name = L"");

	/**
	*	@brief	コマンドアロケータのComPtrを取得する
	*	@return ID3D12CommandAllocatorのComPtr
	*/
	ComPtr<ID3D12CommandAllocator> GetAllocator() const;

	/**
	*	@brief	コマンドアロケータを指すオペレータ
	*/
	ID3D12CommandAllocator* const operator->() const;

private:
	/* 変数宣言 */
	// コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;	//! コマンドアロケータの実体

	/* ローカルメソッド */
	/**
	*	@biref	コンストラクタ
	*/
	CommandAllocator();
};

