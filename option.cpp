#include "stdafx.h"
#include "option.h"

Option* Option::SingletonObject = nullptr;

Option::Option()
{
	// ���
	brightness = 255;
	saveBrightness = 0;

	// ����
	soundVolume = 100;
	bgmVolume = 100;

	// �ɼ�â On/Off
	isOptionOpen = false;

	// ȭ�� ��ȯ
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
/// �̱��� ���� �� ��ȯ
/// </summary>
Option* const Option::GetSingletonObject()
{
	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Option();
	}

	return SingletonObject;
}

/// <summary>
/// �̱��� ����
/// </summary>
void Option::ReleaseSingleton()
{
	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// Scene�� ��ȯ�ǰ� ������� ȿ���� �����ϴ� �Լ�
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
/// Scene�� ��ȯ�� �� ��ο����� ȿ���� �����ϴ� �Լ�
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
	// ȭ���� ��ȯ���� �ƴ� ��쿡�� �۵�
	if (screenChange == false)
	{
		// �ɼ�â�� ���� �ִ� ��쿡�� ��ư�� ������ �۵��ϰ� �Ѵ�.
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
	// Pause ��ư
	if (isOptionOpen == true)
	{
		RENDER_MANAGER->DrawTransImage(UI_PAUSE_DOWN, pauseButton.left, pauseButton.top);
		RENDER_MANAGER->DrawAlphaImage(UI_BRIGHTNESS, 0, 0, 200);
	}
	else
	{
		RENDER_MANAGER->DrawTransImage(UI_PAUSE_UP, pauseButton.left, pauseButton.top);
	}

	// �ɼ�â
	if (isOptionOpen == true)
	{
		RENDER_MANAGER->DrawTransImage(UI_OPTION_BOARD, optionWindow.left, optionWindow.top);
		DrawScrollBar(OptionType::Sound);
		DrawScrollBar(OptionType::BGM);
		DrawScrollBar(OptionType::Brightness);
		RENDER_MANAGER->DrawTransImage(UI_CLOSE_BUTTON, closeButton.left, closeButton.top);
		RENDER_MANAGER->DrawTransImage(UI_EXIT_BUTTON, exitButton.left, exitButton.top);
	}

	// ���
	if (brightness > 0)
	{
		RENDER_MANAGER->DrawAlphaImage(UI_BRIGHTNESS, 0, 0, brightness);
	}
}

/// <summary>
/// ��� ����
/// </summary>
void Option::SetBrightness(short alpha)
{
	brightness = alpha;
	saveBrightness = alpha;
}

void Option::CheckScrollBar()
{
	// ���� �������� ��ũ���� �������� �ܼ��� �̵��ϰ�
	// �ڽ��� �������� �¿�� �巡�� �����ϰ� bool���� True�� �Ѵ�.
	if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::DOWN) == true)
	{
		ScrollClickDownCheck(OptionType::Sound);
		ScrollClickDownCheck(OptionType::BGM);
		ScrollClickDownCheck(OptionType::Brightness);
	}

	// ������ �ڽ��� ������ true�� �Ǹ� �巡�׷� ���� �����ϰ� �Ѵ�.
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

	// ��ư�� ���� bool���� �ʱ�ȭ�Ѵ�.
	if (INPUT_MANAGER->KeyCheck(VK_LBUTTON, KeyState::UP) == true)
	{
		isSoundBoxClick = false;
		isBgmBoxClick = false;
		isBrightnessBoxClick = false;
	}
}

/// <summary>
/// ��ũ�� �ٸ� �׸���
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
/// ��ũ�� �� Ŭ���ߴ��� Ȯ��
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
/// ��ũ�� �ڽ��� ���콺 Ŭ�� ��ġ�� �ű��.
/// </summary>
void Option::MoveToClickLocation(RECT& rc, OptionType type)
{
	int halfWidth = (rc.right - rc.left) / 2;
	int halfHeight = (rc.bottom - rc.top) / 2;
	const POINT& curCursorPoint = INPUT_MANAGER->GetMousePos();

	// ���콺 ��ǥ�� �߽����� x �� ����
	rc.left = curCursorPoint.x - halfWidth;
	rc.right = curCursorPoint.x + halfWidth;

	// x���� ��ũ���� ����� ��ġ ����
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

	// ����� ��ġ�� ���� �� ���� ����
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