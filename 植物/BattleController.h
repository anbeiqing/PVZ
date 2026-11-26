#pragma once
#include"Controller.h"
#include"Timer.h"
class BattleController :public Controller {
	Timer OverallTimeHandle;
	Timer ShakeTimeHandle;
	int sunshineNum = 50;//阳光数，初始值对齐原作
public:
	BattleController();

	virtual void SetupInputComponent(InputComponent* inputComponent)override;
	void Update()override;

	void Shake();
	void MoveRight();

	//阳光计数
	int GetSunshineNum()const { return sunshineNum; }
	void SetSunshineNum(int num) { sunshineNum = num; }
	void AddSunshineNum(int num) { sunshineNum += num; }
};
