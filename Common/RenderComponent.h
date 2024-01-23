#pragma once
#include "SceneComponent.h"


/// <summary>
/// 그릴수 있는 컴포넌트의 공통클래스
/// AABB를 갖는게 주된목적
/// </summary>
class RenderComponent :
    public SceneComponent
{
friend class D3DRenderManager;

public:
	RenderComponent();
	~RenderComponent();

	DirectX::BoundingBox m_BoundingBox;
	Math::Vector3 m_BoundingBoxCenterOffset = Math::Vector3::Zero;

	bool m_bIsCulled = true;
private:
	bool m_bVisibility = true;

public:

	virtual void Update(float DeltaTime) override;
	Math::Vector3 GetBoundingBoxCenterOffset() const { return m_BoundingBoxCenterOffset; }
	void SetBoundingBoxCenterOffset(Math::Vector3 val) { m_BoundingBoxCenterOffset = val; }
	bool GetVisibility() const { return m_bVisibility; }
	void SetVisibility(bool val) { m_bVisibility = val; }

};

