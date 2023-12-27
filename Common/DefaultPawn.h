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
	virtual void OnBeginPlay();// World에서 추가후 게임 진행할때 호출
	virtual void OnEndPlay();  // World 제거되었을 때 호출
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyCurr, const DirectX::Keyboard::State& KeyLast,
    const DirectX::Mouse::State& MouseCurr, const DirectX::Mouse::State& MouseLast);

    virtual void OnPossess(ActorController* pActorController) override;
};

