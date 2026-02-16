#pragma once
#include "ExPort.h"
#include"ResourceSystem.h"
#include"interface.h"
extern "C"
{
	DLL_EXPORTS ResourceInfo GetRootResourceInfo();
	DLL_EXPORTS bool GetChildResourceInfo(ResourceID parentId,ResourceInfo*,__inout int& size);
	DLL_EXPORTS bool CreateResource(ResourceType type, ResourceID parent,char* path,__out ResourceInfo& resourceInfo);
	DLL_EXPORTS bool DeleteResource(ResourceID id, ResourceID parent);


	DLL_EXPORTS int GetInMemeryResourceCount();
	DLL_EXPORTS bool GetAllInMemeryResourceInfos(ResourceInfo*, __inout int& size);
}