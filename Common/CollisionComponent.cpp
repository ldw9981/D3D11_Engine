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
	
	// ����� �浹 ������ ���� ���¿� ������ Begin
	for (auto& pColliderComponent : m_OverlapStateCurr)
	{
		auto it = m_OverlapStatePrev.find(pColliderComponent);
		if (it == m_OverlapStatePrev.end())
		{	//���� ������Ʈ�� �˸���.

			m_pNotifyListener->OnBeginOverlap(this, (*it)->GetOwner(), pColliderComponent);
		}
	}
	// �������� �浹 �������� ���� ������ End
	for (auto& pColliderComponent : m_OverlapStatePrev)
	{
		auto it = m_OverlapStateCurr.find(pColliderComponent);
		if (it == m_OverlapStateCurr.end())
		{	//���� ������Ʈ�� �˸���.
			m_pNotifyListener->OnEndOverlap(this, (*it)->GetOwner(), pColliderComponent);
		}
	}
}

void CollisionComponent::ProcessBlock(CollisionComponent* pOtherComponent)
{
	assert(m_pNotifyListener != nullptr);	
	m_pNotifyListener->OnBlock(this, pOtherComponent->GetOwner(),pOtherComponent);
}
