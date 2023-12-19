#include "pch.h"
#include "StaticMeshComponent.h"


StaticMeshComponent::StaticMeshComponent(Actor* pOwner, const std::string& Name)
	:SceneComponent(pOwner,Name)
{

}


StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::Update(float DeltaTime)
{
}

bool StaticMeshComponent::LoadFBX(const std::string& FileName)
{
	return true;
}
