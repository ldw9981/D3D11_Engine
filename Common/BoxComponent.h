#pragma once
#include "CollisionComponent.h"

class BoxComponent :
    public CollisionComponent
{
public:
    BoxComponent();
    ~BoxComponent();

    BoundingBox  m_Geomety;

    virtual bool IsCollide(CollisionComponent* pOtherComponent) override;
    virtual bool IsCollide(const BoundingOrientedBox& OtherGeometry) override;
    virtual bool IsCollide(const BoundingSphere& OtherGeometry) override;
    virtual bool IsCollide(const BoundingBox& OtherGeometry) override;
    virtual bool IsCollide(const Math::Ray& OtherGeometry, float& Dist) override;
    virtual void Update(float DeltaTime);
};

