#pragma once
#include "CollisionComponent.h"
class SphereComponent :
    public CollisionComponent
{
public:
	SphereComponent();
	~SphereComponent();

	BoundingSphere  m_Geomety;
};

