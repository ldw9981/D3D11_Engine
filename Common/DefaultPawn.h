#pragma once
#include "Pawn.h"
#include "CollisionComponent.h"

class MovementComponent;
class CameraComponent;
class ActorController;
class SphereComponent;
class DefaultPawn :
    public Pawn, public ICollisionNotify
{
public:
    DefaultPawn();    
     ~DefaultPawn();
   
    SceneComponent* m_pSceneComponent = nullptr;
    MovementComponent* m_pMovementComponent = nullptr;
    CameraComponent* m_pCameraComponent = nullptr;
    SphereComponent* m_pSphereComponent = nullptr;

    virtual void Update(float DeltaTime) override;
	virtual void OnBeginPlay();// World에서 추가후 게임 진행할때 호출
	virtual void OnEndPlay();  // World 제거되었을 때 호출
	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker);

    virtual void OnPossess(ActorController* pActorController) override;
	virtual void OnBlock(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) override;
	virtual void OnBeginOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) override;
	virtual void OnEndOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) override;
};

