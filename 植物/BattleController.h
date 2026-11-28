#pragma once
#include"Controller.h"
#include"Timer.h"
#include"BasePlant.h"
class BattleController :public Controller {

	friend class SeedUI;

	Timer OverallTimeHandle;

	Timer SunshineTimerHandle;
	Timer ShakeTimeHandle;
	int sunshineNum = 50;//阳光数，初始值对齐原作

	BasePlant* seedToPlant = nullptr;
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
	void PlantSeed();
	//天上掉阳光
	void ProduceSun();

	void ProduceZombie();
};
