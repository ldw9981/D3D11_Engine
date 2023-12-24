#pragma once

class Actor;
class World
{
public:
	World();
	~World();
public:

	std::list<std::shared_ptr<Actor>> m_actors;	// ������ ���� ������Ʈ��
	bool m_bIsPlaying = false;
	// ���ø� �Լ��� GameObject�� �����Ѵ�.
	template<typename T>
	T* CreateGameObject()
	{
		bool bIsBase = std::is_base_of<Actor, T>::value;
		assert(bIsBase == true);
		T* newObject = std::make_shared<T>();
		newObject->SetOwner(this);
		m_actors.push_back(newObject);
		if(m_bIsPlaying)
			newObject->OnBeginPlay();

		return newObject;
	}

	void Update(float DeltaTime);
	void OnBeginPlay();
	void OnEndPlay();
};

