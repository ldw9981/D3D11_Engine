#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>

/*
    ������(������)�������� ������ ����ó��
    
    ����: ���� ó���� �ٸ� ��ü���� �ñ�� ��.  ��ɰ� ����(������)�� ������ �и� ��Ų��.
    
    ������ ����: ��ü�� ���� ��ȭ�� �����ϴ� �����ڵ�(������:�̺�Ʈ������ �޴°�ü) ����� ��ü�� ����Ͽ� 
    ���� ��ȭ�� ���� ������ �޼��� ���� ���� ��ü�� ���� ����� �� ���������� �����ϵ��� �ϴ� ������ ����
    
    �������̽� �����͸� �����ϰ� ���� �Լ� ȣ���ϴ¹���� ���ø����� �Ϲ�ȭ�� �Ұ����ϴ�.
	std::function�� ����ϸ� ���ø����� �Ϲ�ȭ�� �����ϴ�.
*/

class Button {
public:
    Button(const std::string& text) : buttonText(text) {}

    void Click() 
    {
        std::cout << "��ư�� Ŭ���Ǿ����ϴ�: " << buttonText << std::endl;
        onClick();
    }

    virtual void onClick() 
    {
        std::cout << "�����̴�!!" << std::endl;
    }

private:
    std::string buttonText;
};

class MyButton : public Button {
public:
    MyButton(const std::string& text) : Button(text) {}

    // ������
    virtual void onClick()  
    {
       std::cout << "�Ǵٸ� �����̴�!!" << std::endl;
    }
};


class IButtonNotify
{
public:
    virtual void onClick() = 0;
};

class NotifyButton : public Button {
public:
    NotifyButton(const std::string& text) : Button(text) {}

    std::map<IButtonNotify*,std::weak_ptr<IButtonNotify>> m_listeners;


	void AddListener(IButtonNotify* pInstance, std::weak_ptr<IButtonNotify> ptr)
	{
		m_listeners[pInstance] = ptr;
	}

    void RemoveListener(IButtonNotify* pInstance)
    {
   		m_listeners.erase(pInstance);
    }

    // ������
    virtual void onClick()
    {	
        // ��ϵ� ��� �����ʿ��� �˸�
        for (auto it = m_listeners.begin(); it != m_listeners.end(); )
        {
            if (auto sp = it->second.lock())
            {
				sp->onClick();
				++it;
			}
            else
            {
				it = m_listeners.erase(it);
			}
		}       
	} 
};


class Light1 : public IButtonNotify
{
public:
    virtual void onClick() {
        std::cout << "Light1 OnClick" << std::endl;
    }
};

class Light2 : public IButtonNotify
{
public:
	virtual void onClick() {
		std::cout << "Light2 OnClick" << std::endl;
	}
};



int TestButton();
