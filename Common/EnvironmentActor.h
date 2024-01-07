#pragma once
#include "Actor.h"

class EnvironmentMeshComponent;
class EnvironmentActor : public Actor
{
public:
	EnvironmentActor();
	virtual ~EnvironmentActor();

	EnvironmentMeshComponent* m_pEnvironmentMeshComponent = nullptr;

	virtual void OnBeginPlay() override;
	virtual void OnEndPlay() override;


};

