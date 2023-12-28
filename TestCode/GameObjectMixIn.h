#pragma once
#include "Transform.h"
#include "Movement.h"
class GameObjectMixIn :  public Movement, public Transform
{
public:
	GameObjectMixIn() {};
	virtual ~GameObjectMixIn() {};

	void Update(float DeltaTime);
};	

