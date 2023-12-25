#include "pch.h"
#include "Actor.h"

void Actor::Update(float DeltaTime)
{
	for(auto& pComponent : m_OwnedComponents)
	{
		pComponent->Update(DeltaTime);
	}
}

void Actor::Render(ID3D11DeviceContext* pDeviceContext)
{
}

void Actor::SetRootComponent(std::shared_ptr<SceneComponent> pRootComponent)
{
	m_pRootComponent = pRootComponent;
}

std::shared_ptr<SceneComponent> Actor::GetRootComponent() const
{
	return m_pRootComponent;
}

void Actor::OnBeginPlay()
{
	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->OnBeginPlay();
	}
}

void Actor::OnEndPlay()
{
	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->OnEndPlay();
	}
}
