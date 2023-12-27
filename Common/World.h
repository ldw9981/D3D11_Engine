#pragma once


class Actor;
class World
{
public:
	World();
	~World();
public:

	std::list<std::shared_ptr<Actor>> m_actors;	// 생성된 게임 오브젝트들
	bool m_bIsPlaying = false;
	// 템플릿 함수로 GameObject를 생성한다.
	template<typename T>
	std::shared_ptr<T> CreateGameObject()
	{
		bool bIsBase = std::is_base_of<Actor, T>::value;
		assert(bIsBase == true);
		std::shared_ptr<T> newObject = std::make_shared<T>();
		newObject->SetOwner(this);
		m_actors.push_back(newObject);
		newObject->m_iteratorInWorld = --m_actors.end();

		if(m_bIsPlaying)
			newObject->OnBeginPlay();

		return newObject;
	}

	void DestroyGameObject(Actor* pObject);

	void Update(float DeltaTime);
	void OnBeginPlay();
	void OnEndPlay();
};

