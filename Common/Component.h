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
	std::string m_Name;			// ���� ������Ʈ �ȿ��� ���� ������ ������Ʈ�� �ٸ� �̸��� �����Ͽ� �����Ҽ� �ְ� �Ѵ�.	
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

