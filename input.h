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
	// ����
private:
	// Singleton
	static Input* SingletonObject;

	// Ű Up / Down Bitmask
	int	keyDown[8];
	int	keyUp[8];

	// ���콺 ��ǥ
	POINT	cursorXY;

public:
	// ������ Ȱ��ȭ ���� Input �Ÿ���
	bool isMouseInWindow;
	bool isWindowActivated;

	// �Լ�
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