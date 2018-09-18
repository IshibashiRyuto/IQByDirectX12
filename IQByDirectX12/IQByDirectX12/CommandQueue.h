/*
	CommandQueue.h
	コマンドキューの実体を管理するクラス
	@author Ishibashi Ryuto
	@history
	2018/07/11 初版作成
	2018/07/30 ExecuteCommandLists, Signalを追加
*/
#pragma once
/* システムヘッダインクルード */
#include <memory>
#include <d3d12.h>
#include <wrl.h>

// ComPtr使用宣言
using Microsoft::WRL::ComPtr;
class Device;

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/// @fn Create
	/// コマンドキューの作成
	/// @param[in] device デバイスのポインタ
	/// @retval 生成成功時: CommandQueueのshared_ptr, 生成失敗時: nullptr
	static std::shared_ptr<CommandQueue> Create(std::shared_ptr<Device> device);

	/// @fn Create
	/// コマンドキューの作成
	/// @param[in] device デバイスのポインタ
	/// @param[in] commandQueueDesc コマンドキューの設定項目
	/// @retval 生成成功時: CommandQueueのshared_ptr, 生成失敗時: nullptr
	static std::shared_ptr<CommandQueue> Create(std::shared_ptr<Device> device, const D3D12_COMMAND_QUEUE_DESC& commandQueueDesc);

	/// @fn Get
	/// コマンドキューのComポインタを取得する
	/// @retval ID3D12CommandQueueのComPtr
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;

	/// @fn ExecuteCommandLists
	/// コマンドを実行する
	/// @param[in] numCommandLists	:	コマンドリストの個数
	/// @param[in] ppCommandLists	:	コマンドリスト配列へのポインタ
	void ExecuteCommandList(UINT numCommandLists, ID3D12CommandList *const *ppCommandLists);

	/// @fn Signal
	/// コマンドキューの実行を待つ
	void Signal();

private:
	/* 変数宣言 */
	ComPtr<ID3D12CommandQueue> mCommandQueue;	// コマンドリストの実体
	UINT mFenceValue;							// フェンスパラメータ
	ComPtr<ID3D12Fence> mFence;					// フェンスオブジェクト
	HANDLE mFenceEvent;							// フェンスイベント
};

