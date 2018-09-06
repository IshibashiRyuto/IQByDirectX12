#include "Shader.h"
#include <iostream>
#include <D3DCompiler.h>
#include <d3dx12.h>


Shader::Shader()
{
}


Shader::~Shader()
{
}

std::shared_ptr<Shader> Shader::Create(const std::wstring & filePath, const std::string & entryPoint, const std::string & target)
{
	auto shader = std::shared_ptr<Shader>(new Shader());
	shader->LoadShader(filePath, entryPoint, target);
	return shader;
}

const D3D12_SHADER_BYTECODE & Shader::GetShaderByteCode() const
{
	return mShaderByteCode;
}

void Shader::LoadShader(const std::wstring & filePath, const std::string & entryPoint, const std::string & compileTarget)
{
	ComPtr<ID3DBlob> shaderCode;
	UINT compileOption = 0;

#ifdef _DEBUG
	compileOption |= D3DCOMPILE_DEBUG;
	compileOption |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	auto result = D3DCompileFromFile(filePath.c_str(),
		nullptr,
		nullptr,
		entryPoint.c_str(),
		compileTarget.c_str(),
		compileOption,
		0,
		&shaderCode,
		nullptr);

	if (FAILED(result))
	{
		std::cout << "Failed Load Shader \" " << filePath.c_str() << "\"." << std::endl;
		return;
	}

	mShaderByteCode = CD3DX12_SHADER_BYTECODE(shaderCode.Get());
}
