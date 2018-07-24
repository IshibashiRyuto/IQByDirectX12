/*
	Application.h
	�A�v���P�[�V�����̃R�A����
	@author Ishibashi Ryuto
	@history
	2018/07/11 ���ō쐬
*/
#pragma once
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <vector>
#include <wrl.h>
#include <memory>

class Window;
class Device;
class CommandAllocator;
class CommandQueue;
class RenderTarget;
class DepthBuffer;
class VertexBuffer;

using Microsoft::WRL::ComPtr;

struct Vertex
{
	float position[3];
	float normal[3];
	float uv[2];
};

class Application
{
public:
	Application();
	~Application();

	/// @fn Initialize
	/// ����������
	/// @param window �E�B���h�E�N���X
	/// @retval ����������������
	/// true: ����, false: ���s
	bool Initialize(const Window& window);

	/// @fn Render
	/// �`�揈��
	void Render();

private:
	/* �萔��` */
	const int RENDER_TARGET_NUM = 2;

	/* �ϐ��錾 */
	ComPtr<IDXGIFactory4>				mFactory;
	ComPtr<IDXGISwapChain3>				mSwapChain;
	ComPtr<ID3D12RootSignature>			mRootSignature;
	ComPtr<ID3D12Fence>					mFence;
	ComPtr<ID3DBlob>					mVertexShader;
	ComPtr<ID3DBlob>					mPixelShader;
	ComPtr<ID3D12PipelineState>			mPipelineState;
	ComPtr<ID3D12GraphicsCommandList>	mCommandList;
	std::shared_ptr<Device>				mDevice;
	std::shared_ptr<CommandAllocator>	mCommandAllocator;
	std::shared_ptr<CommandQueue>		mCommandQueue;
	std::shared_ptr<RenderTarget>		mRenderTarget;
	std::shared_ptr<DepthBuffer>		mDepthBuffer;
	std::shared_ptr<VertexBuffer>		mVertexBuffer;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayoutDescs;

	D3D12_STATIC_SAMPLER_DESC mStaticSamplerDesc;
	int mWindowWidth;
	int mWindowHeight;
	int mFenceValue;

	/* ���[�J�����\�b�h��` */

	/// @fn CreateSwapChain
	/// �X���b�v�`�F�C���𐶐�����
	/// @retval true: ��������, false: �������s
	bool CreateSwapChain(const Window& window);

	/// @fn CreateRootSignature
	/// ���[�g�V�O�l�`���𐶐�����
	/// @retval ture: ��������, false: �������s
	bool CreateRootSignature();

	/// @fn CreateFence
	/// �t�F���X�𐶐�����
	/// @retval true: ��������, false: �������s
	bool CreateFence();

	/// @fn ReadShader
	/// �V�F�[�_��ǂݍ���
	/// @retval true: �Ǎ�����, false: �Ǎ����s
	bool ReadShader();

	/// @fn CreatePipelineState
	/// �p�C�v���C���X�e�[�g�̍쐬
	/// @retval ture: ��������, false: �������s
	bool CreatePipelineState();

	/// @fn CreateCommandList
	/// �R�}���h���X�g�̐���
	/// @retval true: ��������, false: �������s
	bool CreateCommandList();
};

