#pragma once
#include "SceneComponent.h"
class CameraComponent :
    public SceneComponent
{
public:
    CameraComponent();
	~CameraComponent();

	virtual void Update(float DeltaTime) override;

	void GetViewMatrix(Math::Matrix& ViewMatrix);
};

