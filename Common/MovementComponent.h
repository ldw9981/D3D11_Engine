#pragma once
#include "Component.h"
#include "D3DRenderManager.h"

class SceneComponent;
class MovementComponent: public Component , public IImGuiRenderable
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

