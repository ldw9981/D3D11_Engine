#include "pch.h"
#include "SceneComponent.h"



SceneComponent::~SceneComponent()
{
}

void SceneComponent::SetLocalPosition(const Math::Vector3& val)
{
	m_bDirty = true;
	m_LocalPosition = val;
}

void SceneComponent::SetLocalRotation(const Math::Vector3& val)
{
	m_bDirty = true;
	m_LocalRotation = val;
}

void SceneComponent::SetLocalScale(const Math::Vector3& val)
{
	m_bDirty = true;
	m_LocalScale = val;
}

void SceneComponent::SetLocalTransform(Math::Matrix val)
{
	m_bDirty = true;	
	Math::Quaternion q;
	val.Decompose(m_LocalScale,q,m_LocalPosition);	
	m_LocalRotation = q.ToEuler();
}

void SceneComponent::Update(float DeltaTime)
{
	if (m_bDirty)
	{
		m_Local = Math::Matrix::CreateScale(m_LocalScale) * 
			Math::Matrix::CreateFromYawPitchRoll(XMConvertToRadians(m_LocalRotation.y), XMConvertToRadians(m_LocalRotation.x), XMConvertToRadians(m_LocalRotation.z)) *
			Math::Matrix::CreateTranslation(m_LocalPosition);
		m_bDirty = false;
	}	
	Transform::Update(DeltaTime);
}

void SceneComponent::OnBeginPlay()
{

}

void SceneComponent::OnEndPlay()
{

}

void SceneComponent::ImGuiRender()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		ImGui::DragFloat3("LocalPosition", &m_LocalPosition.x, 0.1f);
		ImGui::DragFloat3("LocalRotation", &m_LocalRotation.x, 0.1f);
		ImGui::DragFloat3("LocalScale", &m_LocalScale.x, 0.1f);
		ImGui::TreePop();
	}
	m_bDirty =true;
}

