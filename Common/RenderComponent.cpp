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
	CalculateBoundingBox();
}

void RenderComponent::CalculateBoundingBox()
{
	// static,skeletal mesh 다르게 할 필요가있음.
	m_BoundingBox.Center = GetWorldPosition();
}
