#pragma once
#include<memory>
#include"ResourceType.h"
#include<objbase.h>
using ResourceID= GUID ;
//#define ResourceID int
typedef unsigned int ComponentId;
typedef unsigned int ActorId;
typedef unsigned int SceneId;
typedef unsigned int RenderHostId;
//ComponentId INVALID_COMPONENTID = 0;

class IComponent;
class Actor;
//typedef std::weak_ptr<IComponent> ActorComponentWeakPtr;
typedef std::shared_ptr<IComponent>ActorComponentStrongPtr;
typedef std::shared_ptr<Actor> ActorPtr;

enum Layer
{
	/// <summary>
	/// 背景
	/// </summary>
	Background=0,
	/// <summary>
	/// 默认
	/// </summary>
	Default,
	/// <summary>
	/// 水
	/// </summary>
	Water,
	/// <summary>
	/// 透明
	/// </summary>
	Transparent,
	Sky,
	UI,
	User_2_Layer
};


typedef struct CreateRenderHostParameter
{
	int Width;
	int Height;
	int RenderHostType=0;
	int WindowHandle;
} CreateRenderHostParameter;



//组件列表描述
typedef struct ComponentExportInfo
{
	ComponentId ComponentId=0;
	IComponent* ComponentPtr=nullptr;
}ComponentExportInfo;
//资源信息描述
typedef struct ResourceInfo
{
	ResourceID guid;
	ResourceType resourceType;
	const char* path;
	int childrenCount;
	bool hasChildren;
};
