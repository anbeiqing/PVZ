#pragma once
#include"coreMinimal.h"
#include<easyx.h>
#include"Timer.h"
#include<unordered_map>

//动画资源
class Animation
{
friend class Animator;
	int num = 0;//动画帧数
	int index = 0;//当前帧索引


	IMAGE* anis = nullptr;//第二种动画资源
	IMAGE** images = nullptr;//动画帧图片数组
	Timer clock;//计时器


	//更新当前帧索引
	void UpdateIndex() {
		index=(index+1) % num;
		
	}
public:
	Animation() {
		clock.Bind(0, this, &Animation::UpdateIndex,true);
	}
	void Load(std::string name, Vector2D delta = Vector2D(0, 0));

	void SetInterval(double ms) {
		clock.SetDelay(ms);
	}
	void SetIndex(int i) {
		index = i % num;
	}
	

 
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
	void SetNode(std::string nodeName);
	void SetCalled(bool called) { isCalled = called; }

};