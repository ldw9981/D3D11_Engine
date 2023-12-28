#include "pch.h"
#include "TestConstruct.h"
#include <iostream>
#include <vector>

using namespace std;


class Item {
public:
	Item() : m_nx(0) { cout << this << "�⺻ ������ ȣ��" << endl; }

	Item(const int _n) : m_nx(_n) { cout << this << "�Ϲ� ������ ȣ��" << endl; }

	Item(const Item& rhs) : m_nx(rhs.m_nx) { cout << this << "���� ������ ȣ��" << endl; }

	Item(const Item&& rhs) : m_nx(std::move(rhs.m_nx)) { cout << this << "�̵� ������ ȣ��" << endl; }

	~Item() { cout << this << "�Ҹ��� ȣ��" << endl; }

private:
	int m_nx;
};

int main_TestConstruct() {
	std::vector<Item> v;

	//	cout << "push_back ȣ��" << endl;
	//	v.push_back(Item(3));

	//	cout << "emplace_back 3 ȣ��" << endl;
	//	v.emplace_back(3)

	//	cout << "emplace_back Item()ȣ��" << endl;
	//	v.push_back(Item());

	cout << "emplace_back ȣ��" << endl;
	v.emplace_back();

	cout << "emplace_back ȣ��" << endl;
	v.emplace_back();


	cout << "��" << endl;
	return 1;
}