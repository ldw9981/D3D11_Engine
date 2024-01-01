#pragma once

class World;
class IWorldEvent
{
public:
	virtual void OnBeginPlay(World* pWorld)=0;
	virtual void OnEndPlay(World* pWorld)=0;
};

class Actor;
class World
{
public:
	World();
	World(std::string Name);
	~World();
public:
	std::string m_Name="NoName";
	std::list<std::shared_ptr<Actor>> m_Actors;	// 생성된 게임 오브젝트들	
	bool m_bIsPlaying = false;
	IWorldEvent* m_pWorldEvent = nullptr;


	// 템플릿 함수로 GameObject를 생성한다.
	template<typename T>
	std::shared_ptr<T> CreateGameObject()
	{
		bool bIsBase = std::is_base_of<Actor, T>::value;
		assert(bIsBase == true);
		std::shared_ptr<T> newObject = std::make_shared<T>();
		newObject->SetOwner(this);
		newObject->m_iteratorInWorld = m_Actors.insert(m_Actors.end(), newObject);
		return newObject;
	}

	void SetWorldEvent(IWorldEvent* pWorldEvent);
	void DestroyGameObject(Actor* pObject);

	void Update(float DeltaTime);
	void OnBeginPlay();
	void OnEndPlay();
};

