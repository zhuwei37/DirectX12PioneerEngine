#include"IComponent.h"
#include"Actor.h"
Layer IComponent::GetLayer()
{
	return this->m_pOwner->GetLayer();
}