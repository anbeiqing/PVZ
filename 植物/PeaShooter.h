#pragma once
#include"BasePlant.h"

//豌豆射手
class PeaShooter :public BasePlant
{
	AnimationDelegate shootEvent;
	Animation idle;
public:
	PeaShooter();
	virtual void BeginPlay()override;


	void Shoot();
};