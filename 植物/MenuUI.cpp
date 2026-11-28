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



}
