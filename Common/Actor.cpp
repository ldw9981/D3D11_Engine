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
	if (m_ActorStatus == ActorStatus::CREATE)
	{
		OnBeginPlay();
		m_ActorStatus = ActorStatus::PLAY;
	}
	
	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->Update(DeltaTime);	
	}
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
	m_ActorStatus = ActorStatus::PLAY;	
	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->OnBeginPlay();
	}
	m_OnBeginPlay.Invoke();
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
	m_OnEndPlay.Invoke();
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

std::weak_ptr<Component> Actor::GetComponentWeakPtrByName(const std::string& Name)
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

Component* Actor::GetComponentPtrByName(const std::string& Name)
{
	for (auto& pComponent : m_OwnedComponents)
	{
		if (pComponent->GetName() == Name)
		{
			return pComponent.get();
		}
	}
	return nullptr;
}

void Actor::OnRenderImGUI()
{	
	ImGui::Begin("Properties");

	if (ImGui::TreeNodeEx(m_Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		PropertyContainer::OnRenderImGUI();
		ImGui::TreePop();
	}

	for (auto& pComponent : m_OwnedComponents)
	{
		pComponent->OnRenderImGUI();
	}
	ImGui::End();
}
