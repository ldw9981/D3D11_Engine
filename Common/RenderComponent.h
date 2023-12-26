#pragma once
#include "SceneComponent.h"


/// <summary>
/// 그릴수 있는 컴포넌트의 공통클래스
/// AABB를 갖는게 주된목적
/// </summary>
class RenderComponent :
    public SceneComponent
{
public:
	RenderComponent();
	~RenderComponent();

	DirectX::BoundingBox m_BoundingBox;
	bool m_bIsCulled = true;
	virtual void Update(float DeltaTime) override;
};

