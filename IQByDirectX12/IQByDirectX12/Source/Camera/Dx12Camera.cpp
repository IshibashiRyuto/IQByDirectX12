#include "Dx12Camera.h"
#include "../Dx12/DescriptorHeap.h"
#include "../Dx12/ConstantBuffer.h"
#include "../Dx12/GraphicsCommandList.h"
#include "../Dx12/RootSignature.h"

using namespace Math;

/* 定数定義 */
namespace
{
	Math::Vector3 Z_AXIS(0.0f, 0.0f, 1.0f); //! Z軸正の向き
}

Dx12Camera::Dx12Camera(
	const Math::Vector3& pos,
	const Math::Vector3& direction,
	ProjectionType projType,
	const ProjectionParam& projParam,
	std::shared_ptr<Device> device)
	: Camera(pos, direction, projType, projParam)
	, mWorldMatrix(Math::CreateIdent())
	, mCameraDataBuffer(ConstantBuffer::Create(device, sizeof(CameraData)))
	, mCameraHeap(DescriptorHeap::Create(device,1))
{
	UpdateMatrix();
	mCameraHeap->SetConstantBufferView(mCameraDataBuffer->GetConstantBufferView(), 0);
	mCameraHeap->SetBindHeapIndex(0);
}

Dx12Camera::~Dx12Camera()
{
}

std::shared_ptr<Dx12Camera> Dx12Camera::Create(const Math::Vector3 & pos, const Math::Vector3 & direction, ProjectionType projType, const ProjectionParam & projParam, std::shared_ptr<Device> device)
{
	return std::shared_ptr<Dx12Camera>(new Dx12Camera(pos, direction, projType, projParam, device));
}

void Dx12Camera::UpdateMatrix()
{
	if (mIsUpdate)
	{
		// カメラ行列の更新処理
		_UpdateViewMat();

		// カメラ情報をバッファに再セット
		CameraData data;
		data.worldMat = mWorldMatrix;
		data.viewMat = mViewMat;
		data.projMat = mProjMat;
		data.wvpMat = mWorldMatrix * mViewMat * mProjMat;
		data.eyePos = Vector4(mPosition);
		data.eyeDir = Vector4( (mDirection * Math::Quaternion(Z_AXIS) * Math::CreateConjugateQuaternion(mDirection)).v );
		data.right = Vector4( Math::Cross(mUpper, data.eyeDir) );
		data.upper = Vector4( Math::Cross(data.eyeDir, data.right) );
		mCameraDataBuffer->SetData(&data, sizeof(data));
		mIsUpdate = false;
	}
}

void Dx12Camera::SetWorldMatrix(const Math::Matrix4x4 & worldMat)
{
	mWorldMatrix = worldMat;
	mIsUpdate = true;
}

void Dx12Camera::SetCameraData(std::shared_ptr<GraphicsCommandList> commandList, int rootParamIndex)
{
	mCameraHeap->BindGraphicsCommandList(commandList->GetCommandList());
	mCameraHeap->BindRootDescriptorTable(rootParamIndex, 0);
}

void Dx12Camera::BindDescriptorHeap(std::shared_ptr<RootSignature> rootSignature, int rootParamIndex)
{
	rootSignature->SetBindDescriptorHeap(rootParamIndex, mCameraHeap);
}
