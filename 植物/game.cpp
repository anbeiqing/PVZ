#include "coreMinimal.h"
#include"Timer.h"
#define byte win_byte

#include<Windows.h>
#include<thread>
#pragma comment(lib,"winmm.lib")

#include"Math.h"
#include"SpriteRenderer.h"
#include"ResourceManager.h"
#include"BattleLevel.h"
#include"Controller.h"
#include"CollisionManager.h"
#include"BattleController.h"
#include"GameStatic.h"




class Engine {
public:
	//初始化
	static void Init() {


		initgraph(800, 600);
		
		

		BeginBatchDraw();

		//初始化资源管理器
		mainWorld.resourcePool = new ResourceManager;
		mainWorld.resourcePool->Initialize();

		//初始化碰撞管理器
		mainWorld.collisionManager = new CollisionManager;
		mainWorld.collisionManager->Initialize();

		//初始化游戏世界
		mainWorld.currentLevel = new BattleLevel;
		mainWorld.gameInstance = new GameInstance;
		mainWorld.mainController = GameStatics::CreateObject<BattleController>();
		mainWorld.FPSClock = new Timer;


	

	}
	static void Tick() {
		mainWorld.Update();
		mainWorld.Render();
		mainWorld.Input();
		
	}
	static void Tick_() {
		mainWorld.Input();
		mainWorld.Update_();
	}



	static void Exit() {
		
	}
	

};



void CALLBACK timecallback(UINT,UINT, DWORD_PTR, DWORD_PTR,DWORD_PTR)
{
    Engine::Tick();
}

void CALLBACK timecallback_(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
	Engine::Tick_();
}


int main() {

	Engine::Init();


	timeSetEvent(UINT(DELTA_TIME*1000), 1, timecallback, NULL, TIME_CALLBACK_FUNCTION|TIME_PERIODIC);
	timeSetEvent(1, 1, timecallback_, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC);

	Sleep(INFINITE);

	return 0;
}