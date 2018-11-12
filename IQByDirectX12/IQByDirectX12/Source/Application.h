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
#include <vector>
#include <wrl.h>
#include <memory>
#include "Math/Math.h"

class Window;
class Device;
class CommandAllocator;
class CommandQueue;
class RenderTarget;
class DepthBuffer;
class VertexBuffer;
class RootSignature;
class Texture;
class TextureLoader;
class DescriptorHeap;
class ConstantBuffer;
class PMDLoader;
class PMDModelData;
class PMXLoader;
class PMXModelData;
class Model;
class Shader;
class SwapChain;
class Animation;
class Keyboard;
class GraphicsCommandList;
class PipelineStateObject;

class Camera;
class Dx12Camera;

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

	/// @brief		����������
	/// @param[in]	window �E�B���h�E�N���X
	/// @retval		true: ����������I��
	/// @retval		false: ���������s
	bool Initialize(const Window& window);

	/// @brief		�`�揈��
	void Render();

	/// @brief		�I������
	void Terminate();
private:

	/* �萔��` */
	const int RENDER_TARGET_NUM = 3;		//!	�����_�[�^�[�Q�b�g��

	/* �ϐ��錾 */
	std::shared_ptr<SwapChain>				mSwapChain;				//! �X���b�v�`�F�C��
	ComPtr<ID3D12PipelineState>				mPipelineState;			//! �p�C�v���C���X�e�[�g
	std::shared_ptr<GraphicsCommandList>	mCommandList;			//! �R�}���h���X�g
	std::shared_ptr<Device>					mDevice;				//! �f�o�C�X
	std::shared_ptr<CommandQueue>			mCommandQueue;			//! �R�}���h�L���[
	std::shared_ptr<RenderTarget>			mRenderTarget;			//! �����_�[�^�[�Q�b�g
	std::shared_ptr<RootSignature>			mRootSignature;			//! ���[�g�V�O�l�`��
	std::shared_ptr<DepthBuffer>			mDepthBuffer;			//! �[�x�o�b�t�@
	std::vector<D3D12_INPUT_ELEMENT_DESC>	mInputLayoutDescs;
	std::shared_ptr<Shader>					mPixelShaderClass;
	std::shared_ptr<Shader>					mVertexShaderClass;
	std::shared_ptr<PMDLoader>				mModelLoader;
	std::shared_ptr<Model>					mModelData;
	std::shared_ptr<PMXLoader>				mPMXModelLoader;
	std::shared_ptr<Model>					mPMXModelData;
	std::shared_ptr<Animation>				mAnimationData;
	std::vector<std::shared_ptr<Model>>		mInstancingTestModels;

	std::shared_ptr<PipelineStateObject>	mPMDPipelineState;		//! PMD�p�C�v���C���X�e�[�g
	std::shared_ptr<PipelineStateObject>	mPMXPipelineState;		//! PMX�p�C�v���C���X�e�[�g

	std::shared_ptr<Keyboard>			mKeyboard;
	std::shared_ptr<Dx12Camera>			mDx12Camera;

	
	int mWindowWidth;
	int mWindowHeight;

	/* ���[�J�����\�b�h��` */

	/// @brief PMD�pPSO���쐬����
	bool CreatePMDPipelineStateObject();

	/// @brief PMX�pPSO���쐬����
	bool CreatePMXPipelineStateObject();

	/// @fn CreateRootSignature
	/// ���[�g�V�O�l�`���𐶐�����
	/// @retval ture: ��������, false: �������s
	bool CreateRootSignature();

	/// @fn _DebugCreatePMDRootSignature
	bool _DebugCreatePMDRootSignature();

	/// @fn ReadShader
	/// �V�F�[�_��ǂݍ���
	/// @retval true: �Ǎ�����, false: �Ǎ����s
	bool ReadShader();

	bool _DebugReadPMDShader();

	/// @fn CreatePipelineState
	/// �p�C�v���C���X�e�[�g�̍쐬
	/// @retval ture: ��������, false: �������s
	bool CreatePipelineState();

	/// @fn _DebugCreatePMDPipelineState
	/// PMD�p�p�C�v���C���X�e�[�g�̍쐬
	/// @retval true: ��������, false: �������s
	bool _DebugCreatePMDPipelineState();

	/// @fn CreateCommandList
	/// �R�}���h���X�g�̐���
	/// @retval true: ��������, false: �������s
	bool CreateCommandList();

	/// @fn CreateCamera
	/// �J�������쐬����
	void CreateCamera();

	/// @fn LoadPMD
	/// PMD���f���f�[�^�����[�h����
	void LoadPMD();

	/// @fn LoadPMX
	/// PMX���f���f�[�^�����[�h����
	void LoadPMX();

	/// @brief ���[�V�����f�[�^�����[�h����
	void LoadMotion();

	/// @brief �s������X�V����
	void UpdateMatrix();
};

