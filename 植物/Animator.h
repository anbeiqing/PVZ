#pragma once
#include"coreMinimal.h"
#include<easyx.h>
#include"Timer.h"
#include"Delegate.h"
#include<unordered_map>



//动画委托
DECLARE_NO_PARAM_UNICAST_DELEGATE_CLASS(AnimationDelegate)
//动画资源
class Animation
{
friend class Animator;
	int num = 0;//动画帧数
	Vector2D offset = { 0,0 }; //偏移
	int index = 0;//当前帧索引


	IMAGE* anis = nullptr;//第二种动画资源
	IMAGE** images = nullptr;//动画帧图片数组
	Timer clock;//计时器
	std::unordered_map<int, AnimationDelegate>notifications;//自定义动画通知


	//更新当前帧索引
	void UpdateIndex() {
		index=(index+1) % num;
		
	}
public:
	Animation() {
		clock.Bind(0, this, &Animation::Tick,true);
		clock.Stop();
	}
	void Load(std::string name, Vector2D delta = Vector2D(0, 0));

	void Tick();
	//间隔时间
	void SetInterval(double ms) {
		clock.SetDelay(ms);
	}
	void SetIndex(int i) {
		index = i % num;
	}
	//在指定帧处添加动画通知
	void AddNotification(int index, AnimationDelegate& event) { notifications.insert({ index,event }); }

 
};

//动画控制器
class Animator : public Component
{
	std::unordered_map<std::string, Animation&> animations;//动画资源池
	Animation* aniNode = nullptr;//当前动画节点
	bool isCalled = true;

	class SpriteRenderer* rendererAttached = nullptr;//附着渲染
public:
	virtual void Update()override;
	void Insert(std::string name, Animation& ani);

	Animation* GetNode()const { return aniNode; }
	//当前动画帧的图片（还没设过动画节点时返回空）
	//注意：没播放动画时 renderer 的 sprite 是整张图集，尺寸不是一帧的尺寸
	IMAGE* GetCurrentImage()const { return aniNode ? aniNode->images[aniNode->index] : nullptr; }
	void SetNode(std::string nodeName);
	void SetCalled(bool called) { isCalled = called; }

};