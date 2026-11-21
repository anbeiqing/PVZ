#include "BattleLevel.h"
#include"GameStatic.h"
#include"SpriteRenderer.h"
#include"Sprite.h"
#include"sun.h"
#include"Math.h"
#include"PeaShooter.h"

BattleLevel::BattleLevel()
{
	bg0 = GameStatics::CreateObject<StaticMesh>(Vector2D(0, 0));
	bg0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");	


	GameStatics::CreateObject<PeaShooter>(Vector2D(200, 200));
	GameStatics::CreateObject<Sun> (Vector2D(200, 200));
}

void BattleLevel::Update()
{
	;
}
