#pragma once
#include "CollisionComponent.h"
class OrientedBoxComponent :
    public CollisionComponent
{
public:
	OrientedBoxComponent();
	~OrientedBoxComponent();


	BoundingOrientedBox  m_Geomety;

	bool IsCollide(CollisionComponent* pOtherComponent);
	void Update(float DeltaTime);
};

