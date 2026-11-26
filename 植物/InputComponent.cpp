#include "InputComponent.h"
#include<Windows.h>
#include<easyx.h>

void InputComponent::SetMapping(std::string mappingname, int value)
{
	mappings.insert({mappingname, value});
}

void InputComponent::Update()
{
	POINT point;
	GetCursorPos(&point);
	//GetCursorPos 得到的是屏幕坐标，而渲染用的是窗口客户区坐标，
	//不转换的话所有鼠标判定都会整体偏移一个"窗口边框+标题栏"的距离
	if (HWND hwnd = GetHWnd())ScreenToClient(hwnd, &point);
	mousePos = Vector2D(float(point.x), float(point.y));
	for (auto& mapping : mappings) {
		KeyBindfo& info = callbacks[mapping.first];
		if (GetAsyncKeyState(mapping.second) & 0x8000)
		{
			if (info.type == Pressed && !info.isPressed)info.func();	
			if (info.type == Holding)info.func();
		    info.isPressed = true;
		}
	    else if(info.isPressed)
		{
			if (info.type == Pressed)info.isPressed = false;
			else if (info.type == Released)
			{
				info.func();
				info.isPressed = false;
			}
		}
	}
}

Vector2D InputComponent::GetMousePosition()
{
	return mousePos;
}

bool InputComponent::IsMouseButtonPressed()
{
	return GetAsyncKeyState(VK_LBUTTON)&0x8000;
}
Vector2D InputComponent::mousePos = {};
