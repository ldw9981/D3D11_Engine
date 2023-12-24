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

	virtual void OnBeginPlay();// World���� �߰��� ���� �����Ҷ� ȣ��
	virtual void OnEndPlay();  // World ���ŵǾ��� �� ȣ��
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState);
};

