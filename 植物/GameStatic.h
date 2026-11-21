#pragma once
#include"coreMinimal.h"
#include<vector>
class GameStatics final {
public:
	static GameInstance* GetGameInstance();
	static Controller* GetController();
	template<typename T>
	static T* CreateObject(Vector2D pos = Vector2D(0, 0));
	template<typename T>
	static std::vector<T*> FindObjectsOfClass();
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
