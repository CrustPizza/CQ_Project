#include "stdafx.h"
#include "image.h"

// Singleton Initialize
Image* Image::SingletonObject = nullptr;

/// <summary>
/// 비트맵 초기화 생성자
/// </summary>
Image::Image(const HINSTANCE& hInst, const HWND& _hWnd)
{
	hInstance = hInst;
	hWnd = _hWnd;
	hdc = GetDC(hWnd);

	AddBackBuffer();
	Init();
}

/// <summary>
/// 소멸자
/// </summary>
Image::~Image()
{
	Release();
}

/// <summary>
/// 싱글톤 생성 및 반환
/// </summary>
Image* const Image::GetSingletonObject(const HINSTANCE& hInst, const HWND& _hWnd)
{
	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
	// 이미지 매니저의 경우 초기 값이 필요해서 처음에 인자를 전달 받아서 생성하고
	// 후에 디폴트로 0이 적용되어 인수없이 사용할 수 있다.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Image(hInst, _hWnd);
	}

	return SingletonObject;
}

/// <summary>
/// 싱글톤 해제
/// </summary>
void Image::ReleaseSingleton()
{
	// 싱글톤 객체가 있다면 해제한다.
	if (SingletonObject != nullptr)
	{
		SingletonObject->Release();

		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// Back Buffer DC
/// </summary>
const HDC Image::GetMemDC()
{
	return images[(int)BufferType::BackBuffer].memDC;
}

/// <summary>
/// Json에서 데이터 리스트 가져와서 사용할 이미지 등록
/// </summary>
void Image::Init()
{
	// BackGround
	AddImage(BG_LAYER1);
	AddImage(BG_LAYER2);
	AddImage(BG_LAYER3);
	AddImage(BG_LAYER4);

	// Intro UI
	AddImage(UI_ILLUSTBG);
	AddImage(UI_STARTBUTTON, 255, true);
	AddImage(UI_TITLE);
	AddImage(UI_TITLE_KR);

	// Battle UI
	AddImage(UI_BOARD_01);
	AddImage(UI_BOARD_02);
	AddImage(IG_DAMAGE_NUMBER);
	AddImage(IG_HEAL_NUMBER);
	AddImage(IG_WHITE_NUMBER);

	// Skill UI
	AddImage(UI_SKILL_01);
	AddImage(UI_SKILL_02);
	AddImage(UI_SKILL_03);
	AddImage(UI_CHAIN_01);
	AddImage(UI_CHAIN_02);
	AddImage(UI_CHAIN_03);

	// Status UI
	AddImage(UI_STATUS_HERO);
	AddImage(UI_STATUS_BOX);
	AddImage(UI_STATUS_COIN);
	AddImage(UI_STATUS_EMPTY_HP);
	AddImage(UI_STATUS_EMPTY_SP);
	AddImage(UI_STATUS_FULL_HP);
	AddImage(UI_STATUS_FULL_SP);
	AddImage(ARCHON_PROFILE);
	AddImage(JEANNE_PROFILE);
	AddImage(MEOW_PROFILE);
	AddImage(IG_GOLD_NUMBER);
	AddImage(UI_GAME_OVER);

	// Option UI
	AddImage(UI_BRIGHTNESS);
	AddImage(UI_PAUSE_DOWN);
	AddImage(UI_PAUSE_UP);
	AddImage(UI_OPTION_BOARD);
	AddImage(UI_SOUND_ICON);
	AddImage(UI_BGM_ICON);
	AddImage(UI_MINUSON_ICON);
	AddImage(UI_MINUSOFF_ICON);
	AddImage(UI_BRIGHTNESS_ICON);
	AddImage(UI_SCROLL_UNDERBAR);
	AddImage(UI_SCROLL_FRONTBAR);
	AddImage(UI_SCROLL_BOX);
	AddImage(UI_CLOSE_BUTTON);
	AddImage(UI_EXIT_BUTTON);

	// Read image data from json
	for (int i = 0; i < TABLE_MANAGER->FindLastIndexOfTable(TableType::ImageResource); i++)
	{
		AddImageFromJson(TABLE_MANAGER->FindIDfromIndex(i, TableType::ImageResource),
			TABLE_MANAGER->FindImageDataFromID(
				TABLE_MANAGER->FindIDfromIndex(i, TableType::ImageResource))->AlphaValue,
			TABLE_MANAGER->FindImageDataFromID(
				TABLE_MANAGER->FindIDfromIndex(i, TableType::ImageResource))->AlphaFormat,
			TABLE_MANAGER->FindImageDataFromID(
				TABLE_MANAGER->FindIDfromIndex(i, TableType::ImageResource))->Pivot[0],
			TABLE_MANAGER->FindImageDataFromID(
				TABLE_MANAGER->FindIDfromIndex(i, TableType::ImageResource))->Pivot[1]);
	}
}

/// <summary>
/// Back Buffer 생성하는 내부 함수
/// </summary>
void Image::AddBackBuffer()
{
	ImageData temp;

	// HDC
	temp.memDC = CreateCompatibleDC(hdc);

	// Back Buffer로 사용할 비트맵 생성하기
	temp.image = CreateCompatibleBitmap(hdc, windowSize.right, windowSize.bottom);
	SelectObject(temp.memDC, temp.image);

	// Back Buffer 가로 세로 크기
	temp.width = windowSize.right;
	temp.height = windowSize.bottom;

	// 0번을 Back Buffer의 번호로 사용한다.
	images[(int)BufferType::BackBuffer] = temp;
}

/// <summary>
/// 기초적인 이미지 정보를 저장하는 함수
/// 외부 노출시키지 않는다.
/// </summary>
ImageData Image::InputData(int imageID, int pivotX, int pivotY)
{
	ImageData temp;
	BITMAP	bitmapInfo;

	// HDC
	temp.memDC = CreateCompatibleDC(hdc);

	// 이미지 불러오기
	temp.image = LoadBitmap(hInstance, MAKEINTRESOURCE(imageID));
	SelectObject(temp.memDC, temp.image);

	// 이미지 가로 세로 크기
	GetObject(temp.image, sizeof(BITMAP), &bitmapInfo);
	temp.width = bitmapInfo.bmWidth;
	temp.height = bitmapInfo.bmHeight;

	// 이미지 좌상단 (0, 0) 기준 피봇 값
	temp.pivotX = pivotX;
	temp.pivotY = pivotY;

	// 투명색 컬러
	temp.transColor = RGB(0, 255, 0);

	return temp;
}

/// <summary>
/// 기초적인 이미지 정보를 Json을 통해 데이터를 읽어와서 저장하는 함수
/// 외부 노출시키지 않는다.
/// </summary>
ImageData Image::InputDataFromJson(int imageID, int pivotX, int pivotY)
{
	ImageData temp;
	BITMAP	bitmapInfo;

	// HDC
	temp.memDC = CreateCompatibleDC(hdc);

	// 이미지 불러오기
	temp.image = (HBITMAP)LoadImage(hInstance, TABLE_MANAGER->FindImageDataFromID(imageID)->ImageFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(temp.memDC, temp.image);

	// 이미지 가로 세로 크기
	GetObject(temp.image, sizeof(BITMAP), &bitmapInfo);
	temp.width = bitmapInfo.bmWidth;
	temp.height = bitmapInfo.bmHeight;

	// 이미지 좌상단 (0, 0) 기준 피봇 값
	temp.pivotX = pivotX;
	temp.pivotY = pivotY;

	// 투명색 컬러
	temp.transColor = RGB(0, 255, 0);

	return temp;
}

/// <summary>
/// 이미지 추가
/// </summary>
void Image::AddImage(int imageID, int alpha, bool alphaFormat, int pivotX, int pivotY)
{
	images[imageID] = InputData(imageID, pivotX, pivotY);
	images[imageID].blendf.BlendOp = AC_SRC_OVER;
	images[imageID].blendf.BlendFlags = 0;
	images[imageID].blendf.AlphaFormat = alphaFormat;
	images[imageID].blendf.SourceConstantAlpha = alpha;
}

/// <summary>
/// Json 사용하여 이미지 추가
/// </summary>
void Image::AddImageFromJson(int imageID, int alpha, bool alphaFormat, int pivotX, int pivotY)
{
	images[imageID] = InputDataFromJson(imageID, pivotX, pivotY);
	images[imageID].blendf.BlendOp = AC_SRC_OVER;
	images[imageID].blendf.BlendFlags = 0;
	images[imageID].blendf.AlphaFormat = alphaFormat;
	images[imageID].blendf.SourceConstantAlpha = alpha;
}

/// <summary>
/// 이미지 반환
/// </summary>
const ImageData* const Image::GetImage(int imageID)
{
	return &images[imageID];
}

/// <summary>
/// 초기화
/// </summary>
void Image::Release()
{
	// Image release
	for (auto iter = images.begin(); iter != images.end(); iter++)
	{
		DeleteObject(iter->second.image);
		DeleteDC(iter->second.memDC);
	}

	images.clear();
}