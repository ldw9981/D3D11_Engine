#pragma once

void TestDeligate();

class Deligate
{
public:
	Deligate() {};
	~Deligate() {};

	std::multimap<size_t, std::function<void()>> m_Container;

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

	std::multimap<size_t, std::function<void(T)>> m_Container;

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

template<typename T1,typename T2>
class Deligate2
{
public:
	Deligate2() {};
	~Deligate2() {};

	std::multimap<size_t, std::function<void(T1,T2)>> m_Container;

	void operator+=(std::function<void(T1, T2)> func)
	{
		m_Container.insert(std::make_pair(func.target_type().hash_code(), func));
	}

	void Invoke(T1 t1, T2 t2)
	{
		for (auto& Node : m_Container)
		{
			Node.second(t1,t2);
		}
	}

	void operator-=(std::function<void(T1, T2)> func)
	{
		auto it = m_Container.find(func.target_type().hash_code());
		if (it != m_Container.end())
		{
			m_Container.erase(it);
		}
	}
};

template<typename T1, typename T2, typename T3>
class Deligate3
{
public:
	Deligate3() {};
	~Deligate3() {};

	std::multimap<size_t, std::function<void(T1, T2, T3)>> m_Container;

	void operator+=(std::function<void(T1, T2, T3)> func)
	{
		m_Container.insert(std::make_pair(func.target_type().hash_code(), func));
	}

	void Invoke(T1 t1, T2 t2, T3 t3)
	{
		for (auto& Node : m_Container)
		{
			Node.second(t1, t2, t3);
		}
	}

	void operator-=(std::function<void(T1, T2, T3)> func)
	{
		auto it = m_Container.find(func.target_type().hash_code());
		if (it != m_Container.end())
		{
			m_Container.erase(it);
		}
	}
};

template<typename T1, typename T2, typename T3,typename T4>
class Deligate4
{
public:
	Deligate4() {};
	~Deligate4() {};

	std::multimap<size_t, std::function<void(T1, T2, T3, T4)>> m_Container;

	void operator+=(std::function<void(T1, T2, T3, T4)> func)
	{
		m_Container.insert(std::make_pair(func.target_type().hash_code(), func));
	}

	void Invoke(T1 t1, T2 t2, T3 t3, T4 t4)
	{
		for (auto& Node : m_Container)
		{
			Node.second(t1, t2, t3, t4);
		}
	}

	void operator-=(std::function<void(T1, T2, T3, T4)> func)
	{
		auto it = m_Container.find(func.target_type().hash_code());
		if (it != m_Container.end())
		{
			m_Container.erase(it);
		}
	}
};