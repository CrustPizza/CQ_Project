#pragma once

enum class KeyState
{
	UP,
	DOWN,
	STAY,
	TOGGLE
};

class Input
{
	// 변수
private:
	// Singleton
	static Input* SingletonObject;

	// 키 Up / Down Bitmask
	int	keyDown[8];
	int	keyUp[8];

	// 마우스 좌표
	POINT	cursorXY;

public:
	// 윈도우 활성화 관련 Input 거름망
	bool isMouseInWindow;
	bool isWindowActivated;

	// 함수
public:
	Input();
	~Input();

	// Singleton
	static Input* const GetSingletonObject();
	static void	ReleaseSingleton();

	// Key Input
	bool KeyCheck(int key, KeyState state);

	// Mouse Pos
	void SetMousePos(LPARAM lParam);
	const POINT& GetMousePos();

	// Mouse On Button
	bool IsMouseOn(const RECT& rc);
};