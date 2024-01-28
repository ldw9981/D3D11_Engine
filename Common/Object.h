#pragma once
#include "PropertyContainer.h"
class Object : public PropertyContainer
{
public:
	Object();
	~Object() {};

	static int m_CreatedCount;

	std::string m_ClassName;	
	std::string m_Name;
public:
	void SetName(std::string val) { m_Name = val; }
	const std::string& GetName() const { return m_Name; }

	virtual void SerializeOut(nlohmann::ordered_json& object);
	virtual void SerializeIn(nlohmann::ordered_json& object);
	virtual void OnRenderImGUI();
};

