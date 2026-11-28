#include "Wallnut.h"

Wallnut::Wallnut()
{
	this->blood = 100;
	idle.Load("wallnut");
	idle.SetInterval(0.06f);
	idle1.Load("wallnut_1");
	idle1.SetInterval(0.06f);
	idle2.Load("wallnut_2");
	idle2.SetInterval(0.06f);

	ani->Insert("idle", idle);
	ani->Insert("idle1", idle1);
	ani->Insert("idle2", idle2);

	

}

void Wallnut::BeginPlay()
{
	;
}

void Wallnut::Update()
{
	Sprite::Update();
	if(blood>=60)ani->SetNode("idle");
	if (blood >= 20&&blood<60)ani->SetNode("idle1");
	if (blood >= 0&&blood<20)ani->SetNode("idle2");


}
