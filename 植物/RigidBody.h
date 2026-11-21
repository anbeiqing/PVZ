#pragma once
#include"coreMinimal.h"


class RigidBody :public Component
{
	Vector2D velocity = Vector2D(0, 0);
	bool bMoveable = true;
	float gravity = 0.098f;
	bool bGraivtyEnabled = true;
	bool bRotatable = true;
	float angulerVeocity = 0;

public:
	virtual void Update()override;
	void SetGraivty(float gravity) { this->gravity = gravity; }
	void SetMoveable(bool bm) { bMoveable = bm; }
	void SetangulerVeocity(float an) { angulerVeocity = an; }
	void AddForce(Vector2D froce) { velocity += froce*DELTA_TIME; }


	void SetGraivtyEnable(bool gv) { bGraivtyEnabled = gv; }
	void SetRotatable(bool ro) { bRotatable = ro; }


};
