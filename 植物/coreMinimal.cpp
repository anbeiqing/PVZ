#include "coreMinimal.h"
#include"Timer.h"
#include "VisualInterface.h"
#include"Camera.h"
#include"Controller.h"
#include"Collider.h"
#include"SpriteRenderer.h"
#include"Math.h"
#include"UserInterface.h"


 extern World mainWorld{};

void Component::Update()
{
}

void Component::Destruct()
{
	if (pOwner)pOwner->UnregisterComponent(this);
	delete this;//派生类析构会执行（~Component 为虚），因此 ~Collider/~LayerInterface 等会正常注销
}

void SceneComponent::process_Destruct()
{
	//子组件必须全部递归销毁（原来的实现 children 为空时直接返回，叶子组件既不析构也不注销，纯泄漏）
	//child->Destruct() 会把自己从 parent->children 中移除，循环条件因此自然结束
	while (!children.empty())
	{
		SceneComponent* child = *children.begin();
		child->Destruct();
	}
	Component::Destruct();
}

void SceneComponent::Update()
{
}


void SceneComponent::SetAttachTo(SceneComponent* par)
{
	if (par) {
		par->children.insert(this);
		parent = par;
		SetOwner(par->pOwner);
	}
}

void SceneComponent::DetachFrom(SceneComponent* par)
{
	if (par) {
		par->children.erase(this);
		parent = nullptr;
		SetOwner(nullptr);
	}
}

void SceneComponent::Destruct()
{
	if (parent)parent->children.erase(this);
	process_Destruct();
}

Vector2D SceneComponent::GetWorldPosition() const
{
	if (parent)
		return parent->GetWorldPosition() + transform.position;
	else {
		if (pOwner)return pOwner->GetWorldPosition() + transform.position;
		else return transform.position;
	}

}

float SceneComponent::GetWorldRotation() const
{
	if (parent)
		return parent->GetWorldRotation() + GetLocalRotation();
	else {
		if (pOwner)return pOwner->GetWorldRotation();
		else return GetLocalRotation();
	}
	
}

Vector2D SceneComponent::GetWorldScale() const
{
	if (parent)
		return parent->GetWorldScale() + GetLocalScale();
	else {
		if (pOwner)return pOwner->GetWorldScale();
		else return GetLocalScale();
	}
}



Vector2D Object::GetWorldPosition() const
{
	if (parent)
		return parent->GetWorldPosition() + GetLocalPosition();
	else {
		return GetLocalPosition();
	}
}
Vector2D Object::GetWorldScale() const
{
	if(parent)
		return parent->GetWorldScale() + GetLocalScale();
	else {
		return GetLocalScale();
	}
}
float Object::GetWorldRotation() const
{
	if(parent)
		return parent->GetWorldRotation() + GetLocalRotation();
	else {
		return GetLocalRotation();
	}	
}





void Object::AttachTo(Object* par)
{
	if (par) {
		par->children.insert(this);
		parent = par;
	}
}

void Object::SetRootComponent(SceneComponent* newRoot)
{
	const_cast<SceneComponent*&>(root) = newRoot;
}

void Object::DetachFrom(Object* par)
{
	if (par) {
		par->children.erase(this);
		parent = nullptr;
	}
}

void Object::Destory()
{
	if(parent)
		parent->children.erase(this);
	std::stack<Object*> objStack_to_delete;
	objStack_to_delete.push(this);
	while(!objStack_to_delete.empty()){
		Object* current_object = objStack_to_delete.top();
		objStack_to_delete.pop();
		if (!current_object->children.empty()) {
			for (auto& child : current_object->children) {
				objStack_to_delete.push(child);
			}
		}
		mainWorld.GameObjects_to_delete.insert(current_object);
	}
}

void World::Update()
{
	//碰撞信息
	for (auto& arr_i : ColliderZones) {
		for (auto& arr_j : arr_i) {
			arr_j.clear();
		}
	}
	for (auto it :GameColliders) {
		
		if (it->GetCollisionMode() == CollisionMode::None)continue;
		Vector2D half;
		if (it->GetShape() == Collider::Circle) {
			float a = Cast<CircleCollider>(it)->GetRadius();
			half = {a,a};
		}
		else {
			half = Cast<BoxCollider>(it)->GetSize() / 2;
		}
		Vector2D pos = (it)->GetWorldPosition();
		pos -= half;
		int x = int(pos.x) / 100;x = Math::Clamp(x, 0,7);
		int y = int(pos.y) / 100;y= Math::Clamp(y, 0,5);
		pos += half * 2;
		int x_1 = int(pos.x) / 100;x_1 = Math::Clamp(x_1, 0,7);
		int y_1 = int(pos.y) / 100;y_1 = Math::Clamp(y_1, 0,5);
		for (int i = x;i < x_1;++i) {
			for (int j = y;j < y_1;++j) {
				ColliderZones[i][j].insert(it);
			}

		}
	}
	for (auto& arr_i : ColliderZones) {
		for (auto& arr_j : arr_i) {
			if (!arr_j.empty()) {
				for (auto me = arr_j.begin();me != arr_j.end();++me) {
					for (auto he = arr_j.begin();he != arr_j.end();++he) {
						if (he != me)(*me)->Insert(*he);
					}
				}
			}
		}
	}
	//碰撞更新
	for (auto& it : GameColliders)it->Erase();





	for (auto& obj : GameObjects) {
		obj->Update();
	}
	for (auto& obj : GameObjects_to_delete) {
		GameObjects.erase(obj);
		delete obj;
	}
	GameObjects_to_delete.clear();
	for (auto& ui : UserInterfaces) {
		ui->Update();
		
	}
	for (auto& ui : UserInterfaces_to_delete) {
		UserInterfaces.erase(ui);
		delete ui;
	}
	UserInterfaces_to_delete.clear();
	//遍历副本：定时器回调可能销毁（注销）其他定时器，直接遍历 Timers 会让迭代器失效
	std::set<Timer*> timers_snapshot = Timers;
	for (auto& timer : timers_snapshot) {
		if (Timers.find(timer) == Timers.end())continue;//回调里已被销毁
		timer->Execute();
	}
}

void World::Render()
{
	cleardevice();
	for (auto& layer : Layers) {
		if (layer == nullptr)continue;
		layer->Render();
	}
	for (auto& obj : UserInterfaces) {
		obj->ShowInfoBoxs();
	}

	Debug();


	FlushBatchDraw();

	
}

void World::Input()
{
	mainController->PickInfo();
}

void World::Update_()
{
	mainCamera->Calculate();
}

void World::Debug()
{
	for (auto& obj : GameColliders) {
		obj->DrawDebugLine();
	}//碰撞显示
	static int FPS = 0;
	static int number = 0;
	number++;
	setbkmode(TRANSPARENT);
	outtextxy(0, 0,(std::string("FPS:") + std::to_string(FPS)).c_str());
	if (FPSClock->GetDelay() >= 1.0) {
		FPS = number;
		FPSClock->Reset();
		number = 0;
	}

}



void Level::Update()
{
	
}

bool LayerSort::operator()(const LayerInterface* a, const LayerInterface* b) const
{
	if (a->GetLayer() == b->GetLayer())
		return a<b;
	else
		return a->GetLayer() < b->GetLayer();
}

bool ColliderSort::operator()(const Collider* a, const Collider* b) const
{
	if (a->GetLayer() == b->GetLayer())
		return a < b;
	else
		return a->GetLayer() < b->GetLayer();
}
