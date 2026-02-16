#pragma once
#include"interface.h"
#include<string>
class UUIDHelper
{
public:
	static std::string ResourceIdToString(ResourceID id)
	{
		return "";
		//return boost::uuids::to_string(id);
	}
	static ResourceID StringToResourceId(const std::string& idStr)
	{
		return {};
		/*static boost::uuids::string_generator generator;
		auto m_id = generator(idStr);
		return m_id;*/
	}
};