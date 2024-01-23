#include "pch.h"
#include "GameObjectMixIn.h"

void TestGameObjectMixIn()
{
	GameObjectMixIn a;
	a.Update(1.0f);
}


void GameObjectMixIn::Update(float DeltaTime)
{
	UpdateTransform(DeltaTime);
	UpdateMovement(DeltaTime);
}

