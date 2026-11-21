#include "GameStatic.h"

GameInstance* GameStatics::GetGameInstance()
{
    return mainWorld.gameInstance;
}

Controller* GameStatics::GetController()
{
    return mainWorld.mainController;
}

void GameStatics::OpenLevel(std::string levelName)
{
    ;
}
