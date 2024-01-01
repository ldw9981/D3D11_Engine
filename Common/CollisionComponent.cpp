#include "pch.h"
#include "CollisionComponent.h"
#include "D3DRenderManager.h"
#include "CollisionManager.h"
#include "Actor.h"

CollisionComponent::CollisionComponent()
{
	D3DRenderManager::Instance->AddCollisionComponent(this);
	CollisionManager::Instance->AddCollisionComponent(this);
}

CollisionComponent::~CollisionComponent()
{
	CollisionManager::Instance->RemoveCollisionComponent(this);
	D3DRenderManager::Instance->RemoveCollisionComponent(this);
}

void CollisionComponent::ProcessOverlap()
{
	assert(m_pNotifyListener!=nullptr);
	
	// 현재는 충돌 이지만 이전 상태에 업으면 Begin
	for (auto& pColliderComponent : m_OverlapStateCurr)
	{
		auto it = m_OverlapStatePrev.find(pColliderComponent);
		if (it == m_OverlapStatePrev.end())
		{	//게임 오브젝트에 알린다.

			m_pNotifyListener->OnBeginOverlap(this, (*it)->GetOwner(), pColliderComponent);
		}
	}
	// 이전에는 충돌 상태지만 현재 없으면 End
	for (auto& pColliderComponent : m_OverlapStatePrev)
	{
		auto it = m_OverlapStateCurr.find(pColliderComponent);
		if (it == m_OverlapStateCurr.end())
		{	//게임 오브젝트에 알린다.
			m_pNotifyListener->OnEndOverlap(this, (*it)->GetOwner(), pColliderComponent);
		}
	}
}

void CollisionComponent::ProcessBlock(CollisionComponent* pOtherComponent)
{
	assert(m_pNotifyListener != nullptr);	
	m_pNotifyListener->OnBlock(this, pOtherComponent->GetOwner(),pOtherComponent);
}
