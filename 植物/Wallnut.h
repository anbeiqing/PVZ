#pragma once
#include"BasePlant.h"


class Wallnut :public BasePlant
{
	Animation idle;
	Animation idle1;
	Animation idle2;
public:
	Wallnut();
	virtual void BeginPlay()override;
	virtual void Update()override;

};