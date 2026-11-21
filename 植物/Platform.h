#pragma once
#include"coreMinimal.h"
#include"Collider.h"


class Platform :public Object
{
	BoxCollider* collider;
public:
	Platform()
	{
		collider = ConstructComponent<BoxCollider>();
		collider->SetAttachTo(root);
		collider->SetSize(Vector2D(600, 50));
		//collider->Setc
		collider->SetType("Platform");

	}
	virtual void Update()override
	{
		Object::Update();
	}
	virtual void BeginPlay()override
	{
		;
	}
};