#include "Zombie.h"


Zombie::Zombie()
{
	renderer->SetLayer(2);
	walk.Load("normalwalk");
	walk.SetInterval(0.04f);
	ani->Insert("walk", walk);
	ani->SetNode("walk");


	eat.Load("normaleat");
	eat.SetInterval(0.06f);
	ani->Insert("eat", eat);
}

void Zombie::Update()
{
	BaseZombie::Update();
}
