#pragma once

enum class EPropertyType
{
	None,
	Bool,
	Char,
	UnsignedChar,
	Short,
	UnsignedShort,
	Int,
	UnsignedInt,
	Long,
	UnsignedLong,
	LongLong,
	UnsignedLongLong,
	Float,
	Double,
	LongDouble,
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
		else if (typeid(T) == typeid(char))
		{
			return EPropertyType::Char;
		}
		else if (typeid(T) == typeid(unsigned char))
		{
			return EPropertyType::UnsignedChar;
		}
		else if (typeid(T) == typeid(short))
		{
			return EPropertyType::Short;
		}
		else if (typeid(T) == typeid(unsigned short))
		{
			return EPropertyType::UnsignedShort;
		}
		else if (typeid(T) == typeid(int))
		{
			return EPropertyType::Int;
		}
		else if (typeid(T) == typeid(unsigned int))
		{
			return EPropertyType::UnsignedInt;
		}
		else if (typeid(T) == typeid(long))
		{
			return EPropertyType::Long;
		}
		else if (typeid(T) == typeid(unsigned long))
		{
			return EPropertyType::UnsignedLong;
		}
		else if (typeid(T) == typeid(long long))
		{
			return EPropertyType::LongLong;
		}
		else if (typeid(T) == typeid(unsigned long long))
		{
			return EPropertyType::UnsignedLongLong;
		}
		else if (typeid(T) == typeid(float))
		{
			return EPropertyType::Float;
		}
		else if (typeid(T) == typeid(double))
		{
			return EPropertyType::Double;
		}
		else if (typeid(T) == typeid(long double))
		{
			return EPropertyType::LongDouble;
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
	void SetPropertyData(const std::string& Name, T& input)
	{
		auto it = m_Properties.find(Name);
		T test = {};
		EPropertyType inputType = GetPropertyType<T>(test);

		if (it == m_Properties.end())
			return;		
		
		Property& prop = it->second;
		if (prop.Type != inputType)
			return;
		
		if (prop.Type == EPropertyType::Bool)
		{
			*(bool*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Char)
		{
			*(char*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::UnsignedChar)
		{
			*(unsigned char*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Short)
		{
			*(short*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::UnsignedShort)
		{
			*(unsigned short*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Int)
		{
			*(int*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::UnsignedInt)
		{
			*(unsigned int*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Long)
		{
			*(long*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::UnsignedLong)
		{
			*(unsigned long*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::LongLong)
		{
			*(long long*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::UnsignedLongLong)
		{
			*(unsigned long long*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Float)
		{
			*(float*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Double)
		{
			*(double*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::LongDouble)
		{
			*(long double*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Vector2)
		{
			*(Math::Vector2*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Vector3)
		{
			*(Math::Vector3*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Vector4)
		{
			*(Math::Vector4*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::Matrix)
		{
			*(Math::Matrix*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::String)
		{
			*(std::string*)prop.Ptr = input;
		}
		else if (prop.Type == EPropertyType::WString)
		{
			*(std::wstring*)prop.Ptr = input;
		}	
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
};



class TestClass : public PropertyContainer
{
public:
	TestClass();
	~TestClass() {};

public:
	int m_TestInt;
	Math::Matrix m_TestMatrix;

	void SerializeOut(nlohmann::ordered_json& object);

	void SerializeIn(nlohmann::ordered_json& object);
};