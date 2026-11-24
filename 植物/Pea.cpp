#include "Pea.h"

Pea::Pea()
{
	renderer->LoadSprite("pea");
}

void Pea::Update()
{
	Object::Update();//让挂载的组件（碰撞器等）参与更新，否则碰撞体不跟随子弹移动
	AddLocalPosition({1,0});
	if (GetWorldPosition().x > 1500)Destory();
}
