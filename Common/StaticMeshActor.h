#pragma once
#include "Actor.h"

class StaticMeshComponent;
class StaticMeshActor :
    public Actor
{
public:
	StaticMeshActor();
	~StaticMeshActor();

	StaticMeshComponent* m_pStaticMeshComponent=nullptr;

	std::string m_SceneFilePath;
	virtual void OnBeginPlay();
};

