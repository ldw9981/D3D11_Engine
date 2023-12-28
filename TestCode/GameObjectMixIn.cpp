#include "pch.h"
#include "GameObjectMixIn.h"

void GameObjectMixIn::Update(float DeltaTime)
{
	UpdateTransform(DeltaTime);
	UpdateMovement(DeltaTime);
}
