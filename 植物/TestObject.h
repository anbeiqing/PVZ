#pragma once
#include "coreMinimal.h"
#include"Timer.h"
class TestObject : public Object
{
	Timer timer;
public:
	TestObject() {
		timer.Bind(0.5, this, &TestObject::func, true);
	
	};
	
	void func() {
		std::cout << "TestObject func";
	}
};