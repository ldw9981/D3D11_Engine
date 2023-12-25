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
   
    SceneComponent* m_pSceneComponent;
    MovementComponent* m_pMovementComponent;
    CameraComponent* m_pCameraComponent;

    virtual void Update(float DeltaTime) override;
	virtual void OnBeginPlay();// World���� �߰��� ���� �����Ҷ� ȣ��
	virtual void OnEndPlay();  // World ���ŵǾ��� �� ȣ��
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState);

    virtual void OnPossess(PlayerController* pPlayerController) override;
};

