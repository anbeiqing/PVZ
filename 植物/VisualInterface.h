#pragma once
#include"coreMinimal.h"
#include<easyx.h>
#include<string>


class LayerInterface{
	int layer = 0;
	
public:
	bool bDestroy = false;
	LayerInterface();
	virtual ~LayerInterface();
	void SetLayer(int layer);//图层
	int GetLayer()const;

	virtual void Render()=0;
};

struct SpriteInfo
{
	Vector2D offset = Vector2D(0, 0);
	Pair size = Pair(0, 0);
	Pair startLoc = Pair(0, 0);
	Pair endLoc = Pair(0, 0);
};



class ImageInterface {
	friend class Animator;
protected:
	IMAGE* sprite = nullptr;
	SpriteInfo spriteInfo;
	BYTE alpha = 255;//透明度
	float angle = 0;
	void RotateImage(double radian);
	IMAGE* copy = nullptr;

	struct FilterInfo {
		COLORREF color = BLACK;//滤镜颜色
		int level = 5;//颜色过渡层级
		IMAGE* filter = nullptr;

	}filterInfo;
	bool bFilterEnabled = false;
public:
	BYTE GetTransprancy()const {
		return alpha;
	}
	void SetTransprancy(BYTE tr) {
		alpha = tr;
	}
	//当前贴图的像素尺寸（动画每帧可能不同，所以每帧取，不要缓存）
	Vector2D GetSpriteSize()const {
		return sprite ? Vector2D(float(sprite->getwidth()), float(sprite->getheight())) : Vector2D(0, 0);
	}
	//加载静态图片
	void LoadSprite(std::string name);
	/* 推荐设置非动画资源 */
	void SetStartAndEndLoc(Pair start, Pair end);
};