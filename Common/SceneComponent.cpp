#include "pch.h"
#include "SceneComponent.h"


SceneComponent::SceneComponent(Actor* pOwner, const std::string& Name)
	:Component(pOwner,Name)
{	

}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::Update(float DeltaTime)
{

}
