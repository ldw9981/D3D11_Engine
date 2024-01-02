#pragma once
#include "CollisionComponent.h"
class SphereComponent :
    public CollisionComponent
{
public:
	SphereComponent();
	~SphereComponent();

	BoundingSphere  m_Geomety;

	virtual bool IsCollide(CollisionComponent* pOtherComponent) override;
	virtual bool IsCollide(const BoundingOrientedBox& OtherGeometry) override;
	virtual bool IsCollide(const BoundingSphere& OtherGeometry) override;
	virtual bool IsCollide(const BoundingBox& OtherGeometry) override;
	virtual bool IsCollide(const Math::Ray& OtherGeometry, float& Dist) override;
	virtual void Update(float DeltaTime) override;
};

