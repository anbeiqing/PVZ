#pragma once
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"


class BaseZombie :public Sprite
{
protected:
	SpriteRenderer* renderer;
	BoxCollider* box;
	Animator* ani;

	Animation walk;
	Animation eat;
	Animation death;

	float speed =0.05f;
	float blood = 10;
public:
	BaseZombie();
	void Update();

	//受到伤害：扣血、闪烁，血量归零后销毁
	void GetDamage(float dmg);

};
