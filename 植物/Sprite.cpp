#include "Sprite.h"
#include"Math.h"



Sprite::Sprite()
{
	bAlive = std::make_shared<bool>(true);
	renderer = ConstructComponent<SpriteRenderer>();
	renderer->SetAttachTo(root);
	//不要把 renderer 设为 root：它同时是 components 的成员，
	//~Object 先 delete 组件再 delete root 会造成二次释放
}

//void Sprite::Update()
//{
//	//Super::Update();
//	if (isBlinking)
//	{
//		if (!blikFlag)
//		{
//			if (transistionLevel > 0)transistionLevel -= transistionSpeed;
//			if (transistionLevel <= 0)isBlinking = false;
//		}
//		//renderer->RemoveFilter();
//		//if (isBlinking)renderer->AddFilter({ blinkFilter.color, int(transistionLevel),1 });
//	}
//}

Sprite::~Sprite()
{
	if (bAlive)*bAlive = false;//让仍在排队的定时器回调知道自己已成悬空指针
}

void Sprite::LoadSprite(std::string name)
{
	renderer->LoadSprite(name);
}

void Sprite::Blink(float duration, COLORREF color, int level)
{
	if (!isBlinking)
	{
		duration = Math::Clamp(duration, 0.1f, 1.f);
		blikFlag = true;isBlinking = true;
		//blinkFilter = { color, level,1 };
		transistionLevel = float(level);
		transistionSpeed = 2 * level * DELTA_TIME / duration;
		//不能只捕获裸 this：对象销毁后定时器若已进入队列，回调会访问已释放内存
		std::shared_ptr<bool> alive = bAlive;
		recoverTimerHandle.Bind(duration / 2, [this, alive]() { if (alive && *alive)blikFlag = false; });
	}
}
