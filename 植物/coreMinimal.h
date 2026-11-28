




#pragma once
//头文件
#include<iostream>
#include<set>
#include<unordered_set>
#include<stack>
#include<cmath>
#include<string>




//宏定义
#define PI 3.1415926
#define DELTA_TIME 0.01
#define WIN_WIDTH 800
#define WIN_HEIGHT 600


typedef unsigned long long uint64;
typedef unsigned int uint32;




;
//二维向量
struct Vector2D
{
	float x;
	float y;
	Vector2D() :x(0), y(0) {}
	Vector2D(float a, float b) :x(a),y(b){}

	float Size() const
	{
		return sqrtf(x * x + y * y);
	}
	//获取单位向量
	Vector2D Normalize() const
	{
		return Vector2D(x / Size(), y / Size());
	}
	static float Distance(const Vector2D& a, const Vector2D& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
	//旋转
	Vector2D RotateVector(float angle, const Vector2D& vec) {
		double rad = PI * angle / 180;
		float fsin = float(sin(rad));
		float fcos = float(cos(rad));
		return Vector2D(vec.x * fcos - vec.y * fsin, vec.x * fsin + vec.y * fcos);
	}
	//向量点积
	static float DotProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	//向量叉积
	static Vector2D CrossProduct(const Vector2D& v1, const Vector2D& v2)
	{
		return { v1.y * v2.x - v1.x * v2.y, v1.x * v2.y - v1.y * v2.x };
	}
	// 将向量 u 投影到向量 v 所在的直线上
	static Vector2D ProjectVector(const Vector2D& u, const Vector2D& v)
	{
		float scalar = DotProduct(u, v) / v.Size();  // 计算投影向量的缩放因子
		return v.Normalize() * scalar;
	}

	Vector2D operator+(const Vector2D& another) {
		return Vector2D(x + another.x, y + another.y);

	}
	Vector2D operator-(const Vector2D& another) {
		return Vector2D(x - another.x, y - another.y);

	}
	Vector2D operator+=(const Vector2D& another) {
		x += another.x;
		y += another.y;
		return *this;

	}
	Vector2D operator-=(const Vector2D& another) {
		x -= another.x;
		y -= another.y;
		return *this;

	}
	bool operator==(const Vector2D &another)const {
		return x == another.x && y == another.y;
	}
	bool operator!=(const Vector2D& another)const {
		return x != another.x || y != another.y;
	}
	Vector2D operator*(float t)const {
		return Vector2D(x * t, y * t);
	}
	Vector2D& operator*=(float t) {
		x *= t;
		y *= t;
		return *this;
	}
	Vector2D operator/(float t)const {
		return Vector2D(x /t, y / t);
	}
	Vector2D& operator/=(float t) {
		x /= t;
		y /= t;
		return *this;
	}
	Vector2D operator- () const {
		return Vector2D(-x, -y);

	}
	float operator*(const Vector2D& another)const {
		return x * another.x + y * another.y;
	}

};
//添加打印
static std::ostream& operator<<(std::ostream&out,const Vector2D& vec) {
	std::cout << "(" << vec.x << "," << vec.y << ")";
	return out;

}
//数对
struct Pair
{
	int x, y;
	Pair(int x,int y):x(x),y(y){}
	Pair():x(0),y(0){}
	bool operator==(const Pair ano)
	{
		return x == ano.x && y == ano.y;
	}
	bool operator!=(const Pair ano)
	{
		return x != ano.x || y != ano.y;
	}
};




//变换
struct Transform
{
	Vector2D position;
	float rotation;
	Vector2D scale;//规模
	Transform():position(0,0),rotation(0),scale(1,1){}
	Transform(const Vector2D& pos,float a,const Vector2D& sc):position(pos),rotation(a),scale(sc){}
	Transform Idenity()const {
		return Transform(Vector2D(0, 0), 0, Vector2D(1, 1));
	}

};
enum class CombinePattern :unsigned char { Min, Mid, Max };

/* 物理材质类 */
struct PhysicsMaterial
{
	float friction;
	float bounciness;
	PhysicsMaterial() :friction(0.2f), bounciness(0) {}
	PhysicsMaterial(float friction, float bounciness)
		:friction(friction), bounciness(bounciness) {
	}

