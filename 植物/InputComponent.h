#pragma once
#include"coreMinimal.h"
#include<map>
#include<functional>

enum InputType;

struct KeyBindfo
{
	std::function<void()> func;
	InputType type;
	bool isPressed=false;
};



class InputComponent :public Component
{
public:
	void SetMapping(std::string mappingname, int value);

	template<typename T>
	void BindAction(std::string actionName, InputType type, T* obj, void(T::* func)()) {
		if (mappings.find(actionName) != mappings.end())
			callbacks.insert({ actionName,{ std::bind(func,obj),type,false } });
	}

	virtual void Update();
	static Vector2D GetMousePosition();
	static bool IsMouseButtonPressed();

private:
	std::map<std::string, int>mappings;
	std::map<std::string, KeyBindfo> callbacks;
	static Vector2D mousePos;
};

enum InputType
{
	Pressed,
	Released,
	Holding
};

enum KeyCode
{
	VK_LButton = 1,
	VK_RButton = 2,
	VK_MButton = 4,
	VK_WheeledUp = 5,
	VK_WheeledDown = 6,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	
};
