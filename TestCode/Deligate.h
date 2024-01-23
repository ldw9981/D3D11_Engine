#pragma once

void TestDeligate();

class Deligate
{
public:
	Deligate() {};
	~Deligate() {};

	std::unordered_map<size_t,std::function<void()>> m_Listeners;

	void operator+=(std::function<void()> func)
	{
		m_Listeners.insert(std::make_pair(func.target_type().hash_code(),func));
	}

	void Invoke()
	{
		for (auto& Listener : m_Listeners)
		{
			Listener.second();
		}
	}	
	void operator-=(std::function<void()> func)
	{
		auto it = m_Listeners.find(func.target_type().hash_code());
		if (it != m_Listeners.end())
		{
			m_Listeners.erase(it);
		}
	}
};


template<typename T>
class Deligate1
{
public:
	Deligate1() {};
	~Deligate1() {};

	std::unordered_map<size_t, std::function<void()>> m_Listeners;

	void operator+=(std::function<void(T)> func)
	{
		m_Listeners.insert(std::make_pair(func.target_type().hash_code(), func));
	}

	void Invoke(T t)
	{
		for (auto& func : m_Listeners)
		{
			func(t);
		}
	}

	void operator-=(std::function<void()> func)
	{
		auto it = m_Listeners.find(func.target_type().hash_code());
		if (it != m_Listeners.end())
		{
			m_Listeners.erase(it);
		}
	}
};

