#include "BattleLevel.h"
#include"GameStatic.h"
#include"SpriteRenderer.h"
#include"Sprite.h"
#include"sun.h"

#include"Math.h"
#include"PeaShooter.h"
#include"SunFlower.h"
#include"Zombie.h"
#include"BattleController.h"

BattleLevel::BattleLevel()
{
	bg0 = GameStatics::CreateObject<StaticMesh>(Vector2D(0, 0));
	bg0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");


	GameStatics::CreateObject<PeaShooter>(Vector2D(200, 200));
	GameStatics::CreateObject<SunFlower>(Vector2D(200, 100));
	GameStatics::CreateObject<Zombie>(Vector2D(300, 175));
	//GameStatics::CreateObject<Sun> (Vector2D(200, 0));
}

void BattleLevel::Update()
{
	;
}

void BattleLevel::Render()
{
	//左上角的阳光计数板
	int sunshine = 0;
	if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
		sunshine = pController->GetSunshineNum();

	setfillcolor(RGB(255, 247, 213));
	//solidrectangle(10, 24, 130, 56);
	setlinecolor(RGB(120, 90, 40));
	//rectangle(10, 24, 130, 56);

	setbkmode(TRANSPARENT);
	settextcolor(RGB(90, 60, 20));
	settextstyle(20, 0, "微软雅黑");
	//outtextxy(22, 28, (std::string("阳光:") + std::to_string(sunshine)).c_str());
}
