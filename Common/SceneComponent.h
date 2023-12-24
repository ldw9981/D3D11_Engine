#pragma once
#include "Component.h"

#include "Transform.h"



class SceneComponent: public Component, public Transform
{
public:
	SceneComponent() {};
	SceneComponent(Actor* pOwner, const std::string& Name);
	~SceneComponent();


	Math::Vector3 m_Position;
	Math::Vector3 m_Rotation;
	Math::Vector3 m_Scale;


	virtual void Update(float DeltaTime);
};

