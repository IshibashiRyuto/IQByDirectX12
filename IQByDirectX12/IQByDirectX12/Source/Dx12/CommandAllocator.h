/*
	CommandAllocator.h
	コマンドアロケータの実体を管理するクラス
	@author Ishibashi Ryuto
	@history
	2018/07/11 初版作成
*/
#pragma once
/* システムヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <string>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;
class Device;

class CommandAllocator
{
public:

	/// デストラクタ
	~CommandAllocator();

	/// @fn Create
	/// コマンドアロケータを生成する
	/// @note CommandAllocatorクラスはこのメソッドを通じてのみ生成可能
	/// 実体を直接持つことはできない。
	/// @param[in] device			D3D12デバイス
	/// @param[in] commandListType	コマンドリストの種類
	/// @param[in] name		: 名前
	/// @retval 生成成功時:CommandAllocatorのshared_ptr, 生成失敗時: nullptr
	static std::shared_ptr<CommandAllocator> Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring& name = L"");

	/// @fn Get
	/// コマンドアロケータのComPtrを取得する
	/// @retval ID3D12CommandAllocatorのComPtr
	ComPtr<ID3D12CommandAllocator> GetAllocator() const;

	/// コマンドのアロケータを指すオペレータ
	ID3D12CommandAllocator* const operator->() const;

private:
	/* 変数宣言 */
	// コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;

	/* ローカルメソッド */
	/// コンストラクタ
	CommandAllocator();
};

