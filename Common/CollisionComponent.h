#pragma once
#include "SceneComponent.h"

enum class CollisionType
{
	Box=0,
	Sphere,
	OrientedBox,
	Count
};

class CollisionComponent :
    public SceneComponent
{
public:
	CollisionComponent();
	~CollisionComponent();

	bool m_bIsOverlapped = false;
	std::list<CollisionComponent*>::iterator m_Iterator;
	CollisionType m_Type = CollisionType::Count;
	CollisionType GetType() const { return m_Type; }	
};

