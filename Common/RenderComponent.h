#pragma once
#include "SceneComponent.h"


/// <summary>
/// �׸��� �ִ� ������Ʈ�� ����Ŭ����
/// AABB�� ���°� �ֵȸ���
/// </summary>
class RenderComponent :
    public SceneComponent
{
public:
	RenderComponent();
	~RenderComponent();

	DirectX::BoundingBox m_BoundingBox;
	
	virtual void Update(float DeltaTime) override;
};
