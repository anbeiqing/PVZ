#include "BattleUI.h"
#include"SeedUI.h"
#include"BattleController.h"
#include"GameStatic.h"

BattleUI::BattleUI()
{
	SeedBank = AddWidget<Image>();
	SeedBank->AttachTo(rootCanvas);
	SeedBank->SetLayoutPattern(LayoutPattern::MiddleTop);
	SeedBank->SetRelativePosition(Vector2D(-130, 45));
	SeedBank->SetSize(Vector2D(530, 91));
	SeedBank->LoadSprite("seedbank");
	//UI 必须画在背景之上：Layers 按图层号从小到大渲染，背景渲染器是 0 层，
	//这里原来用 -3/-2，等于把整个种子银行画到了背景下面，所以"加了但不显示"
	SeedBank->SetLayer(100);

	Panel = AddWidget<HorizontalPanel>();
	Panel->AttachTo(SeedBank);
	Panel->SetLayoutPattern(LayoutPattern::Center);
	Panel->SetRelativePosition(Vector2D(33, 0));
	Panel->SetUnitSize(Vector2D(54, 77));
	Panel->SetSpacing(2);
	Panel->SetLayer(101);

	Sunshine = AddWidget<Text>();
	Sunshine->AttachTo(SeedBank);
	Sunshine->SetRelativePosition(Vector2D(38, 77));
	Sunshine->SetLayer(105);

	for (int i = 0; i < 8; i++)
	{
		SeedUI* ui = GameStatics::CreateUI<SeedUI>();
		ui->Init(i);
		Panel->AddMember(ui);
	}
}

void BattleUI::Update()
{
	UserInterface::Update();

	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
	{
		Sunshine->SetText(std::to_string(pController->GetSunshineNum()), 4);
	}
}
