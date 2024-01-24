#pragma once

void TestDeligate();

class Deligate
{
public:
	Deligate() {};
	~Deligate() {};

	std::unordered_map<size_t, std::function<void()>> m_Container;

	void operator+=(std::function<void()> func)
	{
		m_Container.insert(std::make_pair(func.target_type().hash_code(), func));
	}

	void Invoke()
	{
		for (auto& Node : m_Container)
		{
			Node.second();
		}
	}
	void operator-=(std::function<void()> func)
	{
		auto it = m_Container.find(func.target_type().hash_code());
		if (it != m_Container.end())
		{
			m_Container.erase(it);
		}
	}
};


template<typename T>
class Deligate1
{
public:
	Deligate1() {};
	~Deligate1() {};

	std::unordered_map<size_t, std::function<void(T)>> m_Container;

	void operator+=(std::function<void(T)> func)
	{
		m_Container.insert(std::make_pair(func.target_type().hash_code(), func));
	}

	void Invoke(T t)
	{
		for (auto& Node : m_Container)
		{
			Node.second(t);
		}
	}

	void operator-=(std::function<void(T)> func)
	{
		auto it = m_Container.find(func.target_type().hash_code());
		if (it != m_Container.end())
		{
			m_Container.erase(it);
		}
	}
};

