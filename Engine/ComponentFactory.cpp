#include"ComponentFactory.h"
#include"MeshFilter.h"
#include"MeshRenderer.h"
#include"Material.h"
ComponentFactory::ComponentFactory()
{
	m_componentFactory.Register<MeshRenderer>(IComponent::GetIdFromName("MeshRenderer"));
	m_componentFactory.Register<MeshFilter>(IComponent::GetIdFromName("MeshFilter"));
	m_componentFactory.Register<Transform>(IComponent::GetIdFromName("Transform"));
	//m_componentFactory.Register<Material>(IComponent::GetIdFromName("Material"));
}
IComponent* ComponentFactory::CreateComponent(const char* name)
{
	auto id=IComponent::GetIdFromName(name);
	return CreateComponent(id);
}
IComponent* ComponentFactory::CreateComponent(ComponentId id)
{
	auto component=m_componentFactory.Create(id);
	return  component;
}