#include "BaseZombie.h"


BaseZombie::BaseZombie()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	renderer->SetLayer(2);
	box = ConstructComponent<BoxCollider>();
	box->SetAttachTo(root);
	box->SetSize({ 95,140 });
	box->SetType(CollisionType::Zombie);//设置类型
	ani = ConstructComponent<Animator>();


	die.Load("normaldeath");
	die.SetInterval(0.16f);
	ani->Insert("die", die);
	

	dieEvent.Bind([this]() {Destory();});
	die.AddNotification(0, dieEvent);//0是从最后一帧切换到第0帧时
}

void BaseZombie::BeginPlay()
{
	Sprite::BeginPlay();
	//添加碰撞事件
	box->OnComponentBeginOverlap.AddDynamic(this, &BaseZombie::OnOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &BaseZombie::EndOverlap);

	eatTimerHandle.Bind(0.5f, [this]() {if (eatingPlant && blood > 0)eatingPlant->GetDamage(damage);},true);
	eatTimerHandle.Stop();
}

void BaseZombie::Update()
{
	Sprite::Update();
	if (blood > 0 && state == ZombieState::Walk)AddLocalPosition({ -speed,0 });
}

void BaseZombie::GetDamage(float dmg)
{
	if (blood <= 0)return;//已进入死亡流程，忽略后续伤害
	blood -= dmg;
	Blink();//受击闪烁
	if (blood <= 0) {
		//死亡：停止啃食，切死亡动画（动画播完由 die 通知 Destory）
		eatTimerHandle.Stop();
		eatingPlant = nullptr;
		state = ZombieState::Standing;
		ani->SetNode("die");
	}
}

void BaseZombie::OnOverlap(Collider* overlapComp, Collider* otherComp, Object* otherActor)
{
	if (otherComp->GetType() != CollisionType::Plant)return;
	if (blood <= 0)return;//死亡过程中不再啃食
	if (otherActor == nullptr)return;

	overlapPlants.insert(otherActor);

	if (state == ZombieState::Walk)
	{
		state = ZombieState::Standing;
		ani->SetNode("eat");
		eatingPlant = Cast<BasePlant>(otherActor);
		eatTimerHandle.Continue();
	}
	
}

void BaseZombie::EndOverlap(Collider* overlapComp, Collider* otherComp, Object* otherActor)
{
	if (otherComp->GetType() != CollisionType::Plant)return;
	if (blood <= 0)return;//死亡过程中不切回行走动画

	overlapPlants.erase(otherActor);

	if (overlapPlants.empty())
	{
		state = ZombieState::Walk;
		ani->SetNode("walk");
		eatingPlant = nullptr;
		eatTimerHandle.Stop();
	}
	else
	{
		eatingPlant = Cast<BasePlant>(*overlapPlants.begin());
	}	
}
