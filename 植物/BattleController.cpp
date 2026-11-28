#include "BattleController.h"
#include"Camera.h"
#include"sun.h"
#include"GameStatic.h"
#include"Math.h"
#include"BattleUI.h"
BattleController::BattleController()
{
	BattleUI* ui = GameStatics::CreateUI<BattleUI>();
	ui->AddToViewport();

	SunshineTimerHandle.Bind(12, this, &BattleController::ProduceSun, true, 7.5);
}

void BattleController::SetupInputComponent(InputComponent* inputComponent)
{
	Controller::SetupInputComponent(inputComponent);

	inputComponent->SetMapping("Plant", KeyCode::VK_LButton);
	inputComponent->BindAction("Plant", InputType::Pressed, this, &BattleController::PlantSeed);

	inputComponent->SetMapping("BackToMenu", KeyCode::VK_Escape);
	inputComponent->BindAction("BackToMenu", InputType::Pressed, this, &BattleController::BackToMenu);
}

void BattleController::Update()
{
	
	Controller::Update();
	if (OverallTimeHandle.GetDelay()>2.f && OverallTimeHandle.GetDelay() <= 5.f && GetWorldPosition().x<260) {
		AddLocalPosition(Vector2D(3.f, 0));

	}
	if(OverallTimeHandle.GetDelay()>7.f&&GetWorldPosition().x>=100.f){
		AddLocalPosition(Vector2D(-3.f, 0));
	}

	if (seedToPlant) {
		seedToPlant->box->Deactivate();
		//本工程里 屏幕位置 = 对象位置 × 2 - 摄像机位置
		//（渲染器、碰撞盒都挂在 root 下，SceneComponent::GetWorldPosition 会把对象位置叠加两遍，
		//  这是既有约定，渲染与碰撞是一致的，不要在这里"修"掉）
		//所以直接把鼠标坐标写进对象位置，画出来的植物会跑到鼠标的两倍远处。
		//反解：让贴图中心落在鼠标下，对象位置 = (鼠标 + 摄像机 - 贴图尺寸/2) / 2
		Vector2D mouse = GetCursorPosition();
		Vector2D cam = camera ? camera->GetCameraPosition() : Vector2D(0, 0);
		Vector2D half = seedToPlant->GetRenderSize() * 0.5f;
		seedToPlant->SetLocalPosition(Vector2D((mouse.x + cam.x - half.x) * 0.5f,
			(mouse.y + cam.y - half.y) * 0.5f));
	}
}

void BattleController::Shake()
{
	/*camera->ShakeCamera(10,1);*/
}

void BattleController::MoveRight()
{
	AddLocalPosition(Vector2D(100.f, 0));
}


void BattleController::PlantSeed()
{
	if (seedToPlant)
	{
		seedToPlant->Activate();
		sunshineNum -= seedToPlant->GetCost();
		seedToPlant = nullptr;
	}
}

void BattleController::BackToMenu()
{
	GameStatics::OpenLevel("Menu");
}

void BattleController::ProduceSun()
{
	Sun* sunshine = GameStatics::CreateObject<Sun>(Vector2D(Math::RandInt(100, 700), 0));
	if (sunshine)sunshine->Fall(Math::RandInt(250, 550));
}

void BattleController::ProduceZombie()
{
	;
}

