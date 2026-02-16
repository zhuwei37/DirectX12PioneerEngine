#pragma once
#include"tamplates.h"
#include"interface.h"
class ComponentFactory
{
public:
	ComponentFactory();
	IComponent* CreateComponent(const char* componentname);
	IComponent* CreateComponent(ComponentId id);
private:
	GenericObjectFactory<IComponent, ComponentId> m_componentFactory;

};