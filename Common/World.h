#pragma once
#include "Deligate.h"

class World;
class Actor;
class World
{
public:
	World();
	World(std::string Name);
	~World();
public:
	std::string m_Name="NoName";
	std::list<std::shared_ptr<Actor>> m_Actors;	// ������ ���� ������Ʈ��	
	bool m_bIsPlaying = false;
	
	Deligate1<World*> m_OnBeginPlay;
	Deligate1<World*> m_OnEndPlay;


	// ���ø� �Լ��� GameObject�� �����Ѵ�.
	template<typename T>
	std::shared_ptr<T> CreateGameObject()
	{
		bool bIsBase = std::is_base_of<Actor, T>::value;
		assert(bIsBase == true);
		std::shared_ptr<T> newObject = std::make_shared<T>();
		newObject->SetOwner(this);
		newObject->m_iteratorInWorld = m_Actors.insert(m_Actors.end(), newObject);
		newObject->SetName(typeid(T).name() );
		return newObject;
	}

	void DestroyGameObject(Actor* pObject);

	void Update(float DeltaTime);
	void OnBeginPlay();
	void OnEndPlay();
};

