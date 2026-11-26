#pragma once
#include"BasePlant.h"
#include"sun.h"


//向日葵
class SunFlower :public BasePlant
{
	Animation idle;
	Timer ProduceTimerHandle;

	float transitionLevel = 0.f;
	int state = 0;
public:
	SunFlower();
	//必须是 Update 且与基类虚函数签名一致；写成 Upadte 只是新函数，永远不会被每帧调用
	virtual void Update() override;
	void Produce();
};
