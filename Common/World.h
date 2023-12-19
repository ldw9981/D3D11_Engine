#pragma once

class Actor;
class World
{
public:
	World();
	~World();
public:

	std::list<std::shared_ptr<Actor>> m_GameObjects;	// ������ ���� ������Ʈ��

	// ���ø� �Լ��� GameObject�� �����Ѵ�.
	template<typename T>
	T* CreateGameObject()
	{
		bool bIsBase = std::is_base_of<Actor, T>::value;
		assert(bIsBase == true);
		T* newObject = std::make_shared<T>();
		newObject->SetOwner(this);
		m_GameObjects.push_back(newObject);
		return newObject;
	}
};

