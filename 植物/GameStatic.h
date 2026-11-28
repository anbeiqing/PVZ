#pragma once
#include"coreMinimal.h"
#include<vector>
class GameStatics final {
public:
	// 游戏实例
	static GameInstance* GetGameInstance();
	//控制器
	static Controller* GetController();

	template<typename T>
	static T* CreateController();

	//主相机在客户区坐标系下的偏移（没有相机时返回 0,0）
	static Vector2D GetCameraPosition();
	//把世界（渲染）坐标换算成窗口客户区坐标
	static Vector2D WorldToScreen(const Vector2D& worldPos);
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
	template<class T>
	static T* CreateUI();

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
inline T* GameStatics::CreateController()
{
	T* c = CreateObject<T>();
	mainWorld.mainController = c;
	return c;
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
template<class T>
inline T* GameStatics::CreateUI()
{
	T* pUI = new T;
	if (pUI && static_cast<UserInterface*>(pUI))
	{
		mainWorld.UserInterfaces.insert(pUI);
		return pUI;
	}
	delete pUI;
	return nullptr;
}

