#include "MenuLevel.h"
#include"MenuUI.h"
#include"GameStatic.h"

MenuLevel::MenuLevel() {}
void MenuLevel::Update() {}
void MenuLevel::BeginPlay()  
{
	MenuUI* ui = GameStatics::CreateUI<MenuUI>();
	if (ui) ui->AddToViewport();
}
