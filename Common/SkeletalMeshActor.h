#pragma once
#include "Actor.h"

class SkeletalMeshComponent;
class SkeletalMeshActor: public Actor
{
public:
	SkeletalMeshActor();
	~SkeletalMeshActor() {};

	SkeletalMeshComponent* m_pSkeletalMeshComponent = nullptr;

	std::string m_SceneFilePath;	// BeginPlay���� �ε�
	virtual void OnBeginPlay();
};

