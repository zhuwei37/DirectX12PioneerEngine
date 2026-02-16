#pragma once
#include<map>
#include<string>
#include<wrl.h>
#include"RenderShader.h"
#include"Shaders.h"
using namespace std;
using namespace Microsoft::WRL;
#define RENDER_SHADER_ID_MIN 0

#define DEFUALT_RENDER_SHADER_ID 0
#define SOLIDCOLOR_RENDER_SHADER_ID 1

#define DEFERRED_RENDER_SHADER_ID 3
#define SCREEN_QUAD_RENDER_SHADER_ID 4
#define DEFERRED_LIGHT_RENDER_SHADER_ID 5
#define PBR_DEFERRED_RENDER_SHADER_ID 6



#define SHADOW_MAP_RENDER_SHADER_ID 25
#define SKYBOX_RENDER_SHADER_ID 26


#define RENDER_SHADER_ID_MAX 30


#define DEFERRED_SHADER_ID "DeferredShader"
#define DEFUALT_SHADERS_ID "DefualtShader"
#define SOLIDCOLOR_SHADER_ID "SolidColorShader"
#define SKYBOX_SHADER_ID "SkyBoxShader"
#define SCREEN_QUAD_SHADER_ID "ScreenQuadShader"
#define DEFERRED_LIGHT_SHADER_ID "DeferredLightShader"
#define PBR_DEFERRED_SHADER_ID "PBRDeferredShader"
#define SHADOW_MAP_SHADER_ID "ShadowMapShader"
class ShaderManager
{
public :
	static ShaderManager* Get()
	{
		static ShaderManager instance;
		return &instance;
	}

	shared_ptr<Shaders> GetShaders(string name);
	std::shared_ptr<RenderShader> GetRenderShader(int renderShaderId);
private :

	ShaderManager() :
		mShadersMap(),
		mShaderDataMap()
	{
		init();
	}
	ComPtr<ID3DBlob> LoadShaderData(string filename ,string,string);
	void LoadShaders();
	void LoadRenderShaders();
	void init();
	map<string,shared_ptr< Shaders>> mShadersMap;
	map<int, std::shared_ptr<RenderShader>> mRenderShaderMap;
	map<string, ComPtr<ID3DBlob>> mShaderDataMap;
};