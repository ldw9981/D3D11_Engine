#pragma once
#include "Component.h"

#include "Transform.h"



class SceneComponent: public Component, public Transform
{
public:
	SceneComponent() {};
	~SceneComponent();

		
	void SetRelativePosition(Math::Vector3 val);
	
	void SetRelativeRotation(Math::Vector3 val);
	void SetRelativeScale(Math::Vector3 val);
	bool m_bDirty = false;


	virtual void Update(float DeltaTime);
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
private:
	Math::Vector3 m_RelativePosition;
	Math::Vector3 m_RelativeScale;
	Math::Vector3 m_RelativeRotation;
};

