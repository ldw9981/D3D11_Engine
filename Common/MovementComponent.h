#pragma once
#include "Component.h"
/*
	RootSceneComponent의 Position을 InpuVector 방향에 따라 업데이트 해주는 컴포넌트
	InpuVector는 한번 사용하고 0으로 초기화한다.
	업데이트 전에 Before에 보관하고 업데이트 후에 After에 보관한다.
*/
class SceneComponent;
class MovementComponent: public Component
{
public:
	MovementComponent();
	~MovementComponent();

	Math::Vector3 m_InputVector;
	float m_Speed=1.0f;
	SceneComponent* m_pRootComponent=nullptr;
	Math::Vector3 m_PositionBefore;
	Math::Vector3 m_PositionAfter;
	
	void AddInputVector(const Math::Vector3& input);

	virtual void Update(float DeltaTime);
	virtual void OnBeginPlay();
	virtual void OnEndPlay();

	virtual void ImGuiRender() override;
	
};

