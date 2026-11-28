#pragma once
#include"coreMinimal.h"
#include<unordered_map>
#include<functional>
#include"RigidBody.h"
#include"CollisionManager.h"
#include"Delegate.h"//AddDynamic/RemoveDynamic 宏统一由 Delegate.h 提供



enum class CollisionMode{None,Trigger,Collision};


//碰撞结果
struct HitResult
{
	Vector2D ImpactPoint;
	Vector2D ImpactNormal;
	Object* HitObject;
	Component* HitComponent;

	HitResult():ImpactPoint(0,0),ImpactNormal(0,0),HitObject(nullptr),HitComponent(nullptr){}
	HitResult(const Vector2D& impactPoint,const Vector2D& impactNormal,Object* obj,Component* component)
		:ImpactPoint(impactPoint),ImpactNormal(impactNormal),HitObject(obj),HitComponent(component){ }
};

//碰撞委托
class CollisionDelegate
{
	//回调签名：触发回调的碰撞器、对方碰撞器、对方所属对象
	using Callback = std::function<void(Collider*, Collider*, Object*)>;
	std::vector<Callback>callbacks;
public:
	template<typename T>
	void Add(T* obj, void(T::* func)(Collider*, Collider*, Object*)) {
		callbacks.push_back(std::bind(func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	void Add(Callback func) {
		callbacks.push_back(func);
	}
	template<typename T>
	void Remove(T* obj, void(T::* func)(Collider*, Collider*, Object*))
	{
		Callback targetFunc = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		for (auto it = callbacks.begin();it!= callbacks.end();++it) {
			if (it->target<Callback>() == targetFunc.target<Callback>())
			{
				callbacks.erase(it);break;
			}
		}
	}
	void Remove(Callback func)
	{
		for (auto it = callbacks.begin();it != callbacks.end();++it) {
			if (it->target<Callback>() == func.target<Callback>())
			{
				callbacks.erase(it);break;
			}
		}
	}
	void BroadCast(Collider* OverlapCollider, Collider* OtherCollider, Object* OtherActor);
};

//碰撞
class Collider :public SceneComponent
{
	friend class Controller;
	friend class BigidBody;
	friend class RigidBody;
public:
	virtual void Deactivate()override;
	enum ColliderShape{Circle,Box};
	Collider() { mainWorld.GameColliders.insert(this); }
	virtual ~Collider() { mainWorld.GameColliders.erase(this); Clear(); }
	const std::vector<Object*>& GetCollisions(CollisionType type);

	int GetLayer()const { return Layer; }
	void SetLayer(int layer) { this->Layer = layer; }

	//碰撞类型
	CollisionType GetType()const { return type; }
	void SetType(CollisionType type) { this->type = type; }

	ColliderShape GetShape()const { return shape; }
	void SetCollisionMode(CollisionMode co) { this->mode=co; }
	CollisionMode GetCollisionMode() { return mode; }
	void Clear();
	void Erase();
	void Insert(Collider* another);
	virtual void DrawDebugLine() = 0;

	CollisionDelegate OnComponentBeginOverlap;
	CollisionDelegate OnComponentEndOverlap;
protected:
	PhysicsMaterial material;
	ColliderShape shape = Circle;
	virtual bool CollisionJudge(Collider* anther) = 0;
	virtual bool IsMouseOver() = 0;//鼠标
private:
	CollisionType type = CollisionType::Default;//类型（务必初始化，否则未定义的枚举值会让碰撞映射全部失效）
	int Layer = 0;//优先级

	CollisionMode mode = CollisionMode::Trigger;
	Pair point{ -1, -1 }, point_1{ -1, -1 };
	std::unordered_set<Collider*>collisions;
	std::vector<Object*>aims;
	std::vector<Collider*>collisions_to_rease;


	//碰撞判断
	static bool (*collisionJudgeMap[3])(Collider*, Collider*);
	static bool collisionJudgeCircleToCircle(Collider* c1, Collider* c2);
	static bool collisionJudgeCircleToBox(Collider* c1, Collider* c2);
	static bool collisionJudgeBoxToBox(Collider* c1, Collider* c2);

	HitResult CollisionHit(Collider* another);

	//碰撞信息
	static HitResult(*collisionHitMap[3])(Collider*, Collider*);
	static HitResult collisionHitCircleToCircle(Collider* c1, Collider* c2);
	static HitResult collisionHitCircleToBox(Collider* c1, Collider* c2);
	static HitResult collisionHitBoxToBox(Collider* c1, Collider* c2);

	RigidBody* rigidAttached = nullptr;//附着的刚体

};
//圆形碰撞
class CircleCollider final :public Collider
{
	float radius = 0;
	float radius_ini = 0;
	virtual bool CollisionJudge(Collider* another)override;
public:
	CircleCollider() { shape = Circle; }
	virtual void Update()override;
	virtual void DrawDebugLine()override;
	virtual bool IsMouseOver()override;
	float GetRadius()const { return radius; }
	void SetRadius(float ra) { radius = ra;radius_ini = ra / sqrtf(GetWorldScale().x * GetWorldScale().y); }
};
//矩形碰撞器

class BoxCollider final :public Collider
{
	struct Rect
	{
		float left = 0, top = 0, right = 0, bottom = 0;
	};
	friend class Collider;
	
	Rect rect;
	Vector2D size = Vector2D(0, 0);
	Vector2D size_ini = Vector2D(0, 0);
	virtual bool CollisionJudge(Collider* another)override;
public:
	BoxCollider() { shape = Box; }
	virtual void Update()override;
	Rect GetRect()const { return rect; }
	static Vector2D GetOverlapRect(const Rect& r1, const Rect& r2);
	virtual void DrawDebugLine()override;
	virtual bool IsMouseOver()override;
	Vector2D GetSize()const { return size; }
	void SetSize(Vector2D s) {
		this->size = s;
		size_ini = size / (sqrtf(GetWorldScale().x * GetWorldScale().y));
	}

};