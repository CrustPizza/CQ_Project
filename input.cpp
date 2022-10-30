#include "stdafx.h"
#include "input.h"

Input* Input::SingletonObject = nullptr;

/// <summary>
/// Bitmask 초기화, 마우스 좌표 초기화
/// </summary>
Input::Input()
{
	ZeroMemory(keyDown, sizeof(int) * 8);
	ZeroMemory(keyUp, sizeof(int) * 8);

	cursorXY = {};
	isMouseInWindow = false;
}

Input::~Input()
{
}

/// <summary>
/// 싱글톤 생성 및 반환
/// </summary>
Input* const Input::GetSingletonObject()
{
	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Input();
	}

	return SingletonObject;
}

/// <summary>
/// 싱글톤 해제
/// </summary>
void Input::ReleaseSingleton()
{
	// 싱글톤 객체가 있다면 해제한다.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// 키 입력을 확인하는 함수
/// </summary>
bool Input::KeyCheck(int key, KeyState state)
{
	if (key == VK_LBUTTON || key == VK_RBUTTON)
	{
		if (isMouseInWindow == false)
		{
			return false;
		}
	}
	else if (isWindowActivated == false)
	{
		return false;
	}

	switch (state)
	{
	case KeyState::UP:
		// 키를 눌렀다가 때는 타이밍에 True를 반환한다.
		if ((GetAsyncKeyState(key) & 0x8000) != 0)
		{
			keyUp[key / 32] |= 1 << (key % 32);
		}
		else if ((keyUp[key / 32] & (1 << (key % 32))) != 0)
		{
			keyUp[key / 32] &= ~(1 << (key % 32));
			return true;
		}

		return false;
	case KeyState::DOWN:
		// 키를 누르는 타이밍에 True를 반환한다.
		if ((GetAsyncKeyState(key) & 0x8000) != 0)
		{
			if ((keyDown[key / 32] & (1 << (key % 32))) == 0)
			{
				keyDown[key / 32] |= 1 << (key % 32);
				return true;
			}
		}
		else
		{
			keyDown[key / 32] &= ~(1 << (key % 32));
		}

		return false;
	case KeyState::STAY:
		// 키를 누르고 있으면 True를 반환한다.
		return GetAsyncKeyState(key) & 0x8000;
	case KeyState::TOGGLE:
		// 키를 누를 때마다 On/Off 된다.
		return GetKeyState(key) & 0x0001;
	}
}

/// <summary>
/// 마우스 좌표 설정하는 함수
/// </summary>
void Input::SetMousePos(LPARAM lParam)
{
	cursorXY = { (short)LOWORD(lParam), (short)HIWORD(lParam) };
}

const POINT& Input::GetMousePos()
{
	return cursorXY;
}

/// <summary>
/// 마우스와 RECT가 충돌되었는지 확인
/// </summary>
bool Input::IsMouseOn(const RECT& rc)
{
	if (rc.left <= cursorXY.x && cursorXY.x <= rc.right)
	{
		if (rc.top <= cursorXY.y && cursorXY.y <= rc.bottom)
		{
			return true;
		}
	}

	return false;
}