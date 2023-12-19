#pragma once
#include "SceneComponent.h"

class Actor;
class StaticMeshComponent :
    public SceneComponent
{
public:
	StaticMeshComponent(Actor* pOwner, const std::string& Name);
	~StaticMeshComponent();

	virtual void Update(float DeltaTime) override;

	bool LoadFBX(const std::string& FileName);
};

