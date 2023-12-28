#include "pch.h"
#include "Actor.h"
#include "InputManager.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

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

void Actor::SetRootComponent(SceneComponent* pRootComponent)
{
	m_pRootComponent = pRootComponent;
}

SceneComponent* Actor::GetRootComponent() const
{
	return m_pRootComponent;
}

void Actor::OnBeginPlay()
{
	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->OnBeginPlay();
	}
	if (b_UseInputProcesser)
	{
		InputManager::Instance->AddInputProcesser(this);
	}
}

void Actor::OnEndPlay()
{
	if (b_UseInputProcesser)
	{
		InputManager::Instance->RemoveInputProcesser(this);
	}

	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->OnEndPlay();
	}
}

void Actor::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{

}

void Actor::SetWorldPosition(const Math::Vector3& val)
{
	if (m_pRootComponent)
	{
		m_pRootComponent->SetLocalPosition(val);
	}	
}

void Actor::SetWorldTransform(Math::Matrix val)
{
	if (m_pRootComponent)
	{
		m_pRootComponent->SetLocalTransform(val);
	}
}

std::weak_ptr<Component> Actor::GetComponentByName(const std::string& Name)
{
	for (auto& pComponent : m_OwnedComponents)
	{
		if (pComponent->GetName() == Name)
		{
			return pComponent;
		}
	}
	return std::weak_ptr<Component>();
}
