#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>

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
        std::cout << "OnClick �޼��� ���" << std::endl;
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
        std::cout << "MyButton Ư�� ���� ����" << std::endl;
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

    std::set<std::weak_ptr<IButtonNotify>> m_listeners;

    // ������
    virtual void onClick()
    {
        std::cout << "MyButton Ư�� ���� ����" << std::endl;
        for (auto it = m_listeners.begin(); it!= m_listeners.end();it++)
        {
            if ((*it).expired())
            {
                it = m_listeners.erase(it);
            }
            else
            {
                auto ptr = (*it).lock();
                ptr->onClick();
            }
        }
      
    }
};





int main_TestButton() {
    MyButton myButton("Ŭ���ϼ���");
    myButton.Click();

    return 0;
}
