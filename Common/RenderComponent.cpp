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
	// 축이 아래에 항상 있다고 가정
	m_BoundingBox.Center.y += m_BoundingBox.Extents.y;
}
