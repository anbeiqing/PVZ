#pragma once
#include"Sprite.h"
#include"Animator.h"
#include"Collider.h"
#include"BasePlant.h"
#include"Timer.h"


class BaseZombie :public Sprite
{
	enum class ZombieState
	{
		Walk,Standing
	}state = ZombieState::Walk;//必须初始化，否则移动/吃植物判断读到垃圾值
	Timer eatTimerHandle;
protected:
	AnimationDelegate dieEvent;
	SpriteRenderer* renderer;
	BoxCollider* box;
	Animator* ani;

	Animation walk;
	Animation eat;
	Animation die;

	float speed =0.08f;
	float blood = 10;
	float damage=1;

	std::unordered_set<Object*>overlapPlants;
	BasePlant* eatingPlant;
public:
	BaseZombie();
	virtual void BeginPlay() override;
	void Update();

	//受到伤害：扣血、闪烁，血量归零后销毁
	void GetDamage(float dmg);

	virtual void OnOverlap(Collider* overlapComp, Collider* otherComp, Object* otherActor);
	virtual void EndOverlap(Collider* overlapComp, Collider* otherComp, Object* otherActor);
};
