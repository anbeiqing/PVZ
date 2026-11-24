#include "BaseZombie.h"

BaseZombie::BaseZombie()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(2);
	box = ConstructComponent<BoxCollider>();
	box->SetAttachTo(root);
	box->SetSize({ 95,140 });
	box->SetType(CollisionType::Zombie);//不设置类型的话碰撞映射里永远匹配不到 Bullet-Zombie
	ani = ConstructComponent<Animator>();
}

void BaseZombie::Update()
{
	Object::Update();
	AddLocalPosition({ -speed,0 });
}

void BaseZombie::GetDamage(float dmg)
{
	blood -= dmg;
	Blink();//受击闪烁；Sprite::Blink 内部有存活标记，对象先销毁也不会回调到野指针
	if (blood <= 0)Destory();
}
