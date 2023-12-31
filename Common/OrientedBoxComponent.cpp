#include "pch.h"
#include "OrientedBoxComponent.h"
#include "SphereComponent.h"
#include "BoxComponent.h"

OrientedBoxComponent::OrientedBoxComponent()
{
	m_ColliderType = ColliderType::OrientedBox;
}

OrientedBoxComponent::~OrientedBoxComponent()
{

}

bool OrientedBoxComponent::IsCollide(CollisionComponent* pOtherComponent)
{
	if (pOtherComponent->GetColliderType() == ColliderType::Box)
	{
		BoxComponent* pBoxComponent = static_cast<BoxComponent*>(pOtherComponent);
		return m_Geomety.Intersects(pBoxComponent->m_Geomety);
	}
	else if (pOtherComponent->GetColliderType() == ColliderType::Sphere)
	{
		SphereComponent* pSphereComponent = static_cast<SphereComponent*>(pOtherComponent);
		return m_Geomety.Intersects(pSphereComponent->m_Geomety);
	}
	else if (pOtherComponent->GetColliderType() == ColliderType::OrientedBox)
	{
		OrientedBoxComponent* pOrientedBoxComponent = static_cast<OrientedBoxComponent*>(pOtherComponent);
		return m_Geomety.Intersects(pOrientedBoxComponent->m_Geomety);
	}
	else
	{
		assert(false);
	}
	return false;
}
