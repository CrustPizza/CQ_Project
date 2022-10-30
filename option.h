#pragma once

class Option
{
	// ����
private:
	// Singleton
	static Option* SingletonObject;

	// ���
	short brightness;
	short saveBrightness;

	// ����
	short soundVolume;
	short bgmVolume;

	// �ɼ�â
	bool isOptionOpen;

	// ȭ�� ��ȯ
	bool screenChange;

	// Pause Button
	RECT pauseButton;

	// Option Rect
	RECT optionWindow;

	enum class OptionType { Sound, BGM, Brightness };

	// Sound Scroll
	RECT soundScroll;
	RECT soundScrollBox;
	bool isSoundBoxClick;

	// Bgm Scroll
	RECT bgmScroll;
	RECT bgmScrollBox;
	bool isBgmBoxClick;

	// Brightness Scroll
	RECT brightnessScroll;
	RECT brightnessScrollBox;
	bool isBrightnessBoxClick;

	// Close Button
	RECT closeButton;

	// Exit BUtton
	RECT exitButton;

	// Scroll
	enum class ScrollGap
	{
		OptionBlank = 70,
		ScrollHeigthBlank = 30,
		ScrollWidthBlank = 60
	};

	// �Լ�
public:
	Option();
	~Option();

	// Singleton
	static Option* const GetSingletonObject();
	static void	ReleaseSingleton();
	
	// ȭ�� ��ȯ ���ÿ� �� ������ ������־�� ��
	bool SceneOpen();
	bool SceneClose();

	bool Update();
	void Render();

private:
	// �ɼ� ����
	void SetBrightness(short alpha);

	// ��ũ�� ��
	void CheckScrollBar();
	void DrawScrollBar(OptionType type);
	void ScrollClickDownCheck(OptionType type);

	// ��ġ ����
	void MoveToClickLocation(RECT& rc, OptionType type);
};