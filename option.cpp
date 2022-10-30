#include "stdafx.h"
#include "option.h"

Option* Option::SingletonObject = nullptr;

Option::Option()
{
	// 밝기
	brightness = 255;
	saveBrightness = 0;

	// 볼륨
	soundVolume = 100;
	bgmVolume = 100;

	// 옵션창 On/Off
	isOptionOpen = false;

	// 화면 전환
	screenChange = false;

	// Pause Button Rect
	int buttonWidth = IMAGE_MANAGER->GetImage(UI_PAUSE_UP)->width;
	int buttonHeight = IMAGE_MANAGER->GetImage(UI_PAUSE_UP)->height;
	pauseButton = { (int)WinInfo::Width - buttonWidth - 5, 5, (int)WinInfo::Width - 5, 5 + buttonHeight };

	// Option Rect
	int optionWidth = IMAGE_MANAGER->GetImage(UI_OPTION_BOARD)->width;
	int optionHeight = IMAGE_MANAGER->GetImage(UI_OPTION_BOARD)->height;
	optionWindow = { ((int)WinInfo::Width - optionWidth) / 2, ((int)WinInfo::Height - optionHeight) / 2,
		((int)WinInfo::Width + optionWidth) / 2, ((int)WinInfo::Height + optionHeight) / 2 };

	// Sound Scroll Rect
	int scrollWidth = IMAGE_MANAGER->GetImage(UI_SCROLL_FRONTBAR)->width;
	int scrollHeight = IMAGE_MANAGER->GetImage(UI_SCROLL_FRONTBAR)->height;
	soundScroll = { (optionWindow.right + optionWindow.left - scrollWidth) / 2 + (int)ScrollGap::ScrollWidthBlank, optionWindow.top + (int)ScrollGap::OptionBlank,
	(optionWindow.right + optionWindow.left + scrollWidth) / 2 + (int)ScrollGap::ScrollWidthBlank, optionWindow.top + (int)ScrollGap::OptionBlank + scrollHeight };

	int scrollBoxWidth = IMAGE_MANAGER->GetImage(UI_SCROLL_BOX)->width;
	int scrollBoxHeight = IMAGE_MANAGER->GetImage(UI_SCROLL_BOX)->height;
	soundScrollBox = {
		soundScroll.right - scrollBoxWidth,
		(soundScroll.top + soundScroll.bottom - scrollBoxHeight) / 2,
		soundScroll.right,
		(soundScroll.top + soundScroll.bottom + scrollBoxHeight) / 2
	};

	// Bgm Scroll Rect
	bgmScroll = soundScroll;
	bgmScroll.top = soundScroll.bottom + (int)ScrollGap::ScrollHeigthBlank;
	bgmScroll.bottom = bgmScroll.top + scrollHeight;
	bgmScrollBox = {
		bgmScroll.right - scrollBoxWidth,
		(bgmScroll.top + bgmScroll.bottom - scrollBoxHeight) / 2,
		bgmScroll.right,
		(bgmScroll.top + bgmScroll.bottom + scrollBoxHeight) / 2
	};

	// Bright Scroll Rect
	brightnessScroll = bgmScroll;
	brightnessScroll.top = bgmScroll.bottom + (int)ScrollGap::ScrollHeigthBlank;
	brightnessScroll.bottom = brightnessScroll.top + scrollHeight;
	brightnessScrollBox = {
		brightnessScroll.right - scrollBoxWidth,
		(brightnessScroll.top + brightnessScroll.bottom - scrollBoxHeight) / 2,
		brightnessScroll.right,
		(brightnessScroll.top + brightnessScroll.bottom + scrollBoxHeight) / 2
	};

	// Close Button Rect
	int closeButtonWidth = IMAGE_MANAGER->GetImage(UI_CLOSE_BUTTON)->width;
	int closeButtonHeight = IMAGE_MANAGER->GetImage(UI_CLOSE_BUTTON)->height;
	closeButton = {
		(optionWindow.right + optionWindow.left) / 2 + 10,
		optionWindow.bottom - closeButtonHeight - 20,
		(optionWindow.right + optionWindow.left) / 2 + closeButtonWidth + 10,
		optionWindow.bottom - 20
	};

	// Exit Button Rect
	int exitButtonWidth = IMAGE_MANAGER->GetImage(UI_EXIT_BUTTON)->width;
	int exitButtonHeight = IMAGE_MANAGER->GetImage(UI_EXIT_BUTTON)->height;
	exitButton = {
		(optionWindow.right + optionWindow.left) / 2 - exitButtonWidth - 10,
		optionWindow.bottom - exitButtonHeight - 20,
		(optionWindow.right + optionWindow.left) / 2 - 10,
		optionWindow.bottom - 20
	};
}

Option::~Option()
{

}

/// <summary>
/// 싱글톤 생성 및 반환
/// </summary>
Option* const Option::GetSingletonObject()
{
	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Option();
	}

	return SingletonObject;
}

