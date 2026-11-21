#include "Camera.h"
#include"Math.h"

Vector2D Camera::Lerp(Vector2D start, Vector2D end, float t)
{

	t = Math::Clamp(t, 0.001f, 0.1f);
	return start + (end - start) * t;
}

float Camera::Lerp(float start, float end, float t)
{
	t = Math::Clamp(t, 0.001f, 0.1f);
	return start + (end - start) * t;
}

Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::Update()
{
	
}

void Camera::SetMainCamera()
{
	mainWorld.mainCamera = this;
}


void Camera::Calculate()
{
	static bool first = true;
	if (first) {
		transform_virtual.position = GetWorldPosition();
		transform_virtual.rotation = GetWorldRotation();
		springArmLength_virtual = springArmLength;
		first = false;
	}
	if (smoothness) {
		transform_virtual.position = Lerp(transform_virtual.position, GetWorldPosition(),
			0.1/smoothness* smoothstep(Vector2D::Distance(transform_virtual.position, GetWorldPosition()) / distanceTreshold));
	}
	else transform_virtual.position = GetWorldPosition();
	if (smoothnessForSpringArm) {
		springArmLength_virtual = Lerp(springArmLength_virtual, springArmLength, 0.1f / smoothnessForSpringArm);
	}
	else springArmLength_virtual = springArmLength;
	if (shakeFlag) {
		if (GetLocalPosition() == shakecenter) {
			shakeSpeed.x = float(Math::RandInt(-1, 1));
			shakeSpeed.y = float(Math::RandInt(-1, 1));
			Intensity -= float(shakeDecay)/100;
			if (Intensity <= 0) {
				shakeFlag = false;
				return;

			}

		}
		if (Vector2D::Distance(GetLocalPosition(), shakecenter) < Intensity) {
			AddLocalPosition(shakeSpeed);
			transform_virtual.position += shakeSpeed;
		}
		else {
			shakeSpeed = -shakeSpeed;
			AddLocalPosition(shakeSpeed);
			transform_virtual.position += shakeSpeed;
		}
		
	}
	
}

void Camera::SetSmoothness(short s)
{
	this->smoothness = Math::Clamp(s, 0.0, 100.0);
}

void Camera::ShakeCamera(char intensity,int decay)
{
	intensity = Math::Clamp(intensity, 0.0, 100.0);
	decay = Math::Clamp(decay, 1, 100);
	Intensity = (float)intensity;
	shakeDecay = decay;
	if (!shakeFlag)shakecenter = GetLocalPosition();
	shakeFlag = true;

	
}

void Camera::SetDistanceTreshold(float d)
{
	this->distanceTreshold = Math::Clamp(d, 0.0, 500.0);
}
