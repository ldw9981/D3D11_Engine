#pragma once
#include "InputManager.h"

#include "MovementComponent.h"
#include "CameraComponent.h"
#include "SkeletalMeshComponent.h"
#include "StaticMeshComponent.h"
#include "SceneComponent.h"
#include "CollisionComponent.h"
#include "D3DRenderManager.h"
#include "Object.h"

class Component;
class World;

enum class ActorStatus
{
	CREATE=0,
	PLAY,
	DESTROY
};
class Actor: public Object, public InputProcesser
{
	friend class World;
public:
	Actor();
	~Actor();
public:
	std::list<std::shared_ptr<Component>> m_OwnedComponents;	// 소유한 컴포넌트들
	SceneComponent* m_pRootComponent = nullptr;		// 컴포넌트 중에 어느게 루트인지 설정
	std::function<void()> m_OnRenderImGUI = std::bind(&Actor::OnRenderImGUI, this);
	Deligate	m_OnBeginPlay;
	Deligate	m_OnEndPlay;
protected:
	World* m_pOwner = nullptr;					// 이 게임 오브젝트가 속한 월드
	bool b_UseInputProcesser = false;
	std::list<std::shared_ptr<Actor>>::iterator m_iteratorInWorld;
	ActorStatus m_ActorStatus = ActorStatus::CREATE;

public:
	World* GetOwner() const { return m_pOwner; }
	void SetOwner(World* val) { m_pOwner = val; }
	virtual void Update(float DeltaTime);

	// 루트 컴포넌트로 설정
	void SetRootComponent(SceneComponent* pRootComponent);
	SceneComponent* GetRootComponent() const;

	// 컴포넌트를 템틀릿 함수로 생성합니다.
	template<typename T>
	std::shared_ptr<T> CreateComponent(const std::string& Name)
	{
		bool bIsBase = std::is_base_of<Component, T>::value;
		assert(bIsBase == true);
		std::shared_ptr<T> pComponent = std::make_shared<T>();
		pComponent->m_pOwner = this;
		pComponent->SetName( Name);
		m_OwnedComponents.push_back(pComponent);		
		return pComponent;
	}

	virtual void OnBeginPlay();	// World에서 추가후 게임 진행할때 호출
	virtual void OnEndPlay();  // World 제거되었을 때 호출
	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker,
		const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker);

	void SetWorldPosition(const Math::Vector3& val);
	void SetWorldTransform(Math::Matrix val);
	std::weak_ptr<Component> GetComponentWeakPtrByName(const std::string& Name);
	Component* GetComponentPtrByName(const std::string& Name);
	ActorStatus GetActorStatus() const { return m_ActorStatus; }
	void SetActorStatus(ActorStatus val) { m_ActorStatus = val; }

	void OnRenderImGUI();
};

