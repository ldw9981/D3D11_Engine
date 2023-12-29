#include "pch.h"
#include "CollisionComponent.h"
#include "D3DRenderManager.h"

CollisionComponent::CollisionComponent()
{
	D3DRenderManager::Instance->AddCollisionComponent(this);
}

CollisionComponent::~CollisionComponent()
{
	D3DRenderManager::Instance->RemoveCollisionComponent(this);
}
