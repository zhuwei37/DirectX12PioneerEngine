#pragma once
#include"ShaderValue.h"
class ShaderProperties
{
public:
	ShaderProperties(std::shared_ptr<UploadGraphicsData> UploadGraphicsData,
	std::map<ShaderPropertyID,IShaderValue*> shaderValues)
	{
		mUploadGraphicsData = UploadGraphicsData;
		ShaderValues = shaderValues;

	}
	void SetValue(ShaderPropertyID propertyID, Texture* value)
	{
		auto it = ShaderValues.find(propertyID);
		if (it != ShaderValues.end())
		{
			auto target = it->second;
				if (target->GetShaderValueType() == ShaderValueType::_Texture)
				{
					ShaderTextureValue* texValue = dynamic_cast<ShaderTextureValue*>(target);
					if (texValue != nullptr)
					{
						texValue->SetTexture(value);
					}
				}
			
		}
	}
	template<typename T>
	void SetValue(ShaderPropertyID propertyID, T& value, ShaderValueType type)
	{
		auto it = ShaderValues.find(propertyID);
		if (it != ShaderValues.end())
		{
			auto target = it->second;
			
			 if (target->GetShaderValueType() == type)
			{
				ShaderValue<T>* shaderValue = dynamic_cast<ShaderValue<T>*> (target);
				if (shaderValue != nullptr)
				{
					shaderValue->SetValue(value);
				}
			}
		}
	}
	template<typename T>
	int GetValue(ShaderPropertyID propertyID, T*& outValue, ShaderValueType type)
	{
		auto it = ShaderValues.find(propertyID);
		if (it != ShaderValues.end())
		{
			auto value = it->second;
			if (value->GetShaderValueType() == type)
			{
				ShaderValue<T>* shaderValue
					= dynamic_cast<ShaderValue<T>*> (value);
				if (shaderValue != nullptr)
				{
					*outValue = shaderValue->GetValue();
					return 0;
				}
			}
		}
		return -1;
	}
	~ShaderProperties()
	{
		if (!ShaderValues.empty())
		{
			for (auto it : ShaderValues)
			{
				delete it.second;
			}
		}
	}
public:
	inline bool HasCustomData()
	{
		return mUploadGraphicsData != nullptr;
	}
	std::shared_ptr<UploadGraphicsData> mUploadGraphicsData;
private:
public:
	std::map<ShaderPropertyID, IShaderValue*> ShaderValues;
};