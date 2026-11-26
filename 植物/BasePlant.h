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
	void Activate();
	int GetCost()const { return cost; }
	//当前这一帧的贴图尺寸（跟随光标摆放植物时用来对齐中心点）
	Vector2D GetRenderSize()const;
};

