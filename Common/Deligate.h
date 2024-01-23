#pragma once
template<typename T>
class Deligate
{
public:
	Deligate() {};
	~Deligate() {};

	std::list<std::function<void(T)>> m_Listener;

	void operator+=(std::function<void(T)> func)
	{
		m_Listener.push_back(func);
	}

	void Invoke(T t)
	{
		for (auto& func : m_Listener)
		{
			func(t);
		}
	}

	void operator-=(std::function<void(T)> func)
	{
		auto it = std::find_if(m_Listener.begin(), m_Listener.end(),
			[&func](const std::function<void(T)>& element) {
				// Compare based on the target of the std::function
				return element.target<void(T)>() == func.target<void(T)>();
			});

		if (it != m_Listener.end())
		{
			m_Listener.erase(it);
		}
	}
};

