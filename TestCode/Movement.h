#pragma once
#include "Transform.h"

class Movement : virtual public ITransform
{
public:
	Movement() {};
	~Movement() {};
public:		
    void UpdateMovement(float DeltaTime);
};

