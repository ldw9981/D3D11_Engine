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

	// static,skeletal mesh �ٸ��� �� �ʿ䰡����.
	m_BoundingBox.Center = GetWorldPosition();
}
