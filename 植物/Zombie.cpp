#include "Zombie.h"


Zombie::Zombie()
{
	renderer->SetLayer(2);
	walk.Load("normalwalk");
	walk.SetInterval(0.06f);
	ani->Insert("walk", walk);
	ani->SetNode("walk");
}
