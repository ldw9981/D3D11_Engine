#pragma once
#include "Pawn.h"

class MovementComponent;
class CameraComponent;
class ActorController;
class DefaultPawn :
    public Pawn
{
public:
    DefaultPawn();    
     ~DefaultPawn();
   
    SceneComponent* m_pSceneComponent = nullptr;
    MovementComponent* m_pMovementComponent = nullptr;
    CameraComponent* m_pCameraComponent = nullptr;

    virtual void Update(float DeltaTime) override;
	virtual void OnBeginPlay();// World���� �߰��� ���� �����Ҷ� ȣ��
	virtual void OnEndPlay();  // World ���ŵǾ��� �� ȣ��
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyCurr, const DirectX::Keyboard::State& KeyLast,
    const DirectX::Mouse::State& MouseCurr, const DirectX::Mouse::State& MouseLast);

    virtual void OnPossess(ActorController* pActorController) override;
};

