#pragma once
#include"coreMinimal.h"
#include<vector>
class GameStatics final {
public:
	// 游戏实例
	static GameInstance* GetGameInstance();
	//控制器
	static Controller* GetController();
	//创建游戏对象
	template<typename T>
	static T* CreateObject(Vector2D pos = Vector2D(0, 0));
	//查找对象
	template<typename T>
	static std::vector<T*> FindObjectsOfClass();

	//通过标签名查找游戏对象
	template<typename T>
	static T* FindObjectOfName(std::string tagName);

	//创建UI对象
	//template<class T>
	//static T* CreateUI();

	//加载地图
	static void OpenLevel(std::string levelName);
};



template<typename T>
inline T* GameStatics::CreateObject(Vector2D pos)
{
	T* pObj = new T;
		if (pObj&&static_cast<Object*>(pObj)) {

			mainWorld.GameObjects.insert(pObj);
			pObj->BeginPlay();
			pObj->SetLocalPosition(pos);
			return pObj;
		}
		return nullptr;
}

template<typename T>
inline std::vector<T*> GameStatics::FindObjectsOfClass()
{
	std::vector<T*> result;
	result.reserve(mainWorld.GameObjects.size());
	for (const auto& obj : mainWorld.GameObjects) {
		if (T* pObj = dynamic_cast<T*>(obj))
			result.push_back(pObj);
	}
	return result;
}

template<typename T>
inline T* GameStatics::FindObjectOfName(std::string tagName)
{
	for (auto& obj : mainWorld.GameObjects)
	{
		if (obj->GetName() == tagName)return obj;
	}
	return nullptr;
}

