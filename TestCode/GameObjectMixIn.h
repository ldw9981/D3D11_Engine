#pragma once
#include "Transform.h"
#include "Movement.h"

//인터페이스 다중상속 테스트
class GameObjectMixIn :  public Movement, public Transform
{
public:
	GameObjectMixIn() {};
	virtual ~GameObjectMixIn() {};

	void Update(float DeltaTime);
};	

