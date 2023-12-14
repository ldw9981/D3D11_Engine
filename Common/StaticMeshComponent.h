#pragma once
#include "SceneComponent.h"
class StaticMeshComponent :
    public SceneComponent
{
public:
	StaticMeshComponent();
	~StaticMeshComponent();

	virtual void Update(float DeltaTime) override;

	bool LoadFBX(const std::string& FileName);
};

