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
	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker);

    virtual void OnPossess(ActorController* pActorController) override;
};

