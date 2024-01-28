#include "pch.h"
#include "Object.h"

Object::Object()
{
	//m_ObjectName = typeid(this).name();
	//m_InstanceName = m_ObjectName + std::to_string(m_CreatedCount++);
}

int Object::m_CreatedCount =0;

void Object::SerializeOut(nlohmann::ordered_json& object)
{
	object["ClassName"] = typeid(this).name();
	PropertyContainer::SerializeOut(object);
}

void Object::SerializeIn(nlohmann::ordered_json& object)
{
	PropertyContainer::SerializeIn(object);
}

void Object::OnRenderImGUI()
{
	PropertyContainer::OnRenderImGUI();
}
