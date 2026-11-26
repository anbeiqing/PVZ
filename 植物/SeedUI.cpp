#include "SeedUI.h"
#include "BattleController.h"
#include "GameStatic.h"
#include"PeaShooter.h"
#include"SunFlower.h"

std::string plantName[] = { "豌豆射手","向日葵","樱桃炸弹","坚果","土豆地雷","寒冰射手","大嘴花","双发射手" };
int plantPrice[] = { 100,50,150,50,25,175,150,200 };
float coolingTime[] = { 5,5,45,30,30,5,5,5 };


SeedUI::SeedUI()
{
	//种子卡片的根画布必须缩到"一格"的大小（HorizontalPanel 的 unitSize），
	//否则卡片内部部件会按整屏 800x600 来画
	rootCanvas->SetSize(Vector2D(54, 77));
	rootCanvas->SetRelativePosition(Vector2D(0, 0));

	button = AddWidget<Button>();
	button->AttachTo(rootCanvas);
	button->SetLayoutPattern(LayoutPattern::Center);
	button->SetSize(rootCanvas->Getsize());
	button->SetLayer(102);

	black = AddWidget<Image>();
	black->AttachTo(rootCanvas);
	black->SetLayoutPattern(LayoutPattern::Center);
	black->SetSize(rootCanvas->Getsize());
	black->SetLayer(104);
	black->LoadSprite("black");
	black->SetTransprancy(125);

	gray = AddWidget<Image>();
	gray->AttachTo(rootCanvas);
	gray->SetLayoutPattern(LayoutPattern::Center);
	gray->SetSize(rootCanvas->Getsize());
	gray->SetLayer(103);
	gray->LoadSprite("gray");
	gray->SetTransprancy(125);
}

void SeedUI::Init(int num)
{
	number = num;
	button->LoadNormalPicture("card" + std::to_string(number + 1));
	button->SetInfoBox(true);
	button->OnMousePressedBegin.AddDynamic(this, &SeedUI::ReadyToPlant);
}

void SeedUI::Update()
{
	UserInterface::Update();

	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
	{
		if (CoolingTimerHandle.GetDelay() < coolingTime[number])
			button->SetInfoText("$9冷却中\n$4" + plantName[number]);
		else if (pController->GetSunshineNum() < plantPrice[number])
			button->SetInfoText("$9没有足够阳光\n$4" + plantName[number]);
		else
			button->SetInfoText(plantName[number]);


		if (pController->GetSunshineNum() < plantPrice[number] || CoolingTimerHandle.GetDelay() < coolingTime[number])
			gray->SetTransprancy(125);
		else gray->SetTransprancy(0);


		if (CoolingTimerHandle.GetDelay() < coolingTime[number])
		{
			black->SetStartAndEndLoc(Pair(), Pair(black->Getsize().x,
				black->Getsize().y * (1 - CoolingTimerHandle.GetDelay() / coolingTime[number])));
		}
	}
}

void SeedUI::ReadyToPlant()
{
	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
	{
		if (CoolingTimerHandle.GetDelay() < coolingTime[number] || pController->GetSunshineNum() < plantPrice[number])return;

		switch (number)
		{
		case 0: pController->seedToPlant = GameStatics::CreateObject<PeaShooter>(); break;
		case 1: pController->seedToPlant = GameStatics::CreateObject<SunFlower>(); break;
		}

		pController->AddSunshineNum(-plantPrice[number]);
		CoolingTimerHandle.Reset();
	}
}

