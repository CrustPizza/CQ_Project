#include "stdafx.h"
#include "intro.h"

Intro::Intro()
{
	// 시작 버튼 좌표 설정
	int buttonWidth = IMAGE_MANAGER->GetImage(UI_STARTBUTTON)->width;
	int buttonHeight = IMAGE_MANAGER->GetImage(UI_STARTBUTTON)->height;
	startButton = { ((int)WinInfo::Width - buttonWidth) / 2, ((int)WinInfo::Height * 4) / 5 - buttonHeight / 2,
	((int)WinInfo::Width - buttonWidth) / 2 + buttonWidth, ((int)WinInfo::Height * 4) / 5 - buttonHeight / 2 + buttonHeight };

	// 버튼의 초기 Alpha 값
	buttonAlpha = 255;
	flag = true;
	startScene = true;
	closeScene = false;
}

Intro::~Intro()
{

}

/// <summary>
/// Start Button Alpha값 조절하는 함수
/// </summary>
void Intro::ChangeButtonAlpha()
{
	// Tap To Start Alpha
	if (flag == true)
	{
		if (buttonAlpha == 0)
		{
			flag = false;
		}
		else
		{
			buttonAlpha -= 15;
		}
	}
	else
	{
		if (buttonAlpha == 255)
		{
			flag = true;
		}
		else
		{
			buttonAlpha += 15;
		}
	}
}

void Intro::SetChange()
{
	closeScene = true;
}

/// <summary>
/// Intro 관련 로직 Update 함수
/// </summary>
void Intro::Update()
{
	if (closeScene == true)
	{
		if (OPTION_MANAGER->SceneClose() == true)
		{
			SOUND_MANAGER->Stop(500001);
			closeScene = false;
			startScene = true;
			SCENE_MANAGER->SceneChange("Battle");
		}

		return;
	}

	if (startScene == true)
	{
		if (OPTION_MANAGER->SceneOpen() != true)
		{
			return;
		}
		SOUND_MANAGER->Play(500001);
		startScene = false;
	}

	// 버튼을 클릭하면 게임 시작
	if (closeScene == false)
	{
		if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true
			|| INPUT_MANAGER->KeyCheck(VK_SPACE, KeyState::DOWN) == true
			|| INPUT_MANAGER->KeyCheck(VK_RETURN, KeyState::DOWN) == true)
		{			
			SOUND_MANAGER->Play(510001);
			closeScene = true;
		}
	}

	SOUND_MANAGER->Update();
	ChangeButtonAlpha();
}

/// <summary>
/// Intro 관련 로직 Render 함수
/// </summary>
void Intro::Render()
{
	RENDER_MANAGER->DrawImage(UI_ILLUSTBG, 0, 0);
	RENDER_MANAGER->DrawTransAlphaImage(UI_TITLE, 50, 200, 200);
	RENDER_MANAGER->DrawTransAlphaImage(UI_TITLE_KR, 700, 300, 150);
	RENDER_MANAGER->DrawAlphaImage(UI_STARTBUTTON, startButton.left, startButton.top, buttonAlpha);
}