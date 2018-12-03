/**
*	@file	GrpahicsCommandList.h
*	@brief	GrahpicsCommandListの宣言を記述する
*	@author	Ishibashi Ryuto
*	@date	2018/09/18	初版作成
*/
#pragma once
/*システムヘッダインクルード*/
#include <memory>
#include <string>
#include <d3d12.h>
#include <wrl.h>

/*クラス使用宣言*/
class CommandAllocator;
class Device;
using Microsoft::WRL::ComPtr;

/**
*	@brief	GrahpicsCommandListのラッパークラス
*	@note	現状、リスト1つに対してアロケータを1つしか用意していないため、
*			フェンス処理が必要な点に注意
*/
class GraphicsCommandList
{
public:
	/**
	*	@brief デストラクタ
	*/
	~GraphicsCommandList();

	/**
	*	@brief		オブジェクトの生成処理
	*
	*	@param[in]	device			: デバイス
	*	@param[in]	commandListType : コマンドリストタイプ
	*	@param[in]	name			: コマンドリストの名前
	*
	*	@note		GraphicsCommandListクラスはこのメソッドを通じてのみ生成可能
	*/
	static std::shared_ptr<GraphicsCommandList> Create(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring& name = L"");

	/**
	*	@brief コマンドリストを取得する
	*/
	ComPtr<ID3D12GraphicsCommandList1> GetCommandList() const;


	/**
	*	@brief コマンドリストをクローズする
	*/
	void Close();

	/**
	*	@brief コマンドリストをリセットする
	*	@note 要コマンドクローズ & 実行 & 実行待ち
	*/
	void Reset();
	void Reset(ComPtr<ID3D12PipelineState> pipelineState);

	/**
	*	@brief コマンドリストへのアクセスを行うオペレータオーバーロード
	*/
	ID3D12GraphicsCommandList1* const operator->();

private:
	/*変数宣言*/
	ComPtr<ID3D12GraphicsCommandList1> mCommandList;			// コマンドリストの実体へのポインタ
	std::shared_ptr<CommandAllocator> mCommandAllocator;	// コマンドアロケータ


	/**
	*	@brief コンストラクタ
	*
	*	@param[in]	device			: デバイス
	*	@param[in]	commandListType	: コマンドリストの種類
	*	@param[in]	name			: コマンドリスト名
	*/
	GraphicsCommandList(std::shared_ptr<Device> device, D3D12_COMMAND_LIST_TYPE commandListType, const std::wstring & name);
};

