#pragma once
#include "D3DRenderManager.h"

class Actor;
class Component : public IImGuiRenderable
{
public:
	friend class Actor;
	Component() {};
	virtual ~Component() {};

	
	
protected:
	Actor* m_pOwner = nullptr;
	std::string m_Name;			// 게임 오브젝트 안에서 같은 종류의 컴포넌트라도 다른 이름을 갖게하여 구분할수 있게 한다.	
public:
	void SetName(std::string val) { m_Name = val; }
	const std::string& GetName() const { return m_Name; }
	
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }
	Actor* GetOwner() { return m_pOwner; }

	virtual void Update(float DeltaTime) = 0;
	virtual void OnBeginPlay() = 0;
	virtual void OnEndPlay() = 0;
	virtual void ImGuiRender() {};
};

