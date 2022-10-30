#pragma once
#include "sceneManager.h"

class Intro : private Scene
{
	// 변수
private:
	bool startScene;
	bool closeScene;

	RECT startButton;
	int  buttonAlpha;
	bool flag;

	// 함수
public:
	Intro();
	~Intro();

	void ChangeButtonAlpha();
	void SetChange();

	void Update();
	void Render();
};