#pragma once
#include "Component.h"

#include "Transform.h"



class SceneComponent: public Component, public Transform
{
public:
	SceneComponent() {};
	~SceneComponent();
protected:
	Math::Vector3 m_LocalPosition = Math::Vector3(0.0f, 0.0f, 0.0f);
	Math::Vector3 m_LocalScale = Math::Vector3(1.0f, 1.0f, 1.0f);
	Math::Vector3 m_LocalRotation = Math::Vector3(0.0f, 0.0f, 0.0f);	// radian
	bool m_bDirty = false;
public:
	void SetLocalPosition(const Math::Vector3& val);	
	void SetLocalRotation(const Math::Vector3& val);
	void SetLocalScale(const Math::Vector3& val);
	void SetLocalTransform(Math::Matrix val);

	const Math::Vector3& GetLocalPosition() const { return m_LocalPosition; }
	const Math::Vector3& GetLocalRotation() const { return m_LocalRotation; }
	const Math::Vector3& GetLocalScale() const { return m_LocalScale; }

	const Math::Vector3 GetForward() const { return Math::Vector3(m_World._31, m_World._32, m_World._33); }

	virtual void Update(float DeltaTime);
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
	virtual void ImGuiRender();

};

