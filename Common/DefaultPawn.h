#pragma once
#include "Pawn.h"
#include "CollisionComponent.h"

/*
    �߷¾��� �̵��ϴ� ����ī�޶�
    �浹ó���� Block ������  Sphere�� ����Ѵ�.
*/
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
private:
	bool m_bUsePicking = false;
public:
    MovementComponent* m_pMovementComponent = nullptr;
    CameraComponent* m_pCameraComponent = nullptr;
    SphereComponent* m_pSphereComponent = nullptr;

    Actor* m_pSelectedActor = nullptr;
    bool m_bDebugPickingRay = false;
public:
    bool GetUsePicking() const { return m_bUsePicking; }
    void SetUsePicking(bool val) { m_bUsePicking = val; }
	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker);

    virtual void OnPossess(ActorController* pActorController) override;
	virtual void OnBlock(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) override;
	virtual void OnBeginOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) override;
	virtual void OnEndOverlap(CollisionComponent* pOwnedComponent, Actor* pOtherActor, CollisionComponent* pOtherComponent) override;

};

