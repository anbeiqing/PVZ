#pragma once
#include"coreMinimal.h"
class MenuLevel final : public Level {
public:
	MenuLevel();
	virtual void Update() override;
	virtual void BeginPlay()override;
	virtual std::string GetLevelName() const override { return "Menu"; }
};