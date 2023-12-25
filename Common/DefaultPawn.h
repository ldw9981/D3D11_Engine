#pragma once
#include "Pawn.h"

class MovementComponent;
class CameraComponent;
class DefaultPawn :
    public Pawn
{
public:
    DefaultPawn();
    ~DefaultPawn();

    std::shared_ptr<SceneComponent> m_pSceneComponent;
    std::shared_ptr<MovementComponent> m_pMovementComponent;
    std::shared_ptr<CameraComponent> m_pCameraComponent;

    virtual void Update(float DeltaTime) override;
	virtual void OnBeginPlay();// World���� �߰��� ���� �����Ҷ� ȣ��
	virtual void OnEndPlay();  // World ���ŵǾ��� �� ȣ��
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState);

    virtual void OnPossess(PlayerController* pPlayerController) override;
};

