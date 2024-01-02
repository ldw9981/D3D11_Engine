#include "pch.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"

CollisionManager* CollisionManager::Instance = nullptr;

CollisionManager::CollisionManager()
{
	assert(Instance == nullptr);
	Instance = this;
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
	pCollisionComponent->m_ItCollisionManager = m_CollisionComponents.insert(m_CollisionComponents.end(), pCollisionComponent);
}

void CollisionManager::RemoveCollisionComponent(CollisionComponent* pCollisionComponent)
{
	m_CollisionComponents.erase(pCollisionComponent->m_ItCollisionManager);
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

bool CollisionManager::Query(const BoundingBox& Input, std::list<CollisionComponent*>& Output)
{	
	for (std::list<CollisionComponent*>::iterator it = m_CollisionComponents.begin(); it != m_CollisionComponents.end(); it++)
	{
		CollisionComponent* pComponent = *it;
		if (pComponent->GetCollisionType() == CollisionType::NoCollision)
			continue;

		if (pComponent->IsCollide(Input))
			Output.push_back(pComponent);
	}
	return !Output.empty();
}

bool CollisionManager::Query(const BoundingSphere& Input, std::list<CollisionComponent*>& Output)
{
	for (std::list<CollisionComponent*>::iterator it = m_CollisionComponents.begin(); it != m_CollisionComponents.end(); it++)
	{
		CollisionComponent* pComponent = *it;
		if (pComponent->GetCollisionType() == CollisionType::NoCollision)
			continue;

		if (pComponent->IsCollide(Input))
			Output.push_back(pComponent);
	}
	return !Output.empty();
}

bool CollisionManager::Query(const BoundingOrientedBox& Input, std::list<CollisionComponent*>& Output)
{
	for (std::list<CollisionComponent*>::iterator it = m_CollisionComponents.begin(); it != m_CollisionComponents.end(); it++)
	{
		CollisionComponent* pComponent = *it;
		if (pComponent->GetCollisionType() == CollisionType::NoCollision)
			continue;

		if (pComponent->IsCollide(Input))
			Output.push_back(pComponent);
	}
	return !Output.empty();
}

bool CollisionManager::Query(const Math::Ray& Input, std::list<RayResult>& Output, bool sort)
{
	for (std::list<CollisionComponent*>::iterator it = m_CollisionComponents.begin(); it != m_CollisionComponents.end(); it++)
	{
		CollisionComponent* pComponent = *it;
		if (pComponent->GetCollisionType() == CollisionType::NoCollision)
			continue;
		
		float Dist = 0.0f;
		if (pComponent->IsCollide(Input, Dist))
		{
			auto item = Output.emplace_back();
			item.Dist = Dist;
			item.pComponent = pComponent;
		}		
	}

	if (sort)
	{
		Output.sort([](const RayResult& A, const RayResult& B) -> bool
			{
				return A.Dist < B.Dist;
			});
	}
	return !Output.empty();
}
