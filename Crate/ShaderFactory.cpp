#include"ShaderFactory.h"
#include <locale>
#include <codecvt>
void ShaderFactory::Reset()
{
	ShaderMap.clear();
}
std::shared_ptr<Shaders> ShaderFactory::ToShaders()
{
	std::shared_ptr<Shaders> shaders=std::make_shared<Shaders>();
	auto it = ShaderMap.find(ShaderType::VS);
	if (it != ShaderMap.end())
	{
	
		shaders->VS =
		{
			reinterpret_cast<BYTE*>(it->second->GetBufferPointer()),
			it->second->GetBufferSize()
		};
	}
	it = ShaderMap.find(ShaderType::PS);
	if (it != ShaderMap.end())
	{
		shaders->PS =
		{
			reinterpret_cast<BYTE*>(it->second->GetBufferPointer()),
			it->second->GetBufferSize()
		};
	}
	shaders->InputLayout=
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	return shaders;

}
void ShaderFactory::SetShader(ShaderType shaderType,const std::string& fileName,const std::string& entryPoint,const std::string& target)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	auto wFilename = converter.from_bytes(fileName);
	auto shader = d3dUtil::CompileShader(wFilename, nullptr, entryPoint, target);
	ShaderMap[shaderType] = shader;
	

}