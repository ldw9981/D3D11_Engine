#include "pch.h"
#include "Movement.h"

void Movement::UpdateMovement(float DeltaTime)
{
	DirectX::SimpleMath::Vector3 position;
	GetPosition(position);
	position = position + m_Direction * m_Speed * DeltaTime;
	SetPosition(position);
}
