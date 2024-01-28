#include "pch.h"
#include "Component.h"

Component::Component()
{

}

void Component::OnRenderImGUI()
{
	if (ImGui::TreeNode(m_Name.c_str()))
	{
		PropertyContainer::OnRenderImGUI();
		ImGui::TreePop();		
	}	
}
