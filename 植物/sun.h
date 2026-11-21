#pragma once
#include"SpriteRenderer.h"
#include"Animator.h"
#include"Collider.h"
#include"RigidBody.h"


class Sun :public StaticMesh
{
	Animation ani;
	Animator* animator;
	CircleCollider* collider;
	RigidBody* rigidbody;
	

public:
	Sun() {

		SpriteRenderer* renderer = GetComponentByClass<SpriteRenderer>();

		renderer->SetLayer(10);
		animator = ConstructComponent<Animator>();
		ani.Load("sun");
		ani.SetInterval(0.1f);

		collider = ConstructComponent<CircleCollider>();
		collider->SetAttachTo(root);
		collider->SetRadius(35);
		collider->SetCollisionMode(CollisionMode::Collision);
		collider->SetType("sun");

		


		animator->Insert("idle", ani);
		animator->SetNode("idle");

		rigidbody = ConstructComponent<RigidBody>();


	

	}
	virtual void Update()override {
		StaticMesh::Update();
		
	}

	void BeginOverlap(Collider* OverlapCom, Object* OverlapActor) {
		;
	}
	void EndOverlap(Collider* OverlapCom, Object* OverlapActor) {
		;
	}

};