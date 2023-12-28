#include "pch.h"
#include "DelegateInterface.h"


int TestButton()
{
	std::shared_ptr<Light1> ptr1 = std::make_shared<Light1>();
	std::shared_ptr<Light2> ptr2 = std::make_shared<Light2>();

	NotifyButton button("클릭하세요");
	button.AddListener(ptr1.get(), ptr1);
	button.AddListener(ptr2.get(), ptr2);

	ptr2 = nullptr;

	button.Click();

	return 0;
}
