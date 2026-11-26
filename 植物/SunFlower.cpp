#include "SunFlower.h"
#include"GameStatic.h"

SunFlower::SunFlower()
{
	idle.Load("sunflower");
	idle.SetInterval(0.06f);
	ani->Insert("idle", idle);
	ani->SetNode("idle");
	ProduceTimerHandle.Bind(20, [this]() {state = 1;}, true, 7.5);
}

void SunFlower::Update()
{
	BasePlant::Update();
	if (state == 1) {
		transitionLevel += 0.4f;
		if (transitionLevel >= 45) {
			Produce();
			state = 2;
		}
	}
	else if (state == 2) {
		transitionLevel -= 0.3f;
		if (transitionLevel <= 1) {
			state = 0;
			transitionLevel = 0;

		}
	}
}

void SunFlower::Produce()
{
	Sun* sunshine = GameStatics::CreateObject<Sun>(GetWorldPosition());
	if (sunshine)sunshine->Throw(GetWorldPosition().y + 25);
}
