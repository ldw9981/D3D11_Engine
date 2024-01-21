#pragma once

enum class EPropertyType
{
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
	WString
};

struct Property
{
	EPropertyType Type;
	void* pValue;
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
	void  AddProperty(std::string Name,T* pData)
	{
		Property prop;
		if (typeid(T) == typeid(bool))
		{
			prop.Type = EPropertyType::Bool;
		}
		else if (typeid(T) == typeid(char))
		{
			prop.Type = EPropertyType::Char;
		}
		else if (typeid(T) == typeid(unsigned char))
		{
			prop.Type = EPropertyType::UnsignedChar;
		}
		else if (typeid(T) == typeid(short))
		{
			prop.Type = EPropertyType::Short;
		}
		else if (typeid(T) == typeid(unsigned short))
		{
			prop.Type = EPropertyType::UnsignedShort;
		}
		else if (typeid(T) == typeid(int))
		{
			prop.Type = EPropertyType::Int;
		}
		else if (typeid(T) == typeid(unsigned int))
		{
			prop.Type = EPropertyType::UnsignedInt;
		}
		else if (typeid(T) == typeid(long))
		{
			prop.Type = EPropertyType::Long;
		}
		else if (typeid(T) == typeid(unsigned long))
		{
			prop.Type = EPropertyType::UnsignedLong;
		}
		else if (typeid(T) == typeid(long long))
		{
			prop.Type = EPropertyType::LongLong;
		}
		else if (typeid(T) == typeid(unsigned long long))
		{
			prop.Type = EPropertyType::UnsignedLongLong;
		}
		else if (typeid(T) == typeid(float))
		{
			prop.Type = EPropertyType::Float;
		}
		else if (typeid(T) == typeid(double))
		{
			prop.Type = EPropertyType::Double;
		}
		else if (typeid(T) == typeid(long double))
		{
			prop.Type = EPropertyType::LongDouble;
		}
		else if (typeid(T) == typeid(Math::Vector2))
		{
			prop.Type = EPropertyType::Vector2;
		}
		else if (typeid(T) == typeid(Math::Vector3))
		{
			prop.Type = EPropertyType::Vector3;
		}
		else if (typeid(T) == typeid(Math::Vector4))
		{
			prop.Type = EPropertyType::Vector4;
		}
		else if (typeid(T) == typeid(Math::Matrix))
		{
			prop.Type = EPropertyType::Matrix;
		}
		else if (typeid(T) == typeid(std::string))
		{
			prop.Type = EPropertyType::String;
		}
		else if (typeid(T) == typeid(std::wstring))
		{
			prop.Type = EPropertyType::WString;
		}
		else
		{
			assert(false);
		}
		prop.pValue = (void*)pData;
		m_Properties.insert(std::make_pair(Name, prop));
	}	

	template<typename T>
	void SetProperty(const std::string& Name, T& input)
	{
		auto it = m_Properties.find(Name);
		if (it != m_Properties.end())
		{
			Property& prop = it->second;
			if (prop.Type == EPropertyType::Bool)
			{
				*(bool*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Char)
			{
				*(char*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::UnsignedChar)
			{
				*(unsigned char*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Short)
			{
				*(short*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::UnsignedShort)
			{
				*(unsigned short*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Int)
			{
				*(int*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::UnsignedInt)
			{
				*(unsigned int*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Long)
			{
				*(long*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::UnsignedLong)
			{
				*(unsigned long*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::LongLong)
			{
				*(long long*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::UnsignedLongLong)
			{
				*(unsigned long long*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Float)
			{
				*(float*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Double)
			{
				*(double*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::LongDouble)
			{
				*(long double*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Vector2)
			{
				*(Math::Vector2*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Vector3)
			{
				*(Math::Vector3*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Vector4)
			{
				*(Math::Vector4*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::Matrix)
			{
				*(Math::Matrix*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::String)
			{
				*(std::string*)prop.pValue = input;
			}
			else if (prop.Type == EPropertyType::WString)
			{
				*(std::wstring*)prop.pValue = input;
			}
			else
			{
				assert(false);
			}
		}
	}
	template<typename T>
	void GetProperty(const std::string& Name, T& output)
	{
		auto it = m_Properties.find(Name);
		if (it != m_Properties.end())
		{
			Property& prop = it->second;
			if (prop.Type == EPropertyType::Bool)
			{
				output = *(bool*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Char)
			{
				output = *(char*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::UnsignedChar)
			{
				output = *(unsigned char*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Short)
			{
				output = *(short*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::UnsignedShort)
			{
				output = *(unsigned short*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Int)
			{
				output = *(int*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::UnsignedInt)
			{
				output = *(unsigned int*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Long)
			{
				output = *(long*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::UnsignedLong)
			{
				output = *(unsigned long*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::LongLong)
			{
				output = *(long long*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::UnsignedLongLong)
			{
				output = *(unsigned long long*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Float)
			{
				output = *(float*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Double)
			{
				output = *(double*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::LongDouble)
			{
				output = *(long double*)prop.pValue;
			}
			/*
			else if (prop.Type == EPropertyType::Vector2)
			{
				output = *((Math::Vector2*)(prop.pValue));
			}
			else if (prop.Type == EPropertyType::Vector3)
			{
				output = *(Math::Vector3*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Vector4)
			{
				output = *(Math::Vector4*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::Matrix)
			{
				output = *(Math::Matrix*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::String)
			{
				output = *(std::string*)prop.pValue;
			}
			else if (prop.Type == EPropertyType::WString)
			{
				output = *(std::wstring*)prop.pValue;
			}
			*/
			else
			{
				assert(false);
			}
		}
	}	
};



class TestClass : public PropertyContainer
{
public:
	TestClass();
	~TestClass() {};
public:
	int m_TestInt=333;
	Math::Matrix m_TestMatrix;

	void SerializeOut(nlohmann::ordered_json& object);

	void SerializeIn(nlohmann::ordered_json& object);
};