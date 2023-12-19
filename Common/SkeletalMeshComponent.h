#pragma once
#include "SceneComponent.h"
class Actor;
class SkeletalMeshComponent :
    public SceneComponent
{
public:
    SkeletalMeshComponent() {}
    SkeletalMeshComponent(Actor* pOwner, const std::string& Name);
	virtual ~SkeletalMeshComponent();

};

