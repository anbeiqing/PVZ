#include "BasePlant.h"



BasePlant::BasePlant()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(2);
	box = ConstructComponent<BoxCollider>();
	box->SetAttachTo(root);
	box->SetSize({50,75});
	box->SetType(CollisionType::Plant);//设置为植物类型，僵尸才能与它产生碰撞
	ani = ConstructComponent<Animator>();
	
}

void BasePlant::GetDamage(float dmg)
{
	blood -= dmg;
	Blink();//受击闪烁
	if (blood <= 0) {
		Destory();
	}
}

void BasePlant::BeginPlay()
{
	Sprite::BeginPlay();
}
