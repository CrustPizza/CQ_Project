#include "stdafx.h"
#include "image.h"

// Singleton Initialize
Image* Image::SingletonObject = nullptr;

/// <summary>
/// ��Ʈ�� �ʱ�ȭ ������
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
/// �Ҹ���
/// </summary>
Image::~Image()
{
	Release();
}

/// <summary>
/// �̱��� ���� �� ��ȯ
/// </summary>
Image* const Image::GetSingletonObject(const HINSTANCE& hInst, const HWND& _hWnd)
{
	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
	// �̹��� �Ŵ����� ��� �ʱ� ���� �ʿ��ؼ� ó���� ���ڸ� ���� �޾Ƽ� �����ϰ�
	// �Ŀ� ����Ʈ�� 0�� ����Ǿ� �μ����� ����� �� �ִ�.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Image(hInst, _hWnd);
	}

	return SingletonObject;
}

/// <summary>
/// �̱��� ����
/// </summary>
void Image::ReleaseSingleton()
{
	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
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
/// Json���� ������ ����Ʈ �����ͼ� ����� �̹��� ���
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
/// Back Buffer �����ϴ� ���� �Լ�
/// </summary>
void Image::AddBackBuffer()
{
	ImageData temp;

	// HDC
	temp.memDC = CreateCompatibleDC(hdc);

	// Back Buffer�� ����� ��Ʈ�� �����ϱ�
	temp.image = CreateCompatibleBitmap(hdc, windowSize.right, windowSize.bottom);
	SelectObject(temp.memDC, temp.image);

	// Back Buffer ���� ���� ũ��
	temp.width = windowSize.right;
	temp.height = windowSize.bottom;

	// 0���� Back Buffer�� ��ȣ�� ����Ѵ�.
	images[(int)BufferType::BackBuffer] = temp;
}

/// <summary>
/// �������� �̹��� ������ �����ϴ� �Լ�
/// �ܺ� �����Ű�� �ʴ´�.
/// </summary>
ImageData Image::InputData(int imageID, int pivotX, int pivotY)
{
	ImageData temp;
	BITMAP	bitmapInfo;

	// HDC
	temp.memDC = CreateCompatibleDC(hdc);

	// �̹��� �ҷ�����
	temp.image = LoadBitmap(hInstance, MAKEINTRESOURCE(imageID));
	SelectObject(temp.memDC, temp.image);

	// �̹��� ���� ���� ũ��
	GetObject(temp.image, sizeof(BITMAP), &bitmapInfo);
	temp.width = bitmapInfo.bmWidth;
	temp.height = bitmapInfo.bmHeight;

	// �̹��� �»�� (0, 0) ���� �Ǻ� ��
	temp.pivotX = pivotX;
	temp.pivotY = pivotY;

	// ����� �÷�
	temp.transColor = RGB(0, 255, 0);

	return temp;
}

/// <summary>
/// �������� �̹��� ������ Json�� ���� �����͸� �о�ͼ� �����ϴ� �Լ�
/// �ܺ� �����Ű�� �ʴ´�.
/// </summary>
ImageData Image::InputDataFromJson(int imageID, int pivotX, int pivotY)
{
	ImageData temp;
	BITMAP	bitmapInfo;

	// HDC
	temp.memDC = CreateCompatibleDC(hdc);

	// �̹��� �ҷ�����
	temp.image = (HBITMAP)LoadImage(hInstance, TABLE_MANAGER->FindImageDataFromID(imageID)->ImageFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(temp.memDC, temp.image);

	// �̹��� ���� ���� ũ��
	GetObject(temp.image, sizeof(BITMAP), &bitmapInfo);
	temp.width = bitmapInfo.bmWidth;
	temp.height = bitmapInfo.bmHeight;

	// �̹��� �»�� (0, 0) ���� �Ǻ� ��
	temp.pivotX = pivotX;
	temp.pivotY = pivotY;

	// ����� �÷�
	temp.transColor = RGB(0, 255, 0);

	return temp;
}

/// <summary>
/// �̹��� �߰�
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
/// Json ����Ͽ� �̹��� �߰�
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
/// �̹��� ��ȯ
/// </summary>
const ImageData* const Image::GetImage(int imageID)
{
	return &images[imageID];
}

/// <summary>
/// �ʱ�ȭ
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