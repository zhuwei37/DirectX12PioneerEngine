#pragma once
#include"d3dUtil.h"
#include<vector>
#include<wrl.h>
class Shaders
{
public:
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout;
	D3D12_SHADER_BYTECODE VS;
	D3D12_SHADER_BYTECODE PS;
};