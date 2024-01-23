#pragma once
#include "SceneComponent.h"


/// <summary>
/// �׸��� �ִ� ������Ʈ�� ����Ŭ����
/// AABB�� ���°� �ֵȸ���
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

