#include "BattleController.h"
#include"Camera.h"

BattleController::BattleController()
{
	ShakeTimeHandle.Bind(1.f, this, &BattleController::Shake, false);
}

void BattleController::SetupInputComponent(InputComponent* inputComponent)
{
	Controller::SetupInputComponent(inputComponent);

	inputComponent->SetMapping("MoveRight",VK_D);
	inputComponent->BindAction("MoveRight", Pressed, this, &BattleController::MoveRight);
}

void BattleController::Update()
{
	
	Controller::Update();
	if (OverallTimeHandle.GetDelay()>2.f && OverallTimeHandle.GetDelay() <= 5.f && GetWorldPosition().x<260) {
		AddLocalPosition(Vector2D(3.f, 0));

	}
	if(OverallTimeHandle.GetDelay()>7.f&&GetWorldPosition().x>=100.f){
		AddLocalPosition(Vector2D(-3.f, 0));
	}
}

void BattleController::Shake()
{
	/*camera->ShakeCamera(10,1);*/
}

void BattleController::MoveRight()
{
	AddLocalPosition(Vector2D(100.f, 0));
}
