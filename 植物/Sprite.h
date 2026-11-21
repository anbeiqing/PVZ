#pragma once
#include"coreMinimal.h"
#include"SpriteRenderer.h"
#include"Timer.h"
#include<memory>


class Sprite :public Object
{
	//DEFINE_SUPER(Object);
	SpriteRenderer* renderer;
	bool isBlinking = false;
	bool blikFlag = false;
	float transistionLevel = 0;
	float transistionSpeed = 0;
	//FilterIfo blinkFilter;
	Timer recoverTimerHandle;
	//生命周期标记：定时器回调可能晚于对象销毁，回调执行时先据此判断 this 是否还有效
	std::shared_ptr<bool> bAlive;
	
public:
	Sprite();
	virtual ~Sprite();
	virtual void BeginPlay()override { ; }
	//virtual void Update();
	void LoadSprite(std::string name);
	void Blink(float duration = 0.2f, COLORREF color = BLACK, int level = 60);


};
