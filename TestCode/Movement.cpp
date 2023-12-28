#include "pch.h"
#include "Movement.h"

void Movement::UpdateMovement(float DeltaTime)
{
	UpdateTransform(DeltaTime);

	DirectX::SimpleMath::Vector3 position;
	GetPosition(position);
	position.x += 1.0f;
	SetPosition(position);
}
