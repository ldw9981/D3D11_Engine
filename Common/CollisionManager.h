#pragma once

class CollisionComponent;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	
		
	std::list<CollisionComponent*> m_CollisionComponents;

public:
	bool Initialize();

	void AddCollisionComponent(CollisionComponent* pCollisionComponent);
	void RemoveCollisionComponent(CollisionComponent* pCollisionComponent);
	void Update(float DeltaTime);
};

