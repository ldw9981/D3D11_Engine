#pragma once
#include "SceneComponent.h"


enum class ColliderType
{
	Box=0,
	Sphere,
	OrientedBox,
	Count
};

enum class CollisionType
{
	NoCollision=0,
	Overlap,
	Block,	
	Count
};

class Actor;
class CollisionComponent;
class ICollisionNotify
{
public:
	virtual void OnBlock(CollisionComponent* pOwnedComponent,Actor* pOtherActor, CollisionComponent* pOtherComponent) = 0;
	virtual void OnBeginOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) = 0;
	virtual void OnEndOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) = 0;
};

class CollisionComponent :
    public SceneComponent
{
	friend class CollisionManager;
public:
	CollisionComponent();
	~CollisionComponent();

	bool m_bIsOverlapped = false;
	ColliderType m_ColliderType = ColliderType::Count;
	CollisionType m_CollisionType = CollisionType::Count;

	std::list<CollisionComponent*>::iterator m_Iterator;
	std::set<CollisionComponent*> m_OverlapStateCurr;    // 현재 충돌 상태
	std::set<CollisionComponent*> m_OverlapStatePrev;	// 이전 충돌 상태
private:
	ICollisionNotify* m_pNotifyListener = nullptr;

public:
	ICollisionNotify* GetNotifyListener() const { return m_pNotifyListener; }
	void SetNotifyListener(ICollisionNotify* val) { m_pNotifyListener = val; }
	ColliderType GetColliderType() const { return m_ColliderType; }	
	CollisionType GetCollisionType() const { return m_CollisionType; }
	virtual bool IsCollide(CollisionComponent* pOtherComponent)=0;

	void ProcessOverlap();
	void ProcessBlock(CollisionComponent* pOtherComponent);

};

