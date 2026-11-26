#pragma once
#include"SpriteRenderer.h"
#include"Animator.h"
#include"Collider.h"
#include"Sprite.h"
#include"RigidBody.h"


class Sun :public Sprite
{
	SpriteRenderer* renderer ;
	Animation ani;
	Animator* animator;
	CircleCollider* collider;
	RigidBody* rigidbody;

	int groundLine;

	Timer DestroyTimerHandle;
	bool bFading = false;
	bool bGrowing = false;
	bool bPicking = false;
	

public:
	Sun();
	virtual void Update()override;

	void Fall(int groundLine);

	void Throw(int groundLine);

	//阳光圆心在窗口客户区上的位置
	Vector2D GetScreenCenter() const;
	//鼠标是否点在阳光上（客户区坐标判定，跟渲染位置一致）
	bool IsMouseOverSun() const;
	

};