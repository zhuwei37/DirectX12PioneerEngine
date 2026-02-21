#include "MaterialProcessor.h"
#include"FileSystem.h"
#include"GUIDHelper.h"
#include"EngineManager.h"
#include"MaterialResource.h"
#include"MaterialContexts.h"
#include"yaml-cpp/emitter.h"

void MaterialProcessor::SerializeShaderValue(YAML::Emitter& out, IShaderValue* ptr, Material* mat)
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
		out << float4.x << float4.y << float4.z << float4.w;
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

std::string MaterialProcessor::Serialization(std::shared_ptr<IResource> resource)
{
	std::shared_ptr<MaterialResource> mMatResource = std::dynamic_pointer_cast<MaterialResource>(resource);
	auto mMat = mMatResource->GetMaterial();
	if (mMat == nullptr)
	{

		return "";
	}
	auto shaderId = mMat->GetRenderShader();
	std::vector<IShaderValue*> shaderFloatValues;
	std::vector<IShaderValue*> shaderFloat3Values;
	std::vector<IShaderValue*> shaderFloat4Values;
	std::vector<IShaderValue*> shaderTextureValues;
	auto shaderProerties = mMat->GetGMat()->mShaderProperties->ShaderValues;
	for (auto value : shaderProerties)
	{

		auto type = value.second->GetShaderValueType();
		switch (type)
		{
		case ShaderValueType::Float:
			{
			shaderFloatValues.push_back(value.second);
				break;
			}
		case ShaderValueType::Float3:
		{
			shaderFloat3Values.push_back(value.second);
			break;
		}
		case ShaderValueType::Float4:
		{
			shaderFloat4Values.push_back(value.second);
			break;
		}
		case ShaderValueType::_Texture:
		{
			shaderTextureValues.push_back(value.second);
			break;
		}
		}
	}
	YAML::Emitter out;
	//std::string str = GUIDHelper::ResourceIdToString(this->GetId());
	out << YAML::BeginMap;

	out << YAML::Key << "RenderShaderId";
	out << YAML::Value << shaderId;


	out << YAML::Key << "ShaderProperty";
	//out << YAML::Value << str;
	out << YAML::Value;

	out << YAML::BeginMap;

	out << YAML::Key << "Float";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderFloatProperty : shaderFloatValues)
	{
		SerializeShaderValue(out, shaderFloatProperty, mMat);
	}
	out << YAML::EndMap;

	out << YAML::Key << "Float3";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderFloat3Property : shaderFloat3Values)
	{
		SerializeShaderValue(out, shaderFloat3Property, mMat);
	}
	out << YAML::EndMap;

	out << YAML::Key << "Float4";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderFloat4Property : shaderFloatValues)
	{
		SerializeShaderValue(out, shaderFloat4Property, mMat);
	}
	out << YAML::EndMap;

	out << YAML::Key << "Texture";
	out << YAML::Value;
	out << YAML::BeginMap;
	for (auto& shaderTextureProperty : shaderTextureValues)
	{
		SerializeShaderValue(out, shaderTextureProperty, mMat);
	}
	out << YAML::EndMap;

	out << YAML::EndMap;

	out << YAML::EndMap;
	std::string   data= out.c_str();
	return data;
}

std::shared_ptr<IResource> MaterialProcessor::Deserialization(std::string& data, std::shared_ptr<MaterialResource> materialResource)
{
	YAML::Node node = YAML::Load(data);
	
	try
	{
		if (node.IsMap())
		{
			if (node["RenderShaderId"].IsDefined())
			{
				int renderShaderId = node["RenderShaderId"].as<int>();
				
				
				materialResource->mTempRenderShaderId = renderShaderId;

				if (node["ShaderProperty"].IsDefined())
				{
					auto shaderProperty = node["ShaderProperty"];
					
					if (shaderProperty.IsMap())
					{
						if (shaderProperty["Float"].IsDefined())
						{
							auto floatValues = shaderProperty["Float"];
							for (auto floatItem : floatValues)
							{
								auto shaderPropertyName= floatItem.first.as<std::string>();
								auto value = floatItem.second.as<float>();
								ShaderPropertyID id = ShaderProperty::PropertyToID(shaderPropertyName);
								//赋值
								MaterialResource::Var var;
								var.type = ShaderValueType::Float;
								var.Float = value;
								var.shaderPropertyID = id;
								materialResource->mTempVars.push_back(var);
							
							}

						}
						if (shaderProperty["Float3"].IsDefined())
						{
							auto float3Values = shaderProperty["Float3"];
							for (auto float3Item : float3Values)
							{
								auto shaderPropertyName = float3Item.first.as<std::string>();
								auto values = float3Item.second;

								ShaderPropertyID id = ShaderProperty::PropertyToID(shaderPropertyName);
								if (values.IsSequence())
								{
									if (values.size() == 3)
									{
										float x = values[0].as<float >();
										float y = values[1].as<float>();
										float z = values[2].as<float>();
										//赋值
										MaterialResource::Var var;
										var.type = ShaderValueType::Float3;
										var.Float3[0] = x;
										var.Float3[1] = y;
										var.Float3[2] = z;
										var.shaderPropertyID = id;
										materialResource->mTempVars.push_back(var);
									}
									
								}
								

							}
						}
						if (shaderProperty["Float4"].IsDefined())
						{
							auto float4Values = shaderProperty["Float4"];
							for (auto float4Item : float4Values)
							{
								auto shaderPropertyName = float4Item.first.as<std::string>();
								auto values = float4Item.second;

								ShaderPropertyID id = ShaderProperty::PropertyToID(shaderPropertyName);
								if (values.IsSequence())
								{
									if (values.size() == 4)
									{
										float x = values[0].as<float >();
										float y = values[1].as<float>();
										float z = values[2].as<float>();
										float w = values[3].as<float>();
										//赋值
										MaterialResource::Var var;
										var.type = ShaderValueType::Float4;
										var.Float4[0] = x;
										var.Float4[1] = y;
										var.Float4[2] = z;
										var.Float4[3] = w;
										var.shaderPropertyID = id;
										materialResource->mTempVars.push_back(var);
									}

								}

							}

						}
						if (shaderProperty["Texture"].IsDefined())
						{
							auto textureValues = shaderProperty["Texture"];
							for (auto textureItem : textureValues)
							{
								auto shaderPropertyName = textureItem.first.as<std::string>();
								auto value = textureItem.second.as<std::string>();

								ShaderPropertyID id = ShaderProperty::PropertyToID(shaderPropertyName);
								ResourceID resourceID = GUIDHelper::StringToResourceId(value);
								//赋值
								MaterialResource::Var var;
								var.type = ShaderValueType::_Texture;
								var._ResourceID = resourceID;
								var.shaderPropertyID = id;
								materialResource->mTempVars.push_back(var);//赋值

							}

						}

					}

				}
			}
}
	}
	catch(const YAML::InvalidNode& error)
	{

	}
	return nullptr;

}








