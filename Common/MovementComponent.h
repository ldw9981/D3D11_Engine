#pragma once
#include "Component.h"


class SceneComponent;
class MovementComponent: public Component
{
public:
	MovementComponent();
	~MovementComponent();

	Math::Vector3 m_InputVector;
	float m_Speed=1.0f;
	SceneComponent* m_pRootComponent;
		
	void AddInputVector(const Math::Vector3& input);

	virtual void Update(float DeltaTime);
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
	
};

