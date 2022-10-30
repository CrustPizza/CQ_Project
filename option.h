#pragma once

class Option
{
	// 변수
private:
	// Singleton
	static Option* SingletonObject;

	// 밝기
	short brightness;
	short saveBrightness;

	// 볼륨
	short soundVolume;
	short bgmVolume;

	// 옵션창
	bool isOptionOpen;

	// 화면 전환
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

	// 함수
public:
	Option();
	~Option();

	// Singleton
	static Option* const GetSingletonObject();
	static void	ReleaseSingleton();
	
	// 화면 전환 사용시에 한 쌍으로 사용해주어야 함
	bool SceneOpen();
	bool SceneClose();

	bool Update();
	void Render();

private:
	// 옵션 세팅
	void SetBrightness(short alpha);

	// 스크롤 바
	void CheckScrollBar();
	void DrawScrollBar(OptionType type);
	void ScrollClickDownCheck(OptionType type);

	// 위치 설정
	void MoveToClickLocation(RECT& rc, OptionType type);
};