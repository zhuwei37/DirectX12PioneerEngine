#pragma once
#include"d3dUtil.h"
#include<memory>
#include<map>
#include"Shaders.h"
#include<string>
#include<wrl.h>
using namespace Microsoft::WRL;
enum ShaderType
{
	VS,
	HS,
	CS,
	PS,
	COUNT
};
class ShaderFactory
{
public:
	void SetShader(ShaderType shaderType,const std::string &fileName,const std::string &entryPoint,const std::string& target );
	void Reset();
	std::shared_ptr<Shaders> ToShaders();
private:
	std::map<ShaderType, ComPtr<ID3DBlob>> ShaderMap;
};