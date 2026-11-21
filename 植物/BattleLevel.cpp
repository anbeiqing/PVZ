#include "BattleLevel.h"
#include"GameStatic.h"
#include"SpriteRenderer.h"
#include"sun.h"
#include"Platform.h"
#include"Math.h"

BattleLevel::BattleLevel()
{
	bg0 = GameStatics::CreateObject<StaticMesh>(Vector2D(0, 0));
	bg0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");


	Sun* sun = GameStatics::CreateObject<Sun>();
	sun->SetLocalPosition(Vector2D(200, 200));

	for (int i = 1;i < 50;i++) {
		GameStatics::CreateObject<Sun>(Vector2D(Math::Randreal(-200, 700), Math::Randreal(-10000, 0)));
	}


	GameStatics::CreateObject<Platform>(Vector2D(200, 200));
	

	
}

void BattleLevel::Update()
{
	;
}
