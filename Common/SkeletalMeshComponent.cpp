#include "pch.h"
#include "SkeletalMeshComponent.h"
#include "SceneComponent.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* pOwner, const std::string& Name)
:SceneComponent(pOwner,Name)
{
}


SkeletalMeshComponent::~SkeletalMeshComponent()
{
}
