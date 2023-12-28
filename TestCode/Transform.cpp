#include "pch.h"
#include "Transform.h"

void Transform::UpdateTransform(float DeltaTime)
{
	printf("Transform::UpdateTransform\n");
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& position)
{
	m_position = position;
}

void Transform::GetPosition(DirectX::SimpleMath::Vector3& position)
{
	position = m_position;
}
