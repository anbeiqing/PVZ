#pragma once
#include"UserInterface.h"


class BattleUI :public UserInterface
{
	Image* SeedBank;
	HorizontalPanel* Panel;
	Text* Sunshine;
public:
	BattleUI();
	void Update();
};