	//物理材质结合效果
	static PhysicsMaterial Combine(const PhysicsMaterial& m1, const PhysicsMaterial& m2, CombinePattern pattern = CombinePattern::Mid)
	{
		if (pattern == CombinePattern::Mid)
		{
			return PhysicsMaterial((m1.friction + m2.friction) * 0.5f, (m1.bounciness + m2.bounciness) * 0.5f);
		}
		else
		{
			return pattern == CombinePattern::Min ? PhysicsMaterial(std::min(m1.friction, m2.friction), std::min(m1.bounciness, m2.bounciness))
				: PhysicsMaterial(std::max(m1.friction, m2.friction), std::max(m1.bounciness, m2.bounciness));
		}
	}
};

//基础类
class Base {
public:
	virtual void Update()=0;
};

//组件基类
class Component : public Base {
protected:
	class Object* pOwner=nullptr;//处理控制逻辑

public:
	virtual void Update() override;
	class Object* GetOwner() { return pOwner; }

	//设置绑定对象

	void SetOwner(class Object* owner) {
		pOwner = owner;
	}
	virtual void Destruct();
	
	//虚析构：Destruct() 中 delete this 时的静态类型是 Component*，
	//若析构函数非虚，派生类（Collider/SpriteRenderer/LayerInterface）的析构不会执行，
	//已释放对象仍残留在 GameColliders/Layers 里，下一帧遍历即崩溃
	virtual ~Component() = default;



	//激活组件
	virtual void Activate();

	//停用组件
	virtual void Deactivate();

	bool bIsEnabled = true;//是否激活
};

//场景组件，处理场景属性
class SceneComponent : public Component
{
protected:
	Transform transform;
	std::set<SceneComponent*> children;
	SceneComponent* parent=nullptr;//处理场景属性关系并且便于增删

	void process_Destruct();
	
public:
	virtual void Update() override;

	//设置所属组件
	void SetAttachTo(SceneComponent* par);

	//删除所属组件
	void DetachFrom(SceneComponent* par);
	void AddPosition(int a, int b);
	
	virtual void Destruct() override;

	//获取相对坐标
	Vector2D GetLocalPosition()const {
		return transform.position;
	}
	float GetLocalRotation()const {
		return transform.rotation;
	}
	Vector2D GetLocalScale()const {
		return transform.scale;
	}


	//获取世界坐标
	Vector2D GetWorldPosition()const;
	float GetWorldRotation()const;
	Vector2D GetWorldScale()const;
	
	//设置相对坐标
	void SetLocalPosition(const Vector2D& pos) {
		transform.position = pos;
	}
	void SetLocalRotation(float angle) {
		transform.rotation = angle;
	}
	void SetLocalScale(const Vector2D& sc) {
		transform.scale = sc;
	}



	void AddLocalPosition(const Vector2D& pos) {
		transform.position += pos;
	}
	void AddLocalRotation(float angle) {
		transform.rotation += angle;
	}


};



//对象类，处理对象关系和组件关系
class Object : public Base {
	
	std::string name;
	std::set<Object*>children;
	Object* parent = nullptr;//处理对象关系并且便于增删


	std::set<Component*>components;
	std::set<Component*>::iterator components_iter;//管理组件的遍历以及增删


protected:

	SceneComponent* const root=new SceneComponent;//场景根组件，赋予场景属性
	

public:

	//创建组件，绑定到对象，并注册到对象容器
	template<typename T>
	inline T* ConstructComponent()
	{
		T* pCom = new T;
		if (pCom && static_cast<Component*>(pCom)) {
			pCom->SetOwner(this);
			RegisterComponent(pCom);
			return pCom;
		}
		return nullptr;

	}
	std::string GetName()const { return name; }
	//设置标签名
	void SetName(const std::string& name) { this->name = name; }
	Object() {
		root->SetOwner(this);
	}

	virtual~Object() {
		
		//不能 range-for：Destruct() 内部会 erase 当前节点，使迭代器失效
		while (!components.empty()) {
			(*components.begin())->Destruct();
		}
		delete root;
	}


	virtual void Update()override {
		components_iter = components.begin();
		while (components_iter != components.end()) {
			(*components_iter)->Update();
			if (components_iter == components.end())break;
			components_iter++;
		}
	}
	virtual void BeginPlay()=0;
	virtual void EndPlay() {}
	


	//设置所属对象
	void AttachTo(Object* par);

	//设置场景根组件
	void SetRootComponent(SceneComponent* newRoot);

