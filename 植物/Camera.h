#pragma once
#include"coreMinimal.h"
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

class Camera : public SceneComponent {
	friend class SpriteRenderer;
	friend class CircleCollider;
	friend class BoxCollider;

	Transform transform_virtual;//虚拟相机

	float distanceTreshold = 100.0f;//距离阈值，超过这个距离才会移动摄像机
	short smoothness = 30;//平滑度，越大越平滑


	char Intensity = 0.0f;//震动强度
	bool shakeFlag = false;//震动标志
	Vector2D shakecenter = Vector2D(0, 0);//震动中心
	Vector2D shakeSpeed = Vector2D(0.05f, 0.05f);//震动速度
	int shakeDecay = 5;//震动衰减速度

	float springArmLength = 40.f;//弹簧长度
	float springArmLength_virtual = 10.f;//虚拟弹簧长度
	float smoothnessForSpringArm=20.f;



	Vector2D Lerp(Vector2D start, Vector2D end, float t);//线性插值
	float Lerp(float start, float end, float t);

	//平滑处理
	float smoothstep(float x) {

		x = x < 1 ? 1:x;
		return pow(x,float(smoothness)/100);
	}


public:
	Camera();
	~Camera();
	virtual void Update() override;

	void SetMainCamera();
	Vector2D GetCameraPosition()const {
		return transform_virtual.position;
	}
	void Calculate();
	void SetSmoothness(short s);
	void ShakeCamera(char intensity,int decay=10);
	
	void SetDistanceTreshold(float d);
	void SetspringArmLength(float length) {
		springArmLength = length;
	}
	void SetsmoothnessForSpringArm(float smooth) {
		smoothnessForSpringArm = smooth;
	}
};
