#include "pch.h"
#include "DefaultPawn.h"
#include "D3DRenderManager.h"

DefaultPawn::DefaultPawn()
{
	m_pSceneComponent = CreateComponent<SceneComponent>("SceneComponent");
	SetRootComponent(m_pSceneComponent);

	m_pMovementComponent = CreateComponent<MovementComponent>("MovementComponent");
	m_pCameraComponent = CreateComponent<CameraComponent>("CameraComponent");
	m_pCameraComponent->SetParent(m_pSceneComponent.get());
}

DefaultPawn::~DefaultPawn()
{
}

void DefaultPawn::Update(float DeltaTime)
{
	__super::Update(DeltaTime);

	/*
	m_rotMatrix = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.0f);
	m_forward = Vector3(m_rotMatrix._31, m_rotMatrix._32, m_rotMatrix._33);
	m_right = m_rotMatrix.Right();

	float speed = MOVEMENT_GAIN;       // 이동속도
	m_MoveDirection = m_forward * fowardScale + m_right * rightScale + Vector3::Up * upScale; // 회전상태와 키를 고려한 월드에서의 이동방향
	m_MoveDirection.Normalize(); //  순수 크기1로  정규화

	m_cameraPos = m_cameraPos + m_MoveDirection * speed;   // 정면벡터

	// limit pitch to straight up or straight down
	constexpr float limit = XM_PIDIV2 - 0.01f;
	m_pitch = max(-limit, m_pitch);
	m_pitch = min(+limit, m_pitch);

	// keep longitude in sane range by wrapping
	if (m_yaw > XM_PI)
	{
		m_yaw -= XM_2PI;
	}
	else if (m_yaw < -XM_PI)
	{
		m_yaw += XM_2PI;
	}

	XMVECTOR lookAt = m_cameraPos + m_forward;   // 정면벡터

	D3DRenderManager::Instance->SetEyePosition(m_cameraPos);
	D3DRenderManager::Instance->m_View = XMMatrixLookAtLH(m_cameraPos, lookAt, Vector3::Up)
	*/

	//D3DRenderManager::Instance->SetEyePosition(m_pRootComponent->GetWorldPosition());
	//D3DRenderManager::Instance->m_View = XMMatrixLookAtLH(m_pRootComponent->GetWorldPosition(), Math::Vector3(0.0f,0.0f,1.0f), Vector3::Up);
}

void DefaultPawn::OnBeginPlay()
{
	__super::OnBeginPlay();
}

void DefaultPawn::OnEndPlay()
{
	__super::OnEndPlay();
}

void DefaultPawn::OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState)
{
	PlayerController* pController = GetController();

	if (KeyboardState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		m_pMovementComponent->AddInputVector(Math::Vector3(0.0f,0.0f,1.0f));
	}
	if (KeyboardState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		m_pMovementComponent->AddInputVector(Math::Vector3(0.0f, 0.0f,- 1.0f));
	}
}

void DefaultPawn::OnPossess(PlayerController* pPlayerController)
{
	__super::OnPossess(pPlayerController);
	D3DRenderManager::Instance->SetCamera(m_pCameraComponent);
}
