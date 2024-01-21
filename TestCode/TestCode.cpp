// TestCode.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

#include "GameObjectMixIn.h"

#include "PropertyContainer.h"

int main()
{
	GameObjectMixIn a;
	a.Update(1.0f);


	TestClass b;

	int result;
	b.GetProperty<int>(std::string("m_TestInt"), result);
	
}
