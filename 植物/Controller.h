#pragma once
#include"coreMinimal.h"
#include"InputComponent.h"

class Controller : public Object {


	InputComponent* inputComponent;
protected:
	class Camera* camera ;
	virtual void SetupInputComponent(InputComponent* inputComponent) { ; }
public:
	Controller();
	void PickInfo() {
		inputComponent->Update();
	}
	virtual void BeginPlay()override;
	Vector2D GetCursorPosition()const { return inputComponent->GetMousePosition(); }
	Object* GetObjectUnderCursor();
	bool IsMouseClicked()const { return InputComponent::IsMouseButtonPressed(); }

	struct HitResult GetHitResultUnderCursor();

};

