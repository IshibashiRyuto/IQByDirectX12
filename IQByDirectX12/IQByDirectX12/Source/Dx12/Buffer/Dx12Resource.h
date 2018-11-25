/**
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
class GraphicsCommandList;

/**	
*	@class	Dx12Resource
*	@brief	ID3D12Resource�̃��b�p�[�N���X
*	@note	Create�ŃI�u�W�F�N�g���쐬�Anew�͕s��
*/
class Dx12Resource
{
public:
	/// @brief	�f�X�g���N�^
	virtual ~Dx12Resource();

	/**
	*	@brief	Dx12Resource�I�u�W�F�N�g�𐶐�����
	*
	*	@param[in]	device		: dx12�f�o�C�X
	*	@param[in]	heapProp	: �q�[�v�v���p�e�B
	*	@param[in]	heapFlag	: �q�[�v�t���O
	*	@param[in]	desc		: ���\�[�X�f�X�N
	*	@param[in]	state		: ���\�[�X�̏����X�e�[�g
	*	@param[in]	clearValue	: �N���A�l(nullptr��)
	*
	*	@retval	��������	: Dx12Resource�̃X�}�[�g�|�C���^
	*	@retval	�������s	: nullptr
	*/
	static std::shared_ptr<Dx12Resource> Create(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE * pClearValue,
		const std::wstring& resourceName = L""
	);

	/**
	*	@brief	���\�[�X���̂��擾����
	*	@retval	���\�[�X���̂�COM�|�C���^
	*/
	const ComPtr<ID3D12Resource> GetResource() const;

	/**
	*	@brief	���\�[�X��ԑJ�ڂ��s��
	*
	*	@param[in]	commandList		: �J�ڃo���A���ڂ���R�}���h���X�g
	*	@param[in]	transitionState	: �J�ڐ���
	*/
	void TransitionState(std::shared_ptr<GraphicsCommandList> commandList, D3D12_RESOURCE_STATES transitionState);


protected:
	/**
	*	@brief	�R���X�g���N�^
	*	
	*	@param[in]	device			: dx12�f�o�C�X
	*	@param[in]	heapProp		: �q�[�v�v���p�e�B
	*	@param[in]	heapFlag		: �q�[�v�t���O
	*	@param[in]	desc			: ���\�[�X�f�X�N
	*	@param[in]	state			: ���\�[�X�̏����X�e�[�g
	*	@param[in]	clearValue		: �N���A�l
	*	@param[in]	resourceName	: ���\�[�X��
	*	@param[out]	result			: �o�b�t�@�̐����ɐ���������
	*/
	Dx12Resource(
		std::shared_ptr<Device> device,
		const D3D12_HEAP_PROPERTIES& heapProp,
		D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		D3D12_RESOURCE_STATES states,
		const D3D12_CLEAR_VALUE* pClearValue,
		const std::wstring& resourceName,
		HRESULT& result);

	/**
	*	@brief	�R���X�g���N�^
	*	@param[in]	resource	: �\�z�ς݃��\�[�X�o�b�t�@
	*/
	Dx12Resource(std::shared_ptr<Dx12Resource> resource);

	/**
	*	@brief	�R���X�g���N�^
	*	@param[in]	resource	: �\�z�ς�ID3D12Resource
	*	@param[in]	state		: ���\�[�X�̃X�e�[�g
	*/
	Dx12Resource(ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state);

	/* �ϐ��錾 */
	ComPtr<ID3D12Resource> mResource;		//! ���\�[�X����
	D3D12_RESOURCE_STATES mCurrentState;	//! ���\�[�X�̏��
};

