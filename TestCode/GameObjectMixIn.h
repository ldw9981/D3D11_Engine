#pragma once
#include "Transform.h"
#include "Movement.h"

//�������̽� ���߻�� �׽�Ʈ
class GameObjectMixIn :  public Movement, public Transform
{
public:
	GameObjectMixIn() {};
	virtual ~GameObjectMixIn() {};

	void Update(float DeltaTime);
};	

