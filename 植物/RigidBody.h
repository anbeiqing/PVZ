#pragma once
#include"coreMinimal.h"


class RigidBody :public Component
{
	Vector2D velocity = Vector2D(0, 0);
	float maxSpeed = 1000.0f;
	bool bMoveable = true;
	float gravity = 9.8f;
	bool bGravityEnabled = true;
	float angularVelocity = 0;
	bool bRotatable = true;
	float angulerVeocity = 0;
	float mass = 1.0f;
	float linearDrag = 0.f;
	float angularDrag = 0.f;
	std::unordered_set<Collider*> colliders;
	void RestrictVelocity(Vector2D impactNormal, const PhysicsMaterial& material, RigidBody* another = nullptr);

public:
	virtual void Update()override;
	void SetGraivty(float gravity) { this->gravity = gravity; }
	//设置速度
	void SetVelocity(Vector2D velocity) { this->velocity = velocity; }
	//设置速度最大值
	void SetMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }
	//是否启用重力
	void SetGravityEnabled(bool enabled) { this->bGravityEnabled = enabled; }
	//添加冲量
	void AddPulse(Vector2D pulse) { velocity += pulse / mass; }
	// 是否可移动
	void SetMoveable(bool moveable) { this->bMoveable = moveable;if (!moveable)velocity = Vector2D(0, 0); }

	void SetangulerVeocity(float an) { angulerVeocity = an; }

	void AddForce(Vector2D froce) { velocity += froce*DELTA_TIME; }

	//设置线性阻尼
	void SetLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }

	//设置角阻尼
	void SetAngularDrag(float angularDrag) { this->angularDrag = angularDrag; }


	
	//是否可旋转
	void SetRotatable(bool rotatable) { this->bRotatable = rotatable;if (!rotatable)angularVelocity = 0; }


};
