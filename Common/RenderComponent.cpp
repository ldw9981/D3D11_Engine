#include "pch.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent()
{

}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::Update(float DeltaTime)
{
	__super::Update(DeltaTime);
	m_BoundingBox.Center = GetWorldPosition();
	// ���� �Ʒ��� �׻� �ִٰ� ����
	m_BoundingBox.Center.y += m_BoundingBox.Extents.y;
}
