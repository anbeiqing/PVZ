#include "MenuUI.h"
#include"GameStatic.h"

void MenuUI::OnAdventureHoverBegin()
{
	Adventure->SetRelativePosition(adventureHomePos + Vector2D(0, -4));
}

void MenuUI::OnAdventureHoverEnd()
{
	Adventure->SetRelativePosition(adventureHomePos);
}

void MenuUI::OnAdventureClicked()
{
	//这里只登记请求，真正切场景在 World::Update 帧末的 ApplyPendingLevel 里完成
	GameStatics::OpenLevel("Battle");
}

void MenuUI::SetupVase(Button*& button, Image*& glow, float cx, float cy, float w, float h)
{
	Vector2D center = Vector2D(cx - WIN_WIDTH * 0.5f, cy - WIN_HEIGHT * 0.5f);

	glow = AddWidget<Image>();
	glow->AttachTo(rootCanvas);
	glow->SetLayoutPattern(LayoutPattern::Center);
	glow->SetRelativePosition(center);
	glow->SetSize(Vector2D(w * 1.7f, h * 1.7f));
	glow->LoadSprite("vase_glow");
	glow->SetTransprancy(0);        //平时完全透明，悬停时才亮
	glow->SetLayer(99);

	
	button = AddWidget<Button>();
	button->AttachTo(rootCanvas);
	button->SetLayoutPattern(LayoutPattern::Center);
	button->SetRelativePosition(center);
	button->SetSize(Vector2D(w, h));
	button->SetLayer(100);
}

void MenuUI::OnOptionsHoverBegin() { OptionsGlow->SetTransprancy(160); }
void MenuUI::OnOptionsHoverEnd() { OptionsGlow->SetTransprancy(0); }
void MenuUI::OnHelpHoverBegin() { HelpGlow->SetTransprancy(160); }
void MenuUI::OnHelpHoverEnd() { HelpGlow->SetTransprancy(0); }
void MenuUI::OnQuitHoverBegin() { QuitGlow->SetTransprancy(160); }
void MenuUI::OnQuitHoverEnd() { QuitGlow->SetTransprancy(0); }

void MenuUI::OnQuitClicked()
{
	
	closegraph();
	exit(0);
}

void MenuUI::OnOptionsClicked() { ShowTip(TipOptions); }
void MenuUI::OnHelpClicked() { ShowTip(TipHelp); }

void MenuUI::ShowTip(Image* tip)
{
	TipHelp->SetTransprancy(0);
	TipOptions->SetTransprancy(0);
	tip->SetTransprancy(255);
	
	TipTimerHandle.Bind(3.5, this, &MenuUI::HideTips, false);
}

void MenuUI::HideTips()
{
	TipHelp->SetTransprancy(0);
	TipOptions->SetTransprancy(0);
}

MenuUI::MenuUI()
{
	Background = AddWidget<Image>();
	Background->AttachTo(rootCanvas);
	Background->SetLayoutPattern(LayoutPattern::Center);
	Background->SetRelativePosition(Vector2D(0, 0));
	Background->SetSize(Vector2D(WIN_WIDTH, WIN_HEIGHT));
	Background->LoadSprite("surface");
	Background->SetLayer(0);

	//冒险模式石牌：屏幕坐标(578,150) → 相对 rootCanvas 中心(400,300) 的偏移
	Adventure = AddWidget<Button>();
	Adventure->AttachTo(rootCanvas);
	Adventure->SetLayoutPattern(LayoutPattern::Center);
	adventureHomePos = Vector2D(578 - WIN_WIDTH * 0.5f, 150 - WIN_HEIGHT * 0.5f);
	Adventure->SetRelativePosition(adventureHomePos);
	Adventure->SetSize(Vector2D(331, 145));
	Adventure->LoadNormalPicture("adventure");
	Adventure->LoadClickPicture("adventure_down");
	Adventure->SetLayer(100);      //必须高于底图的 0 层

	//悬停时整块石牌上浮 4 像素：位置就是中心点，改它立刻生效
	Adventure->OnMouseHoverBegin.AddDynamic(this, &MenuUI::OnAdventureHoverBegin);
	Adventure->OnMouseHoverEnd.AddDynamic(this, &MenuUI::OnAdventureHoverEnd);
	Adventure->OnMousePressedEnd.AddDynamic(this, &MenuUI::OnAdventureClicked);

	SetupVase(Options, OptionsGlow, 598, 496, 86, 84);
	SetupVase(Help, HelpGlow, 668, 505, 56, 84);
	SetupVase(Quit, QuitGlow, 745, 505, 72, 92);

	Options->OnMouseHoverBegin.AddDynamic(this, &MenuUI::OnOptionsHoverBegin);
	Options->OnMouseHoverEnd.AddDynamic(this, &MenuUI::OnOptionsHoverEnd);
	Options->OnMousePressedEnd.AddDynamic(this, &MenuUI::OnOptionsClicked);

	Help->OnMouseHoverBegin.AddDynamic(this, &MenuUI::OnHelpHoverBegin);
	Help->OnMouseHoverEnd.AddDynamic(this, &MenuUI::OnHelpHoverEnd);
	Help->OnMousePressedEnd.AddDynamic(this, &MenuUI::OnHelpClicked);

	Quit->OnMouseHoverBegin.AddDynamic(this, &MenuUI::OnQuitHoverBegin);
	Quit->OnMouseHoverEnd.AddDynamic(this, &MenuUI::OnQuitHoverEnd);
	Quit->OnMousePressedEnd.AddDynamic(this, &MenuUI::OnQuitClicked);

	//提示面板：两张都常驻，靠 alpha 0/255 显隐
	TipHelp = AddWidget<Image>();
	TipHelp->AttachTo(rootCanvas);
	TipHelp->SetLayoutPattern(LayoutPattern::Center);
	TipHelp->SetRelativePosition(Vector2D(0, -10));
	TipHelp->SetSize(Vector2D(520, 230));
	TipHelp->LoadSprite("tip_help");
	TipHelp->SetTransprancy(0);
	TipHelp->SetLayer(200);

	TipOptions = AddWidget<Image>();
	TipOptions->AttachTo(rootCanvas);
	TipOptions->SetLayoutPattern(LayoutPattern::Center);
	TipOptions->SetRelativePosition(Vector2D(0, -10));
	TipOptions->SetSize(Vector2D(520, 230));
	TipOptions->LoadSprite("tip_options");
	TipOptions->SetTransprancy(0);
	TipOptions->SetLayer(200);



}
