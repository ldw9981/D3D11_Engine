#include "pch.h"
#include "PropertyContainer.h"

TestClass::TestClass()
{
	ADD_PROPERTY(m_TestInt);
	ADD_PROPERTY(m_TestMatrix);
}


void TestClass::SerializeOut(nlohmann::ordered_json& object)
{
	

}

void TestClass::SerializeIn(nlohmann::ordered_json& object)
{	


}

