#pragma once
#include "InputManager.h"
#include "SceneComponent.h"
#include "MovementComponent.h"
#include "CameraComponent.h"
#include "SkeletalMeshComponent.h"
#include "StaticMeshComponent.h"


class Component;

class World;
class Actor: public InputProcesser
{
	friend class World;
public:
	Actor() {};
	~Actor() {};
public:
	std::list<std::shared_ptr<Component>> m_OwnedComponents;	// ������ ������Ʈ��
private:
	World* m_pOwner = nullptr;					// �� ���� ������Ʈ�� ���� ����
public:
	std::shared_ptr<SceneComponent> m_pRootComponent;		// ������Ʈ �߿� ����� ��Ʈ���� ����
	World* GetOwner() const { return m_pOwner; }
	void SetOwner(World* val) { m_pOwner = val; }
	virtual void Update(float DeltaTime);
	virtual void Render(ID3D11DeviceContext* pDeviceContext);

	// ��Ʈ ������Ʈ�� ����
	void SetRootComponent(std::shared_ptr<SceneComponent> pRootComponent);
	std::shared_ptr<SceneComponent> GetRootComponent() const;

	// ������Ʈ�� ��Ʋ�� �Լ��� �����մϴ�.
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

	virtual void OnBeginPlay();	// World���� �߰��� ���� �����Ҷ� ȣ��
	virtual void OnEndPlay();  // World ���ŵǾ��� �� ȣ��
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState) {};


	void SetWorldPosition(const Math::Vector3& val);
};

