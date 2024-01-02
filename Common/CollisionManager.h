#pragma once

class CollisionComponent;
struct RayResult
{
public:
	float Dist = 0.0f;
	CollisionComponent* pComponent=nullptr;
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	
	static CollisionManager* Instance;
	std::list<CollisionComponent*> m_CollisionComponents;

public:
	bool Initialize();

	void AddCollisionComponent(CollisionComponent* pCollisionComponent);
	void RemoveCollisionComponent(CollisionComponent* pCollisionComponent);
	void Update(float DeltaTime);

	bool Query(const BoundingBox& Input,std::list<CollisionComponent*>& Output);
	bool Query(const BoundingSphere& Input,std::list<CollisionComponent*>& Output);
	bool Query(const BoundingOrientedBox& Input,std::list<CollisionComponent*>& Output);
	bool Query(const Math::Ray& Input,std::list<RayResult>& Output,bool sort);
};

