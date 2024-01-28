#include "pch.h"
#include "Component.h"

Component::Component()
{

}

void Component::OnRenderImGUI()
{
	if (ImGui::TreeNodeEx(m_Name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		PropertyContainer::OnRenderImGUI();
		ImGui::TreePop();		
	}	
}
