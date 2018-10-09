#include <iostream>
#include "Device.h"



Device::Device()
	: mDevice(nullptr)
{
}


Device::~Device()
{
}

std::shared_ptr<Device> Device::Create()
{
	HRESULT result;
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	
	auto device = std::shared_ptr<Device>(new Device());

	for (auto l : levels)
	{
		result = D3D12CreateDevice(nullptr, l, IID_PPV_ARGS(&(device->mDevice)));
		// �f�o�C�X�������o������A�t�B�[�`���[���x����ۑ����ďI��
		if (result == S_OK)
		{
			device->mFeatureLevel = l;
			return device;
		}
	}
#ifdef _DEBUG
	std::cout << "Failed Create Device." << std::endl;
#endif
	return nullptr;
}

ComPtr<ID3D12Device> Device::GetDevice() const
{
	return mDevice;
}

D3D_FEATURE_LEVEL Device::GetFeatureLevel() const
{
	return mFeatureLevel;
}

ID3D12Device * const Device::operator->()
{
	return mDevice.Get();
}
