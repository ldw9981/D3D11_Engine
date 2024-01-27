#pragma once

void TestPropertyContainer();

enum class EPropertyType
{
	None,
	Bool,	
	Int,	
	Float,	
	Vector2,
	Vector3,
	Vector4,
	Matrix,
	String,
	WString,	
};

struct Property
{
	EPropertyType Type;
	void* Ptr;
};

#define ADD_PROPERTY(Name) \
	AddProperty(#Name, &Name)

class PropertyContainer
{
public:
	PropertyContainer() {};
	~PropertyContainer() {};

	std::map<std::string, Property> m_Properties;

	template<typename T>
	EPropertyType GetPropertyType(T Data)
	{		
		if (typeid(T) == typeid(bool))
		{
			return EPropertyType::Bool;
		}	
		else if (typeid(T) == typeid(int))
		{
			return EPropertyType::Int;
		}
		else if (typeid(T) == typeid(float))
		{
			return EPropertyType::Float;
		}
		else if (typeid(T) == typeid(Math::Vector2))
		{
			return EPropertyType::Vector2;
		}
		else if (typeid(T) == typeid(Math::Vector3))
		{
			return EPropertyType::Vector3;
		}
		else if (typeid(T) == typeid(Math::Vector4))
		{
			return EPropertyType::Vector4;
		}
		else if (typeid(T) == typeid(Math::Matrix))
		{
			return EPropertyType::Matrix;
		}
		else if (typeid(T) == typeid(std::string))
		{
			return EPropertyType::String;
		}
		else if (typeid(T) == typeid(std::wstring))
		{
			return EPropertyType::WString;
		}
		assert(false);
		return EPropertyType::None;
	}
	template<typename T>
	void  AddProperty(std::string Name,T* pData)
	{
		Property prop;
		T test = {};
		prop.Type = GetPropertyType<T>(test);		
		prop.Ptr = (void*)pData;
		m_Properties.insert(std::make_pair(Name, prop));
	}	

	template<typename T>
	bool SetPropertyData(const std::string& Name, T& input)
	{
		auto it = m_Properties.find(Name);
		T test = {};
		EPropertyType inputType = GetPropertyType<T>(test);

		if (it == m_Properties.end())
			return false;		
		
		Property& prop = it->second;
		if (prop.Type != inputType)
			return false;
		
		*(T*)prop.Ptr = input;		
		return true;
	}
	template<typename T>
	bool GetPropertyData(const std::string& Name, T& output)
	{
		auto it = m_Properties.find(Name);
		T test = {};
		EPropertyType outputType = GetPropertyType<T>(test);

		if (it == m_Properties.end())
			return false;

		Property& prop = it->second;
		if (prop.Type != outputType)
			return false;

		output = *static_cast<T*>(prop.Ptr);
		return true;
	}	

	void SerializeOut(nlohmann::ordered_json& object);
	void SerializeIn(nlohmann::ordered_json& object);
};


class TestClass : public PropertyContainer
{
public:
	TestClass()
	{
		m_TestInt = 666;
		m_TestMatrix = Math::Matrix::Identity;
		ADD_PROPERTY(m_TestInt);	// 		AddProperty("m_TestInt", &m_TestInt);
		ADD_PROPERTY(m_TestMatrix); //		AddProperty("m_TestMatrix", &m_TestMatrix);
	}
	~TestClass() {};

public:
	int m_TestInt;
	Math::Matrix m_TestMatrix;

	void SerializeOut(nlohmann::ordered_json& object);

	void SerializeIn(nlohmann::ordered_json& object);
};