#pragma once
#include "Actor.h"

class SkeletalMeshComponent;
class SkeletalMeshActor: public Actor
{
public:
	SkeletalMeshActor();
	~SkeletalMeshActor() {};

	std::shared_ptr<SkeletalMeshComponent> m_pSkeletalMeshComponent;

	virtual void Update(float DeltaTime);
};

