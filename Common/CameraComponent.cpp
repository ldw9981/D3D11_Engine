#include "pch.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent()
{

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Update(float DeltaTime)
{
	__super::Update(DeltaTime);
}

void CameraComponent::GetViewMatrix(Math::Matrix& ViewMatrix)
{
	Math::Vector3 eye = m_World.Translation();
	Math::Vector3 target = m_World.Translation() + GetForward();
	Math::Vector3 up = m_World.Up();
	ViewMatrix = XMMatrixLookAtLH(eye, target, up);
}

DirectX::SimpleMath::Vector3 CameraComponent::GetForward()
{
	return -m_World.Forward();
}

