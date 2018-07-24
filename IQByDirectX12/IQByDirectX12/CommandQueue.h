/*
	CommandQueue.h
	コマンドキューの実体を管理するクラス
	@author Ishibashi Ryuto
	@history
	2018/07/11 初版作成
*/
#pragma once
/* システムヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/// @fn Create
	/// コマンドキューの作成
	/// @param[in] device デバイスのポインタ
	/// @retval 生成成功時: CommandQueueのshared_ptr, 生成失敗時: nullptr
	static std::shared_ptr<CommandQueue> Create(ComPtr<ID3D12Device> device);

	/// @fn Create
	/// コマンドキューの作成
	/// @param[in] device デバイスのポインタ
	/// @param[in] commandQueueDesc コマンドキューの設定項目
	/// @retval 生成成功時: CommandQueueのshared_ptr, 生成失敗時: nullptr
	static std::shared_ptr<CommandQueue> Create(ComPtr<ID3D12Device> device, const D3D12_COMMAND_QUEUE_DESC& commandQueueDesc);

	/// @fn Get
	/// コマンドキューのComポインタを取得する
	/// @retval ID3D12CommandQueueのComPtr
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

private:
	/* 変数宣言 */
	ComPtr<ID3D12CommandQueue> mCommandQueue;
};

