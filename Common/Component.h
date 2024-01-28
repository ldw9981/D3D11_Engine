#pragma once
#include "D3DRenderManager.h"
#include "Object.h"

class Actor;
class Component : public Object
{
public:
	friend class Actor;
	Component();
	virtual ~Component() {};

	
	
protected:
	Actor* m_pOwner = nullptr;
	
public:

	
	void SetOwner(Actor* pOwner) { m_pOwner = pOwner; }
	Actor* GetOwner() { return m_pOwner; }

	virtual void Update(float DeltaTime) = 0;
	virtual void OnBeginPlay() = 0;
	virtual void OnEndPlay() = 0;
	virtual void OnRenderImGUI();
};

