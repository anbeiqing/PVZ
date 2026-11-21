#pragma once

#include"VisualInterface.h"
#include"Timer.h"
class SceneComponent;
class LayerInterface;
class ImageInterface;

//渲染器
class SpriteRenderer final:public SceneComponent,public LayerInterface,public ImageInterface
{
public:
	void Render()override;

};


class StaticMesh :public Object
{
	SpriteRenderer* renderer;
	

public:
	StaticMesh();
	virtual void BeginPlay()override { ; }
};