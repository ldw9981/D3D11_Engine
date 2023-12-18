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
        std::cout << "버튼이 클릭되었습니다: " << buttonText << std::endl;
        onClick();
    }

    virtual void onClick() 
    {
        std::cout << "OnClick 메세지 출력" << std::endl;
    }

private:
    std::string buttonText;
};

class MyButton : public Button {
public:
    MyButton(const std::string& text) : Button(text) {}

    // 재정의
    virtual void onClick()  
    {
        std::cout << "MyButton 특정 동작 수행" << std::endl;
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

    void AddListener(std::shared_ptr<IButtonNotify> ptr)
    {
        m_listeners.insert(ptr);
    }

    // 재정의
    virtual void onClick()
    {
        std::cout << "MyButton 특정 동작 수행" << std::endl;
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


class Other : public IButtonNotify
{
public:
    virtual void onClick() {
        std::cout << "Other OnClick" << std::endl;
    }
};


int main_TestButton() {

    //std::shared_ptr<Other> ptr = std::make_shared<Other>();
    NotifyButton button("클릭하세요");
    //button.AddListener(ptr);

    button.Click();

    return 0;
}
