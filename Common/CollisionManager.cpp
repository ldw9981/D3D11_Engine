#include "pch.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

bool CollisionManager::Initialize()
{
	return true;
}

void CollisionManager::AddCollisionComponent(CollisionComponent* pCollisionComponent)
{
	pCollisionComponent->m_Iterator = m_CollisionComponents.insert(m_CollisionComponents.end(), pCollisionComponent);
}

void CollisionManager::RemoveCollisionComponent(CollisionComponent* pCollisionComponent)
{
	m_CollisionComponents.erase(pCollisionComponent->m_Iterator);
}

void CollisionManager::Update(float DeltaTime)
{	

	for (std::list<CollisionComponent*>::iterator it_source = m_CollisionComponents.begin(); it_source != m_CollisionComponents.end(); it_source++)
	{
		CollisionComponent* pSource = *it_source;		
		if (pSource->GetCollisionType() == CollisionType::NoCollision)
			continue; 

		pSource->m_OverlapStatePrev = pSource->m_OverlapStateCurr;
		pSource->m_OverlapStateCurr.clear();

		for (std::list<CollisionComponent*>::iterator it_target = std::next(it_source, 1); it_target != m_CollisionComponents.end(); it_target++)
		{
			CollisionComponent* pTarget = *it_target;
			if (pTarget->GetCollisionType() == CollisionType::NoCollision)
				continue;

			// 같은 오브젝트끼리는 충돌하지 않는다.
			if (pSource->GetOwner() == pTarget->GetOwner())
				continue;

			if (pSource->IsCollide(pTarget) == false)
				continue;
		
			if (pSource->GetCollisionType() == CollisionType::Block && pTarget->GetCollisionType() == CollisionType::Block)
			{
				if(pSource->m_pNotifyListener)
					pSource->ProcessBlock(pTarget);

				if (pTarget->m_pNotifyListener)
					pTarget->ProcessBlock(pSource);
			}
			else // Block and Overlap   or Overlap and Overlap			
			{
				pSource->m_OverlapStateCurr.insert(pTarget);
				pTarget->m_OverlapStateCurr.insert(pSource);

				if (pSource->m_pNotifyListener)
					pSource->ProcessOverlap();

				if (pTarget->m_pNotifyListener)
					pTarget->ProcessOverlap();
			}		
		}
	}
}
