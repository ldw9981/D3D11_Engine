#pragma once
#include "Actor.h"

class SkeletalMeshComponent;
class SkeletalMeshActor: public Actor
{
public:
	SkeletalMeshActor();
	~SkeletalMeshActor() {};

	std::weak_ptr<SkeletalMeshComponent> m_pSkeletalMeshComponent;

};

