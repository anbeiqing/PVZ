#pragma once
#include"UserInterface.h"


class MenuUI final : public UserInterface
{
private:
	void OnAdventureHoverBegin();
	void OnAdventureHoverEnd();
	void OnAdventureClicked();
	Vector2D adventureHomePos;
public:
	MenuUI();

	//全屏底图
	Image* Background = nullptr;
	Button* Adventure = nullptr;

};