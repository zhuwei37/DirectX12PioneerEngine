#include"MaterialResource.h"
#include"EngineManager.h"
#include"yaml-cpp/yaml.h"
#include"GUIDHelper.h"
void  MaterialResource::SerializeShaderValue(YAML::Emitter& out,IShaderValue*  ptr,Material* mat)
{
	auto shaderProeprtyType = ptr->GetShaderValueType();
	auto id = ptr->GetShaderPropertyID();
	auto idstr = ShaderProperty::PropertyIDToString(ptr->GetShaderPropertyID());
	switch (shaderProeprtyType)
	{
	case  ShaderValueType::Float:
	{
		ShaderValue<float>* value = dynamic_cast<ShaderValue<float>*>(ptr);
		
		out << YAML::Key << idstr;
		out << YAML::Value << value->GetValue();

		break;
	}
	case ShaderValueType::Float3:
	{
		ShaderValue<DirectX::XMFLOAT3>* value = dynamic_cast<ShaderValue<DirectX::XMFLOAT3>*>(ptr);
		auto float3 = value->GetValue();
		out << YAML::Key << idstr;
		out << YAML::Value;
		out << YAML::BeginSeq;
		out << float3.x << float3.y << float3.z;
		out << YAML::EndSeq;
	    break;
	}
	case ShaderValueType::Float4:
	{
		ShaderValue<DirectX::XMFLOAT4>* value = dynamic_cast<ShaderValue<DirectX::XMFLOAT4>*>(ptr);
		auto float4 = value->GetValue();
		out << YAML::Key << idstr;
		out << YAML::Value;
		out << YAML::BeginSeq;
		out << float4.x << float4.y << float4.z<<float4.w;
		out << YAML::EndSeq;
		break;
	}
	case ShaderValueType::_Texture:
	{
		ShaderTextureValue* value = dynamic_cast<ShaderTextureValue*>(ptr);
		ResourceID resoureceID;

		if (mat->GetTexture(id, resoureceID))
		{
			std::string str = GUIDHelper::ResourceIdToString(resoureceID);
			out << YAML::Key << idstr;
			out << YAML::Value << str;
		}
		break;
	}
	default:break;
	}

}
std::string MaterialResource::Serialize()
{
	
	if (this->mMat == nullptr)
	{
		
		return "";
	}

	std::vector<IShaderValue*> shaderFloatValues;
	std::vector<IShaderValue*> shaderFloat3Values;
	std::vector<IShaderValue*> shaderFloat4Values;
	std::vector<IShaderValue*> shaderTextureValues;
	auto shaderProerties = this->mMat->GetGMat()->mShaderProperties->ShaderValues;
	//for(auto )
	YAML::Emitter out;
	//std::string str = GUIDHelper::ResourceIdToString(this->GetId());
	out << YAML::BeginMap;
	out << YAML::Key << "ShaderProperty";
	//out << YAML::Value << str;
	out << YAML::Value;

	out << YAML::BeginMap;

	out << YAML::Key << "Float";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderFloatProperty : shaderFloatValues)
	{
		SerializeShaderValue(out, shaderFloatProperty, mMat.get());
	}
	out << YAML::EndMap;

	out << YAML::Key << "Float3";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderFloat3Property : shaderFloat3Values)
	{
		SerializeShaderValue(out, shaderFloat3Property, mMat.get());
	}
	out << YAML::EndMap;

	out << YAML::Key << "Float4";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderFloat4Property : shaderFloatValues)
	{
		SerializeShaderValue(out, shaderFloat4Property, mMat.get());
	}
	out << YAML::EndMap;

	out << YAML::Key << "Texture";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderTextureProperty : shaderTextureValues)
	{
		SerializeShaderValue(out, shaderTextureProperty, mMat.get());
	}
	out << YAML::EndMap;

	out << YAML::EndMap;

	out << YAML::EndMap;

	return out.c_str();
}
void MaterialResource::SetValues(Material* mat)
{
	mat->SetRenderShader(this->mTempRenderShaderId);
	for (auto& var : mTempVars)
	{
		switch (var.type)
		{
		case ShaderValueType::Float:
		{
			mat->SetFloat(var.shaderPropertyID, var.Float);
			break;
		}
		case ShaderValueType::Float3:
		{
			DirectX::XMFLOAT3 float3 = { var.Float3[0],var.Float3[1],var.Float3[2] };
			mat->SetFloat3(var.shaderPropertyID, float3);
			break;
		}
		case ShaderValueType::Float4:
		{
			DirectX::XMFLOAT4 float4 = { var.Float4[0],var.Float4[1],var.Float4[2],var.Float4[3]};
			mat->SetFloat4(var.shaderPropertyID, float4);
			break;
		}
		case ShaderValueType::_Texture:
		{
			ResourceID resourceId = var._ResourceID;
			auto resource=EngineManager::Get()->GetResourceSystem()->GetResourceManager()->GetResource(resourceId);
			if (resource != nullptr)
			{
				auto texture=dynamic_cast<TextureResource*>(resource);
				if (texture != nullptr)
				{
					mat->SetTexture(var.shaderPropertyID, texture);
				}
			}
			break;
		}
		default:
			break;
		}
	}
	this->mTempVars.clear();
}
