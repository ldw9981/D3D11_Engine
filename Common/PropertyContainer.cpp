#include "pch.h"
#include "PropertyContainer.h"


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
		else if (property.second.Type == EPropertyType::WString)
		{
			std::wstring value;
			GetPropertyData<std::wstring>(property.first, value);
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
		else if (property.second.Type == EPropertyType::WString)
		{
			std::wstring value = object[property.first];
			SetPropertyData<std::wstring>(property.first, value);
		}
	}
}


void PropertyContainer::OnRenderImGUI()
{
	for (auto& property : m_Properties)
	{
		if (property.second.Type == EPropertyType::Bool)
		{
			bool* pPtr = (bool*)property.second.Ptr;
			ImGui::Checkbox(property.first.c_str(), pPtr);
			
		}
		else if (property.second.Type == EPropertyType::Int)
		{
			int* pPtr = (int*)property.second.Ptr;
			ImGui::DragInt(property.first.c_str(), pPtr);
		}
		else if (property.second.Type == EPropertyType::Float)
		{
			float* pPtr = (float*)property.second.Ptr;
			ImGui::DragFloat(property.first.c_str(), pPtr);
		}
		else if (property.second.Type == EPropertyType::Vector2)
		{
			Math::Vector2* pPtr = (Math::Vector2*)property.second.Ptr;
			ImGui::DragFloat2(property.first.c_str(), (float*)pPtr);
		}
		else if (property.second.Type == EPropertyType::Vector3)
		{
			Math::Vector3* pPtr = (Math::Vector3*)property.second.Ptr;
			ImGui::DragFloat3(property.first.c_str(), (float*)pPtr);
		}
		else if (property.second.Type == EPropertyType::Vector4)
		{
			Math::Vector4* pPtr = (Math::Vector4*)property.second.Ptr;
			ImGui::DragFloat4(property.first.c_str(), (float*)pPtr);
		}
		else if (property.second.Type == EPropertyType::Matrix)
		{
			Math::Matrix* pPtr = (Math::Matrix*)property.second.Ptr;
			ImGui::DragFloat4(property.first.c_str(), (float*)pPtr);
			ImGui::DragFloat4(property.first.c_str(), (float*)pPtr + 4);
			ImGui::DragFloat4(property.first.c_str(), (float*)pPtr + 8);
			ImGui::DragFloat4(property.first.c_str(), (float*)pPtr + 12);
		}
		else if (property.second.Type == EPropertyType::String)
		{
			std::string* pPtr = (std::string*)property.second.Ptr;		
			ImGui::InputText(property.first.c_str(), pPtr);
		}
		else if (property.second.Type == EPropertyType::WString)
		{
			//std::wstring* pPtr = (std::wstring*)property.second.Ptr;
			//ImGui::InputText(property.first.c_str(), (char*)pPtr, 256);
			
		}
	}
}