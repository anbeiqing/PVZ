#pragma once
#include"coreMinimal.h"
#include"UserInterface.h"



class BattleLevel :public Level {
	class StaticMesh* bg0;
	 

public:
	BattleLevel();
	virtual void Update()override;
	virtual void Render()override;
	virtual void BeginPlay() override;


};