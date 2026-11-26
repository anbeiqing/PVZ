#include "GameStatic.h"
#include"Camera.h"

GameInstance* GameStatics::GetGameInstance()
{
    return mainWorld.gameInstance;
}

Controller* GameStatics::GetController()
{
    return mainWorld.mainController;
}

Vector2D GameStatics::GetCameraPosition()
{
	if (mainWorld.mainCamera)return mainWorld.mainCamera->GetCameraPosition();
	return Vector2D(0, 0);
}

Vector2D GameStatics::WorldToScreen(const Vector2D& worldPos)
{
	Vector2D pos = worldPos;//Vector2D 的 operator- 不是 const 成员，先拷一份
	return pos - GetCameraPosition();
}

void GameStatics::OpenLevel(std::string levelName)
{
    ;
}