/// <summary>
/// 싱글톤 해제
/// </summary>
void Option::ReleaseSingleton()
{
	// 싱글톤 객체가 있다면 해제한다.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// Scene이 전환되고 밝아지는 효과를 연출하는 함수
/// </summary>
bool Option::SceneOpen()
{
	brightness -= (255 - saveBrightness) / (int)WinInfo::Frame;

	if (brightness <= saveBrightness)
	{
		brightness = saveBrightness;
		screenChange = false;
		return true;
	}

	return false;
}

/// <summary>
/// Scene이 전환될 때 어두워지는 효과를 연출하는 함수
/// </summary>
bool Option::SceneClose()
{
	brightness += (255 - saveBrightness) / (int)WinInfo::Frame;
	screenChange = true;

	if (brightness >= 255)
	{
		brightness = 255;
		return true;
	}

	return false;
}

bool Option::Update()
{
	// 화면이 전환중이 아닐 경우에만 작동
	if (screenChange == false)
	{
		// 옵션창이 닫혀 있는 경우에는 버튼을 눌러도 작동하게 한다.
		if (isOptionOpen == false)
		{
			if (INPUT_MANAGER->IsMouseOn(pauseButton) == true)
			{
				if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true)
				{
					isOptionOpen = !isOptionOpen;
				}
			}
		}
		else
		{
			if (INPUT_MANAGER->IsMouseOn(closeButton) == true)
			{
				if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true)
				{
					isOptionOpen = !isOptionOpen;
				}
			}

			if (INPUT_MANAGER->IsMouseOn(exitButton) == true)
			{
				if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true)
				{
					SCENE_MANAGER->SceneBack();
					isOptionOpen = !isOptionOpen;
				}
			}

			CheckScrollBar();
		}

		if (INPUT_MANAGER->KeyCheck(VK_ESCAPE, KeyState::DOWN) == true)
		{
			isOptionOpen = !isOptionOpen;
		}
	}

	return isOptionOpen;
}

void Option::Render()
{
	// Pause 버튼
	if (isOptionOpen == true)
	{
		RENDER_MANAGER->DrawTransImage(UI_PAUSE_DOWN, pauseButton.left, pauseButton.top);
		RENDER_MANAGER->DrawAlphaImage(UI_BRIGHTNESS, 0, 0, 200);
	}
	else
	{
		RENDER_MANAGER->DrawTransImage(UI_PAUSE_UP, pauseButton.left, pauseButton.top);
	}

	// 옵션창
	if (isOptionOpen == true)
	{
		RENDER_MANAGER->DrawTransImage(UI_OPTION_BOARD, optionWindow.left, optionWindow.top);
		DrawScrollBar(OptionType::Sound);
		DrawScrollBar(OptionType::BGM);
		DrawScrollBar(OptionType::Brightness);
		RENDER_MANAGER->DrawTransImage(UI_CLOSE_BUTTON, closeButton.left, closeButton.top);
		RENDER_MANAGER->DrawTransImage(UI_EXIT_BUTTON, exitButton.left, exitButton.top);
	}

	// 밝기
	if (brightness > 0)
	{
		RENDER_MANAGER->DrawAlphaImage(UI_BRIGHTNESS, 0, 0, brightness);
	}
}

/// <summary>
/// 밝기 조절
/// </summary>
void Option::SetBrightness(short alpha)
{
	brightness = alpha;
	saveBrightness = alpha;
}

void Option::CheckScrollBar()
{
	// 누른 시점에서 스크롤을 눌렀으면 단순히 이동하고
	// 박스를 눌렀으면 좌우로 드래그 가능하게 bool값을 True로 한다.
	if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true)
	{
		ScrollClickDownCheck(OptionType::Sound);
		ScrollClickDownCheck(OptionType::BGM);
		ScrollClickDownCheck(OptionType::Brightness);
	}

	// 위에서 박스를 눌러서 true가 되면 드래그로 값을 조절하게 한다.
	if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::STAY) == true)
	{
		if (isSoundBoxClick == true)
		{
			MoveToClickLocation(soundScrollBox, OptionType::Sound);
		}

		if (isBgmBoxClick == true)
		{
			MoveToClickLocation(bgmScrollBox, OptionType::BGM);
		}

		if (isBrightnessBoxClick == true)
		{
			MoveToClickLocation(brightnessScrollBox, OptionType::Brightness);
		}
	}

	// 버튼을 때면 bool값을 초기화한다.
	if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::UP) == true)
	{
		isSoundBoxClick = false;
		isBgmBoxClick = false;
		isBrightnessBoxClick = false;
	}
}

