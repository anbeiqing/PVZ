#pragma once
#include"UserInterface.h"
#include"Timer.h"


class MenuUI final : public UserInterface
{
private:
	void OnAdventureHoverBegin();
	void OnAdventureHoverEnd();
	void OnAdventureClicked();
	Vector2D adventureHomePos;
	//花瓶
	void SetupVase(Button*& button, Image*& glow, float cx, float cy, float w, float h);
	void OnOptionsHoverBegin(); 
	void OnOptionsHoverEnd();
	void OnHelpHoverBegin();  
	void OnHelpHoverEnd();
	void OnQuitHoverBegin();   
	void OnQuitHoverEnd();
	void OnOptionsClicked(); 
	void OnHelpClicked(); 
	void OnQuitClicked();

	void ShowTip(Image* tip);
	void HideTips();

	Timer TipTimerHandle;

public:
	MenuUI();

	//全屏底图
	Image* Background = nullptr;
	Button* Adventure = nullptr;

	Button* Options = nullptr;
	Button* Help = nullptr;
	Button* Quit = nullptr;

	Image* OptionsGlow = nullptr;
	Image* HelpGlow = nullptr;
	Image* QuitGlow = nullptr;

	Image* TipHelp = nullptr;
	Image* TipOptions = nullptr;

};