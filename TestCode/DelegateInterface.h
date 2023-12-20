#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>

/*
    옵저버(리스너)패턴으로 구현한 위임처리
    
    위임: 실제 처리를 다른 객체에게 맡기는 것.  기능과 동작(컨텐츠)의 구현을 분리 시킨다.
    
    옵저버 패턴: 객체의 상태 변화를 관찰하는 관찰자들(옵저버:이벤트통지를 받는객체) 목록을 객체에 등록하여 
    상태 변화가 있을 때마다 메서드 등을 통해 객체가 직접 목록의 각 옵저버에게 통지하도록 하는 디자인 패턴
    
    인터페이스 포인터를 보관하고 직접 함수 호출하는방식은 템플릿으로 일반화가 불가능하다.
	std::function을 사용하면 템플릿으로 일반화가 가능하다.
*/

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