/// <summary>
/// 스크롤 바를 그린다
/// </summary>
void Option::DrawScrollBar(OptionType type)
{
	int iconWidth = IMAGE_MANAGER->GetImage(UI_SOUND_ICON)->width;
	int iconHeight = IMAGE_MANAGER->GetImage(UI_SOUND_ICON)->height;

	switch (type)
	{
	case OptionType::Sound:
		RENDER_MANAGER->DrawBarImage(UI_SCROLL_UNDERBAR, soundScroll.left, soundScroll.top, (float)soundVolume / 100);
		RENDER_MANAGER->DrawTransImage(UI_SCROLL_BOX, soundScrollBox.left, soundScrollBox.top);
		RENDER_MANAGER->DrawTransImage(UI_SOUND_ICON, (soundScroll.left + optionWindow.left - iconWidth) / 2, (soundScroll.top + soundScroll.bottom - iconHeight) / 2);
		break;
	case OptionType::BGM:
		RENDER_MANAGER->DrawBarImage(UI_SCROLL_UNDERBAR, bgmScroll.left, bgmScroll.top, (float)bgmVolume / 100);
		RENDER_MANAGER->DrawTransImage(UI_SCROLL_BOX, bgmScrollBox.left, bgmScrollBox.top);
		RENDER_MANAGER->DrawTransImage(UI_BGM_ICON, (bgmScroll.left + optionWindow.left - iconWidth) / 2, (bgmScroll.top + bgmScroll.bottom - iconHeight) / 2);
		break;
	case OptionType::Brightness:
		RENDER_MANAGER->DrawBarImage(UI_SCROLL_UNDERBAR, brightnessScroll.left, brightnessScroll.top, (float)(150 - saveBrightness) / 150);
		RENDER_MANAGER->DrawTransImage(UI_SCROLL_BOX, brightnessScrollBox.left, brightnessScrollBox.top);
		RENDER_MANAGER->DrawTransImage(UI_BRIGHTNESS_ICON, (brightnessScroll.left + optionWindow.left - iconWidth) / 2, (brightnessScroll.top + brightnessScroll.bottom - iconHeight) / 2);
		break;
	}
}

/// <summary>
/// 스크롤 바 클릭했는지 확인
/// </summary>
void Option::ScrollClickDownCheck(OptionType type)
{
	switch (type)
	{
	case OptionType::Sound:
		if (INPUT_MANAGER->IsMouseOn(soundScroll) == true)
		{
			if (INPUT_MANAGER->IsMouseOn(soundScrollBox) == true)
			{
				isSoundBoxClick = true;
			}

			MoveToClickLocation(soundScrollBox, OptionType::Sound);
		}
		break;
	case OptionType::BGM:
		if (INPUT_MANAGER->IsMouseOn(bgmScroll) == true)
		{
			if (INPUT_MANAGER->IsMouseOn(bgmScrollBox) == true)
			{
				isBgmBoxClick = true;
			}

			MoveToClickLocation(bgmScrollBox, OptionType::BGM);
		}
		break;
	case OptionType::Brightness:
		if (INPUT_MANAGER->IsMouseOn(brightnessScroll) == true)
		{
			if (INPUT_MANAGER->IsMouseOn(brightnessScrollBox) == true)
			{
				isBrightnessBoxClick = true;
			}

			MoveToClickLocation(brightnessScrollBox, OptionType::Brightness);
		}
		break;
	}
}

/// <summary>
/// 스크롤 박스를 마우스 클릭 위치로 옮긴다.
/// </summary>
void Option::MoveToClickLocation(RECT& rc, OptionType type)
{
	int halfWidth = (rc.right - rc.left) / 2;
	int halfHeight = (rc.bottom - rc.top) / 2;
	const POINT& curCursorPoint = INPUT_MANAGER->GetMousePos();

	// 마우스 좌표를 중심으로 x 값 변경
	rc.left = curCursorPoint.x - halfWidth;
	rc.right = curCursorPoint.x + halfWidth;

	// x값이 스크롤을 벗어나면 위치 조절
	if (rc.right >= soundScroll.right)
	{
		rc.left = soundScroll.right - halfWidth * 2;
		rc.right = soundScroll.right;
	}
	else if (rc.left <= soundScroll.left)
	{
		rc.left = soundScroll.left;
		rc.right = soundScroll.left + halfWidth * 2;
	}

	// 변경된 위치에 따라 각 값을 조절
	switch (type)
	{
	case OptionType::Sound:
		soundVolume = ((float)(rc.left - soundScroll.left) * 100 / (float)(soundScroll.right - soundScroll.left - halfWidth * 2));
		SOUND_MANAGER->SetVolume(Soundtype::SFX, (float)soundVolume / 100);
		break;
	case OptionType::BGM:
		bgmVolume = ((float)(rc.left - soundScroll.left) * 100 / (float)(soundScroll.right - soundScroll.left - halfWidth * 2));
		SOUND_MANAGER->SetVolume(Soundtype::BGM, (float)bgmVolume / 100);
		break;
	case OptionType::Brightness:
		SetBrightness(150 - ((float)(rc.left - soundScroll.left) * 150 / (float)(soundScroll.right - soundScroll.left - halfWidth * 2)));
		break;
	}
}