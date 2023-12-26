#pragma once
#include "Component.h"

#include "Transform.h"



class SceneComponent: public Component, public Transform
{
public:
	SceneComponent() {};
	~SceneComponent();

		
	void SetLocalPosition(Math::Vector3 val);
	
	void SetLocalRotation(Math::Vector3 val);
	void SetLocalScale(Math::Vector3 val);
	bool m_bDirty = false;


	virtual void Update(float DeltaTime);
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
private:
	Math::Vector3 m_LocalPosition = Math::Vector3(0.0f,0.0f,0.0f);
	Math::Vector3 m_LocalScale = Math::Vector3(1.0f, 1.0f, 1.0f);
	Math::Vector3 m_LocalRotation = Math::Vector3(0.0f, 0.0f, 0.0f);	// radian
};