	//删除所属
	void DetachFrom(Object* par);
	//将组件注册到对象容器
	void RegisterComponent(Component* pCom) {
		components.insert(pCom);
	}

	//从对象容器中注销组件
	void UnregisterComponent(Component* pCom)
	{
		auto it = components.find(pCom);
		if (it == components.end())return;
		components_iter = components.erase(it);
	}

	//通过类名获取组件
	
	template<class P>
	P* GetComponentByClass() {
		for (auto& Com : components) {
			if (P* pCom = dynamic_cast<P*>(Com))
				return pCom;
		}
		return nullptr;
	}

	//从mainworld中删除对象，销毁对象
	void Destory();

	Vector2D GetLocalPosition()const {return root->GetLocalPosition();};
	Vector2D GetLocalScale()const {return root->GetLocalScale();};
	float GetLocalRotation()const {return root->GetLocalRotation();};


	Vector2D GetWorldPosition()const ;
	Vector2D GetWorldScale()const ;
	float GetWorldRotation()const ;



	void SetLocalPosition(const Vector2D& pos) { root->SetLocalPosition(pos); };
	void SetLocalScale(const Vector2D& sc) { root->SetLocalScale(sc); };
	void SetLocalRotation(float angle) { root->SetLocalRotation(angle); };

	void AddLocalPosition(const Vector2D& pos) { root->AddLocalPosition(pos); };
	void AddLocalRotation(float angle) { root->AddLocalRotation(angle); };
};

//全局游戏单例
class GameInstance {



};

//游戏关卡，处理关卡逻辑
class Level : public Base {
public:
	virtual void Update() override;
	//关卡自己的渲染钩子：World::Render 渲染完所有场景图层后调用，用来画 HUD 等屏幕空间内容
	virtual void Render();
	
};



//渲染图层排序
struct LayerSort {
	bool operator()(const class LayerInterface* a, const class LayerInterface* b)const;
};
//碰撞图层排序规则
struct ColliderSort {
	bool operator()(const class Collider* a, const class Collider* b)const;
 };



//游戏世界，处理游戏对象，UI，计时器，渲染，碰撞
class World final {

	//友缘声明
	friend class Timer;
	friend class Engine;
	friend class GameStatics;
	friend class LayerInterface;
	friend void Object::Destory();
	friend class ImageInterface;
	friend class Camera;
	friend class SpriteRenderer;
	friend class ResourceManager;
	friend class Animation;
	friend class Button;
	friend class Bar;


	friend class Collider;
	friend class CircleCollider;
	friend class BoxCollider;
	friend class Controller;
	friend class UserInterface;
	friend class Widget;

	//资源
	ResourceManager* resourcePool = nullptr;

	//计时器
	Timer* FPSClock;
	//碰撞管理器
	class CollisionManager* collisionManager = nullptr;


	//场景对象，UI，计时器
	std::set<Object*>GameObjects;
	std::set<Object*>GameObjects_to_delete;
	std::set<class UserInterface*>UserInterfaces;
	std::set<class UserInterface*>UserInterfaces_to_delete;
	std::set<Timer*>Timers;


	//渲染，碰撞
	std::set<LayerInterface*,LayerSort>Layers;
	std::unordered_set<class Collider*>GameColliders;
	std::unordered_set<class Collider*>GameColliders_to_clear;
	std::set<class Collider*, ColliderSort>ColliderZones[8][6];
	std::set<LayerInterface*, LayerSort>UIdetectZones[8][6];
	std::set<class BoxCollider*>Colliders;

	//游戏单例
	
	Level* currentLevel = nullptr;
	class Controller* mainController = nullptr;
    Camera* mainCamera =nullptr;
	GameInstance* gameInstance = nullptr;


	


	//核心逻辑

	void Update();

	void Render();

	void Input();

	void Update_();

	//帧率
	void Debug();

};
extern World mainWorld;
//获取一个随机整数


//类型转换
template<class T>
inline T* Cast(Base* b){
	if(b)return dynamic_cast<T*>(b);	
	return nullptr;
}
template<class T>
inline T* Cast(LayerInterface* layer) {
	if (layer == nullptr)
		return nullptr;
	// 先把接口指针转成它背后的Base*，再转目标类型
	Base* b = dynamic_cast<Base*>(layer);
	if (b) return dynamic_cast<T*>(b);
	return nullptr;
}