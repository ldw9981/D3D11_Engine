#include "pch.h"
#include "PropertyContainer.h"
#include <fstream>
void PropertyContainer::SerializeOut(nlohmann::ordered_json& object)
{
	for (auto& property : m_Properties)
	{
		if (property.second.Type == EPropertyType::Bool)
		{
			bool value;
			GetPropertyData<bool>(property.first, value);
			object[property.first] = value;
		}
		else if (property.second.Type == EPropertyType::Int)
		{
			int value;
			GetPropertyData<int>(property.first, value);
			object[property.first] = value;
		}
		else if (property.second.Type == EPropertyType::Float)
		{
			float value;
			GetPropertyData<float>(property.first, value);
			object[property.first] = value;
		}
		else if (property.second.Type == EPropertyType::Vector2)
		{
			Math::Vector2 value;
			GetPropertyData<Math::Vector2>(property.first, value);
			object[property.first] = { value.x, value.y };
		}
		else if (property.second.Type == EPropertyType::Vector3)
		{
			Math::Vector3 value;
			GetPropertyData<Math::Vector3>(property.first, value);
			object[property.first] = { value.x, value.y, value.z };
		}
		else if (property.second.Type == EPropertyType::Vector4)
		{
			Math::Vector4 value;
			GetPropertyData<Math::Vector4>(property.first, value);
			object[property.first] = { value.x, value.y, value.z, value.w };
		}
		else if (property.second.Type == EPropertyType::Matrix)
		{
			Math::Matrix value;
			GetPropertyData<Math::Matrix>(property.first, value);
			object[property.first] = { value._11, value._12, value._13, value._14,
									   value._21, value._22, value._23, value._24,
									   value._31, value._32, value._33, value._34,
									   value._41, value._42, value._43, value._44 };
		}
		else if (property.second.Type == EPropertyType::String)
		{
			std::string value;
			GetPropertyData<std::string>(property.first, value);
			object[property.first] = value;
		}	
		else if (property.second.Type == EPropertyType::Path)
		{
			std::filesystem::path value;
			GetPropertyData<std::filesystem::path>(property.first, value);
			object[property.first] = value;
		}
	}
}

void PropertyContainer::SerializeIn(nlohmann::ordered_json& object)
{
	for (auto& property : m_Properties)
	{
		if (property.second.Type == EPropertyType::Bool)
		{
			bool value = object[property.first];
			SetPropertyData<bool>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Int)
		{
			int value = object[property.first];
			SetPropertyData<int>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Float)
		{
			float value = object[property.first];
			SetPropertyData<float>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Vector2)
		{
			Math::Vector2 value = { object[property.first][0],object[property.first][1] };
			SetPropertyData<Math::Vector2>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Vector3)
		{
			Math::Vector3 value = { object[property.first][0],object[property.first][1],object[property.first][2] };
			SetPropertyData<Math::Vector3>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Vector4)
		{
			Math::Vector4 value = { object[property.first][0],object[property.first][1],object[property.first][2],object[property.first][3] };
			SetPropertyData<Math::Vector4>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Matrix)
		{
			Math::Matrix value = { object[property.first][0],object[property.first][1],object[property.first][2],object[property.first][3],
								   object[property.first][4],object[property.first][5],object[property.first][6],object[property.first][7],
								   object[property.first][8],object[property.first][9],object[property.first][10],object[property.first][11],
								   object[property.first][12],object[property.first][13],object[property.first][14],object[property.first][15] };
			SetPropertyData<Math::Matrix>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::String)
		{
			std::string value = object[property.first];
			SetPropertyData<std::string>(property.first, value);
		}
		else if (property.second.Type == EPropertyType::Path)
		{
			std::filesystem::path value = object[property.first];
			SetPropertyData<std::filesystem::path>(property.first, value);
		}
	}
}


void PropertyContainer::OnRenderImGUI()
{

}

void TestPropertyContainer()
{
	TestClass b;

	std::string a = typeid(Math::Vector2).name();
	std::filesystem::path path;

	int resultInt;
	Math::Matrix resultMat;
	b.GetPropertyData<int>(std::string("m_TestInt"), resultInt);
	b.GetPropertyData<Math::Matrix>(std::string("m_TestMatrix"), resultMat);
	b.GetPropertyData<std::filesystem::path>(std::string("m_TestPath"), path);

	nlohmann::ordered_json object;
	b.SerializeOut(object);

	std::ofstream ofs("test.json");
	ofs << object.dump(2);
	ofs.close();
}

void TestClass::SerializeOut(nlohmann::ordered_json& object)
{	
	object["ClassName"] = typeid(this).name();
	PropertyContainer::SerializeOut(object);

}

void TestClass::SerializeIn(nlohmann::ordered_json& object)
{	
	PropertyContainer::SerializeIn(object);
}

