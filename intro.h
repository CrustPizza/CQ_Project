#pragma once
#include "sceneManager.h"

class Intro : private Scene
{
	// ����
private:
	bool startScene;
	bool closeScene;

	RECT startButton;
	int  buttonAlpha;
	bool flag;

	// �Լ�
public:
	Intro();
	~Intro();

	void ChangeButtonAlpha();
	void SetChange();

	void Update();
	void Render();
};