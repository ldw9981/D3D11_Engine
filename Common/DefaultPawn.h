#pragma once
#include "Pawn.h"

class MovementComponent;
class DefaultPawn :
    public Pawn
{
public:
    DefaultPawn();
    ~DefaultPawn();

    std::shared_ptr<SceneComponent> m_SceneComponent;
    std::shared_ptr<MovementComponent> m_MovementComponent;

	virtual void OnBeginPlay();// World에서 추가후 게임 진행할때 호출
	virtual void OnEndPlay();  // World 제거되었을 때 호출
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState);
};

