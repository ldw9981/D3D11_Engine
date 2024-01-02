#include "pch.h"
#include "BoxComponent.h"
#include "SphereComponent.h"
#include "OrientedBoxComponent.h"

BoxComponent::BoxComponent()
{
	m_ColliderType = ColliderType::Box;
}

BoxComponent::~BoxComponent()
{

}

bool BoxComponent::IsCollide(CollisionComponent* pOtherComponent)
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

bool BoxComponent::IsCollide(const BoundingOrientedBox& OtherGeometry)
{
	return m_Geomety.Intersects(OtherGeometry);
}

bool BoxComponent::IsCollide(const BoundingSphere& OtherGeometry)
{
	return m_Geomety.Intersects(OtherGeometry);
}

bool BoxComponent::IsCollide(const BoundingBox& OtherGeometry)
{
	return m_Geomety.Intersects(OtherGeometry);
}

bool BoxComponent::IsCollide(const Math::Ray& OtherGeometry)
{
	float Dist;
	return OtherGeometry.Intersects(m_Geomety,Dist);	
}

void BoxComponent::Update(float DeltaTime)
{
	__super::Update(DeltaTime);

	m_Geomety.Center = GetWorldPosition();
}
