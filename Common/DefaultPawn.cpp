#include "pch.h"
#include "DefaultPawn.h"
#include "D3DRenderManager.h"
#include "ActorController.h"
#include "SphereComponent.h"
#include "MovementComponent.h"

constexpr float ROTATION_GAIN = 0.004f;
constexpr float MOVEMENT_GAIN = 0.07f;

DefaultPawn::DefaultPawn()
{
	m_pMovementComponent = CreateComponent<MovementComponent>("MovementComponent").get();
	m_pMovementComponent->m_Speed = 200.0f;

	m_pCameraComponent = CreateComponent<CameraComponent>("CameraComponent").get();
	SetRootComponent(m_pCameraComponent);

	m_pSphereComponent = CreateComponent<SphereComponent>("SphereComponent").get();
	m_pSphereComponent->SetParent(m_pCameraComponent);
	m_pSphereComponent->SetCollisionType(CollisionType::Block);
	m_pSphereComponent->m_Geomety.Radius = 3.0f;
	m_pSphereComponent->SetNotifyListener(this);

	D3DRenderManager::Instance->AddImguiRenderable(m_pMovementComponent);
}

DefaultPawn::~DefaultPawn()
{
	D3DRenderManager::Instance->RemoveImguiRenderable(m_pMovementComponent);
}



void DefaultPawn::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	ActorController* pController = GetController();
	float fowardScale = 0.0f, rightScale = 0.0f, upScale = 0.0f;

	Math::Matrix rotMatrix = Matrix::CreateFromYawPitchRoll(pController->GetYaw(), pController->GetPitch(), 0.0f);
	Math::Vector3 forward = Vector3(rotMatrix._31, rotMatrix._32, rotMatrix._33);
	Math::Vector3 right = rotMatrix.Right();

	if (KeyTracker.IsKeyPressed(Keyboard::Keys::R))
	{
		SetWorldTransform(Matrix::Identity);		
		pController->SetPitch(0.0f);
		pController->SetYaw(0.0f);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		m_pMovementComponent->AddInputVector(forward);
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		m_pMovementComponent->AddInputVector(-forward);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::A))
	{
		m_pMovementComponent->AddInputVector(-right);
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::D))
	{
		m_pMovementComponent->AddInputVector(right);
	}

	if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::E))
	{
		m_pMovementComponent->AddInputVector(-Math::Vector3::Up);
	}
	else if (KeyState.IsKeyDown(DirectX::Keyboard::Keys::Q))
	{
		m_pMovementComponent->AddInputVector(Math::Vector3::Up);
	}


	InputManager::Instance->m_Mouse->SetMode(MouseState.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
	if (MouseState.positionMode == Mouse::MODE_RELATIVE)
	{
		Vector3 delta = Vector3(float(MouseState.x), float(MouseState.y), 0.f) * ROTATION_GAIN;
		pController->AddPitch(delta.y);
		pController->AddYaw(delta.x);

		m_pCameraComponent->SetLocalRotation(Math::Vector3(pController->GetPitch(), pController->GetYaw(), 0.0f));
	}
}

void DefaultPawn::OnPossess(ActorController* pPlayerController)
{
	__super::OnPossess(pPlayerController);
	
	auto pComponent = (std::weak_ptr<Component>)GetComponentWeakPtrByName("CameraComponent");
	if (pComponent.expired() ==false)
	{
		std::weak_ptr<CameraComponent> pCameraComponent = std::dynamic_pointer_cast<CameraComponent>(pComponent.lock());
		D3DRenderManager::Instance->SetCamera(pCameraComponent);
	}	
}

void DefaultPawn::OnBlock(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent)
{	
	SetWorldPosition(m_pMovementComponent->m_PositionBefore);	
}

void DefaultPawn::OnBeginOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent)
{

}

void DefaultPawn::OnEndOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent)
{

}
