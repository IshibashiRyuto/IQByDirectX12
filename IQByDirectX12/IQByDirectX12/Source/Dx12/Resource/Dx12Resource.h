/*
*	@file	Dx12Resource.h
*	@brief	ID3D12Resource�̃��b�s���O�N���X���L�q����
*	@author	Ishibashi Ryuto
*	@date	2018/11/16	���ō쐬
*/

#pragma once
/* �w�b�_�C���N���[�h */
#include <memory>
#include <d3d12.h>
#include <wrl.h>
#include <string>

/* �N���X�g�p�錾 */
using Microsoft::WRL::ComPtr;
class Device;

///	@class	Dx12Resource
/// @brief	ID3D12Resource�̃��b�p�[�N���X
///	@note	Create�ŃI�u�W�F�N�g���쐬�Anew�͕s��
class Dx12Resource
{
public:
	/// @brief	�R���X�g���N�^
	/// @param[in]	device		: dx12�f�o�C�X
	///	@param[in]	heapFlag	: �q�[�v�t���O
	/// @param[in]	heapProp	: �q�[�v�v���p�e�B
	/// @param[in]	desc		: ���\�[�X�f�X�N
	/// @param[in]	state		: ���\�[�X�̏����X�e�[�g
	/// @param[in]	clearValue	: �N���A�l
	/// @param[out]	result		: 
	Dx12Resource(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE& clearValue,
		HRESULT& result);

	/// @brief	�f�X�g���N�^
	~Dx12Resource();

	///	@brief	Dx12Resource�I�u�W�F�N�g�𐶐�����
	///
	/// @param[in]	device		: dx12�f�o�C�X
	/// @param[in]	heapProp	: �q�[�v�v���p�e�B
	///	@param[in]	heapFlag	: �q�[�v�t���O
	/// @param[in]	desc		: ���\�[�X�f�X�N
	/// @param[in]	state		: ���\�[�X�̏����X�e�[�g
	/// @param[in]	clearValue	: �N���A�l
	///
	/// @retval	��������	: Dx12Resource�̃X�}�[�g�|�C���^
	/// @retval	�������s	: nullptr
	std::shared_ptr<Dx12Resource> Create(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE& clearValue,
		const std::wstring& resourceName = L""
	);

	/// @brief	���\�[�X���̂��擾����
	/// @retval	���\�[�X���̂�COM�|�C���^
	const ComPtr<ID3D12Resource> GetResource() const;

private:

	/* �ϐ��錾 */
	ComPtr<ID3D12Resource> mResource;	//! ���\�[�X����
};

