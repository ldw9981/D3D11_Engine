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
        //std::cout << "��ư�� Ŭ���Ǿ����ϴ�: " << buttonText << std::endl;
        onClick();
    }

    virtual void onClick() 
    {
        //std::cout << "OnClick �޼��� ���" << std::endl;
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
       // std::cout << "MyButton Ư�� ���� ����" << std::endl;
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

    //std::set<std::weak_ptr<IButtonNotify>> m_listeners;

    void AddListener(std::shared_ptr<IButtonNotify> ptr)
    {
       
    }

    // ������
    virtual void onClick()
    {
        std::cout << "MyButton Ư�� ���� ����" << std::endl;
       
      
    }
};


class Other : public IButtonNotify
{
public:
    virtual void onClick() {
        std::cout << "Other OnClick" << std::endl;
    }
};


int main_TestButton() {

    //std::shared_ptr<Other> ptr = std::make_shared<Other>();
    //NotifyButton button("Ŭ���ϼ���");
    //button.AddListener(ptr);

    //button.Click();

    return 0;
}
