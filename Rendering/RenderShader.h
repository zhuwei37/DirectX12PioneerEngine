#pragma once
#include"PipelineState.h"
#include"RootSignature.h"
#include"ShaderValue.h"
class RenderShader
{
public:
	std::shared_ptr<RootSignature> mRootSignature;
	std::vector<ShaderPass> mShaderPasses;
};