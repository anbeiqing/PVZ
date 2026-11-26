#pragma once

#include"Sprite.h"
#include"Collider.h"
#include"Animator.h"

//植物基类
class BasePlant :public Sprite
{
protected:
	SpriteRenderer* renderer;
	BoxCollider* box;
	Animator* ani;

	float blood = 10;
	int cost = 0;
public:
	BasePlant();
	void GetDamage(float dmg);
	virtual void BeginPlay()override;
};

