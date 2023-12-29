#pragma once
#include "CollisionComponent.h"
class OrientedBoxComponent :
    public CollisionComponent
{
public:
	OrientedBoxComponent();
	~OrientedBoxComponent();


	BoundingOrientedBox  m_Geomety;
};

