#pragma once
#include "Component.h"
/*
	RootSceneComponent�� Position�� InpuVector ���⿡ ���� ������Ʈ ���ִ� ������Ʈ
	InpuVector�� �ѹ� ����ϰ� 0���� �ʱ�ȭ�Ѵ�.
	������Ʈ ���� Before�� �����ϰ� ������Ʈ �Ŀ� After�� �����Ѵ�.
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
	void SetSpeed(float speed) { m_Speed = speed; }
	virtual void Update(float DeltaTime);
	virtual void OnBeginPlay();
	virtual void OnEndPlay();

	virtual void OnRenderImGUI() override;
	
};

