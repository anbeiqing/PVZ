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

Vector2D BasePlant::GetRenderSize()const
{
	//动画播放时 renderer 的 sprite 会被 Animator 每帧换成"当前帧"图片，尺寸才是真实的一帧；
	
	if (ani) {
		if (IMAGE* frame = ani->GetCurrentImage()) {
			return Vector2D(float(frame->getwidth()), float(frame->getheight()));
		}
	}
	return renderer ? renderer->GetSpriteSize() : Vector2D(0, 0);
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

void BasePlant::Activate()
{
	ani->Activate();
	box->Activate();
}
