#include "pch.h"
#include "PropertyContainer.h"


void TestPropertyContainer()
{
	TestClass b;

	int resultInt;
	Math::Matrix resultMat;
	b.GetPropertyData<int>(std::string("m_TestInt"), resultInt);
	b.GetPropertyData<Math::Matrix>(std::string("m_TestMatrix"), resultMat);
}

void TestClass::SerializeOut(nlohmann::ordered_json& object)
{
	

}

void TestClass::SerializeIn(nlohmann::ordered_json& object)
{	


}

