#include"MaterialResource.h"
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
