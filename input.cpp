#include "stdafx.h"
#include "input.h"

Input* Input::SingletonObject = nullptr;

/// <summary>
/// Bitmask �ʱ�ȭ, ���콺 ��ǥ �ʱ�ȭ
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
/// �̱��� ���� �� ��ȯ
/// </summary>
Input* const Input::GetSingletonObject()
{
	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Input();
	}

	return SingletonObject;
}

/// <summary>
/// �̱��� ����
/// </summary>
void Input::ReleaseSingleton()
{
	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// Ű �Է��� Ȯ���ϴ� �Լ�
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
		// Ű�� �����ٰ� ���� Ÿ�ֿ̹� True�� ��ȯ�Ѵ�.
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
		// Ű�� ������ Ÿ�ֿ̹� True�� ��ȯ�Ѵ�.
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
		// Ű�� ������ ������ True�� ��ȯ�Ѵ�.
		return GetAsyncKeyState(key) & 0x8000;
	case KeyState::TOGGLE:
		// Ű�� ���� ������ On/Off �ȴ�.
		return GetKeyState(key) & 0x0001;
	}
}

/// <summary>
/// ���콺 ��ǥ �����ϴ� �Լ�
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
/// ���콺�� RECT�� �浹�Ǿ����� Ȯ��
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