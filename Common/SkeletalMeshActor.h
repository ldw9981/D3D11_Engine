#pragma once
#include "Actor.h"

class SkeletalMeshComponent;
class SkeletalMeshActor: public Actor
{
public:
	SkeletalMeshActor();
	~SkeletalMeshActor() {};

	SkeletalMeshComponent* m_pSkeletalMeshComponent = nullptr;

	std::string m_SceneFilePath;	// BeginPlay에서 로딩
	virtual void OnBeginPlay();
};

