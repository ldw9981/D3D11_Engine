#pragma once
#include "CollisionComponent.h"

class BoxComponent :
    public CollisionComponent
{
public:
    BoxComponent();
    ~BoxComponent();

    BoundingBox  m_Geomety;

    virtual bool IsCollide(CollisionComponent* pOtherComponent);
    virtual void Update(float DeltaTime);
};

