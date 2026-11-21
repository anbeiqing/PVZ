#pragma once
#include"Controller.h"
#include"Timer.h"
class BattleController :public Controller {
	Timer OverallTimeHandle;
	Timer ShakeTimeHandle;
public:
	BattleController();

	virtual void SetupInputComponent(InputComponent* inputComponent)override;
	void Update()override;

	void Shake();
	void MoveRight();
};