std::shared_ptr<IResource> MaterialProcessor::CreateResource(boost::filesystem::path& path)
{
	std::shared_ptr<MaterialResource> mr = std::make_shared<MaterialResource>();
	boost::filesystem::path curPath = path;
	auto descriptionFilePath = GetDescriptionFilePath(curPath);
	if (FileSystem::Create_File(curPath))
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "RenderShaderId";
		out << YAML::Value << 0;
		out << YAML::EndMap;
		auto matData = out.c_str();
		FileSystem::WriteFile(curPath, matData);
		if (FileSystem::Create_File(descriptionFilePath))
		{
			auto new_id = IResource::NewID();
			auto guid = GUIDHelper::ResourceIdToString(new_id);
			YAML::Emitter out;
			out << YAML::BeginMap;
			out << YAML::Key << "resourceid";
			out << YAML::Value << guid;
			out << YAML::EndMap;
			auto metaString = out.c_str();;
			FileSystem::WriteFile(descriptionFilePath, metaString);
			auto p = path.string();
			auto newResource = std::make_shared< MaterialResource>( new_id);
			newResource->mTempRenderShaderId = 0;
			newResource->SetDirty(true);
			newResource->SetPath(p);
			return newResource;
		}
	}
    return mr;
}
std::shared_ptr<IResource> MaterialProcessor::CreateResource()
{
	std::shared_ptr<MaterialResource> mr = std::make_shared<MaterialResource>();
	return mr;
}
void MaterialProcessor::SaveResource(std::shared_ptr<IResource> resource)
{
	boost::filesystem::path curPath=resource->GetPath();
    auto data=this->Serialization(resource);
	FileSystem::WriteFile(curPath, data);

}

std::shared_ptr<IResource> MaterialProcessor::LoadResource(boost::filesystem::path& path, ResourceManager*)
{
	boost::filesystem::path curPath = path;
	auto descriptionFilePath = GetDescriptionFilePath(curPath);
	if (!FileSystem::IsExist(descriptionFilePath))
	{
		if (FileSystem::Create_File(descriptionFilePath))
		{
			auto id = IResource::NewID();
			auto guid = GUIDHelper::ResourceIdToString(id);
			YAML::Emitter meta;
			meta << YAML::BeginMap;
			meta << YAML::Key << "resourceid";
			meta<<YAML::Value<< guid;
			meta << YAML::EndMap;
			auto metaString = meta.c_str();
			FileSystem::WriteFile(descriptionFilePath, metaString);
		}
		else
		{
			return nullptr;
		}
	}
	std::string data;
	data = FileSystem::ReadFile(descriptionFilePath);
	auto meta =YAML::Load(data);
	auto guidStr = meta["resourceid"].as<std::string>();
	auto guid = GUIDHelper::StringToResourceId(guidStr);
	auto pathstring = path.string();
	auto name = path.filename().string();



	//this->Deserialization();
	std::shared_ptr<MaterialResource> materialResource = std::make_shared<MaterialResource>(guid);
	materialResource->SetPath(pathstring);
	std::string materialData = FileSystem::ReadFile(curPath);
	this->Deserialization(materialData, materialResource);
	
	return materialResource;
}

void MaterialProcessor::DeleteResource(IResource*)
{
}

bool MaterialProcessor::DeleteResource(boost::filesystem::path& path)
{
    return false;
}
