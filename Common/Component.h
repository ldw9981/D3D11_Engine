#pragma once

class Actor;
class Component
{
public:
	Component() {};
	Component(Actor* pOwner, const std::string& Name) : m_pOwner(pOwner), m_Name(Name)
	{
	}
	virtual ~Component() {};

protected:
	Actor* m_pOwner = nullptr;
	std::string m_Name;			// 게임 오브젝트 안에서 같은 종류의 컴포넌트라도 다른 이름을 갖게하여 구분할수 있게 한다.	
public:
	const std::string& GetName() const { return m_Name; }
	virtual void Update(float DeltaTime) = 0;
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }
	Actor* GetOwner() { return m_pOwner; }


};

