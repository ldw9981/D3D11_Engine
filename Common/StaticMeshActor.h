#pragma once
#include "Actor.h"

class BoxComponent;
class StaticMeshComponent;
class StaticMeshActor :
    public Actor
{
public:
	StaticMeshActor();
	~StaticMeshActor();


	BoxComponent* m_pBoxComponent = nullptr;
	StaticMeshComponent* m_pStaticMeshComponent=nullptr;

	std::string m_SceneFilePath;
	virtual void OnBeginPlay();
};

