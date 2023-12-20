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
        std::cout << "함정이닷!!" << std::endl;
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
       std::cout << "또다른 함정이다!!" << std::endl;
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

    // 재정의
    virtual void onClick()
    {		
        // 등록된 모든 리스너에게 알림
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
