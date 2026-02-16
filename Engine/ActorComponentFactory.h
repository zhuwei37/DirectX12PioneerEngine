#pragma once
#include"interface.h"
#include"tamplates.h"
#include<memory>
#include"IComponent.h"
#include"MeshFilter.h"
#include"Material.h"
#include"MeshRenderer.h"
class ActorComponentFactory
{
	public:
		ActorComponentFactory():
			m_componentFactory()
		{
			Register();
		}
		std::shared_ptr<IComponent> CreateComponent(ComponentId id)
		{
			return std::shared_ptr<IComponent>(m_componentFactory.Create(id));
		}
		std::shared_ptr<IComponent> CreateComponent(const char* name)
		{
			ComponentId id = IComponent::GetIdFromName(name);
			return CreateComponent(id);
		}
		
private:
	void Register()
	{
		m_componentFactory.Register<Transform>(IComponent::GetIdFromName("Transform"));
		m_componentFactory.Register<MeshFilter>(IComponent::GetIdFromName("MeshFilter"));
	//	m_componentFactory.Register<Material>(IComponent::GetIdFromName("Material"));
		m_componentFactory.Register<MeshRenderer>(IComponent::GetIdFromName("MeshRenderer"));
	}
	GenericObjectFactory<IComponent, ComponentId> m_componentFactory;
};
