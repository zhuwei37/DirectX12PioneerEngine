#pragma once
#define SceneNodeNameMaxLength 50
#pragma pack(push,1)
typedef struct SceneNodeInfo
{
	unsigned int SceneNodeID;
	unsigned int ParentID;
	unsigned int ActorId;
	int ChildCount;
	char Name[SceneNodeNameMaxLength] ;
} SceneNodeInfo;
#pragma pack(pop)