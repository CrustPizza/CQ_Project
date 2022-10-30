#pragma once
#include "sceneManager.h"

class Battle : private Scene
{
	// ����
private:
	const GameRule* waveData;
	bool startScene;
	bool closeScene;

	double enemyStatRate;
	int index;
	int delay;

	// �Լ�
public:
	Battle();
	~Battle();

	void Init();
	void Update();
	void Render();

	void SetChange();

private:
	void RoundStart();

	void DrawBackGround(int x, int y);
	void DrawBlockBoard(int x, int y);
	void DrawBoxStatus(int x, int y);
	void DrawCoinStatus(int x, int y);
};