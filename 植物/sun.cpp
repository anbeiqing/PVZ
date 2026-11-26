#include "sun.h"
#include"Math.h"
#include"GameStatic.h"
#include"BattleController.h"
#include"InputComponent.h"
Sun::Sun()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(10);
	animator = ConstructComponent<Animator>();
	ani.Load("sun");
	ani.SetInterval(0.1f);

	animator->Insert("idle", ani);
	animator->SetNode("idle");

	collider = ConstructComponent<CircleCollider>();
	collider->SetAttachTo(root);
	collider->SetRadius(35);
	collider->SetCollisionMode(CollisionMode::Collision);

	rigidbody = ConstructComponent<RigidBody>();
	
	rigidbody->SetGraivty(600.f);//如果用9.8则可能弹出窗外


	groundLine = Math::RandInt(100, 500);

	DestroyTimerHandle.Bind(10, [this]() {bFading = true; });
}

//阳光计数器在客户区上的位置（和 BattleLevel::Render 里画的那块底板对齐）
static const float SunCounterX = 70.f;
static const float SunCounterY = 40.f;

static const float PickSpeed = 14.f;//被收集时每帧朝计数器飞的像素数（渲染坐标）
static const float PickArriveDistance = 20.f;//离计数器多近算收集完成
static const int SunValue = 25;//一颗阳光的价值
static const BYTE FadeSpeed = 4;//淡出时每帧减少的透明度

Vector2D Sun::GetScreenCenter() const
{
	float radius = collider->GetRadius();
	//碰撞器的坐标口径是"左上角"，圆心要加回半径
	return GameStatics::WorldToScreen(collider->GetWorldPosition() + Vector2D(radius, radius));
}

bool Sun::IsMouseOverSun() const
{
	//Collider 自带的 IsMouseOver 是拿世界坐标直接和鼠标的客户区坐标比，
	//摄像机一移动就错位，这里两边都换算到客户区坐标再比
	return Vector2D::Distance(InputComponent::GetMousePosition(), GetScreenCenter()) <= collider->GetRadius();
}

void Sun::Update()
{
	Sprite::Update();

	//1 拾取：鼠标左键点到阳光，就进入"被收集"状态
	if (!bPicking && !bFading && InputComponent::IsMouseButtonPressed() && IsMouseOverSun())
	{
		bPicking = true;
		rigidbody->SetGravityEnabled(false);
		rigidbody->SetMoveable(false);
		collider->SetCollisionMode(CollisionMode::None);//捡起来以后不再参与碰撞
	}

	//2 被收集：朝左上角的阳光计数器飞，到达后加分并销毁
	if (bPicking)
	{
		Vector2D target(SunCounterX, SunCounterY);
		Vector2D dir = target - GetScreenCenter();
		float distance = dir.Size();
		if (distance <= PickArriveDistance)
		{
			if (BattleController* pController = Cast<BattleController>(GameStatics::GetController()))
				pController->AddSunshineNum(SunValue);
			Destory();
			return;
		}
		//渲染坐标 = 2 × 对象自身位置（root 组件的坐标会被叠加一次），所以位移要减半
		AddLocalPosition(dir.Normalize() * (PickSpeed * 0.5f));
		return;
	}

	//3 落到地面线以后停在原地
	if (GetWorldPosition().y > groundLine && !bPicking)
	{
		rigidbody->SetGravityEnabled(false);
		rigidbody->SetMoveable(false);
	}

	//4 超时淡出：透明度减到 0 就销毁
	if (bFading)
	{
		BYTE transprancy = renderer->GetTransprancy();
		if (transprancy <= FadeSpeed) { Destory(); return; }
		renderer->SetTransprancy(BYTE(transprancy - FadeSpeed));
	}

	//5 抛出时的"长大"动画
	if (bGrowing)
	{
		Vector2D scale = GetLocalScale() + Vector2D(1, 1) * 0.05f;
		if (scale.x >= 1.f) { bGrowing = false; scale = { 1,1 }; }
		SetLocalScale(scale);
	}
}

void Sun::Fall(int groundLine)
{
	rigidbody->SetGravityEnabled(false);
	this->groundLine = groundLine;
	rigidbody->SetVelocity(Vector2D(0, 50));
	DestroyTimerHandle.SetDelay(15);
}

void Sun::Throw(int groundLine)
{
	rigidbody->AddPulse(Vector2D(Math::RandInt(-40, 50), -125));
	this->groundLine = groundLine;
	bGrowing = true;
	SetLocalScale(Vector2D(1, 1) * 0.1f);
}


