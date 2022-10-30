#include "stdafx.h"
#include "render.h"

RenderClass* RenderClass::SingletonObject = nullptr;
HDC	RenderClass::memDC = nullptr;

/// <summary>
/// 생성자
/// </summary>
RenderClass::RenderClass()
{
	scroll_location_ground = 0;
	scroll_location_back01 = 0;
	scroll_location_back02 = 0;
	scroll_location_back03 = 0;

	memDC = IMAGE_MANAGER->GetMemDC();

#ifdef DebugMode
	frame = 0;
	total = 0;
	time = 0;
#endif
}

/// <summary>
/// 소멸자
/// </summary>
RenderClass::~RenderClass()
{

}

/// <summary>
/// 싱글톤 생성 및 반환
/// </summary>
RenderClass* const RenderClass::GetSingletonObject()
{
	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new RenderClass();
	}

	return SingletonObject;
}

/// <summary>
/// 싱글톤 해제
/// </summary>
void RenderClass::ReleaseSingleton()
{
	// 싱글톤 객체가 있다면 해제한다.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// Buffer 복사 함수
/// </summary>
void RenderClass::Render(HDC hdc)
{
	// Back Buffer를 메인 hdc로 복사한다.
	BitBlt(
		hdc,
		0, 0, windowSize.right, windowSize.bottom,
		memDC, 0, 0, SRCCOPY);

#ifdef DebugMode
	frame++;
#endif
}

#ifdef DebugMode
/// <summary>
/// Debug용 히트박스
/// </summary>
void RenderClass::HitBox(POINT dest, POINT src, COLORREF color, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID, 3, color);
	HPEN hOld = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, dest.x, dest.y, nullptr);
	LineTo(hdc, dest.x + src.x, dest.y);
	LineTo(hdc, dest.x + src.x, dest.y + src.y);
	LineTo(hdc, dest.x, dest.y + src.y);
	LineTo(hdc, dest.x, dest.y);

	DeleteObject(SelectObject(hdc, hOld));
}

void RenderClass::FPS()
{
	time++;
	cerr << "FPS : " << +frame;
	cerr << " / Play Time : " << time / 60 << "m " << time % 60;
	cerr << "s / Average : " << total / time << endl;
	total += frame;
	frame = 0;
}
#endif DebugMode

/// <summary>
/// 기본적인 이미지 출력
/// </summary>
void RenderClass::DrawImage(int imageID, int x, int y, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// 이미지를 피봇 값만큼 뺀 위치에 출력시킨다.
	BitBlt(
		hdc,
		x - image->pivotX, y - image->pivotY,
		image->width, image->height,
		image->memDC,
		0, 0, SRCCOPY);
}

/// <summary>
/// 투명색 이미지 출력
/// </summary>
void RenderClass::DrawTransImage(int imageID, int x, int y, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// 이미지를 피봇 값만큼 뺀 위치에서 지정된 RGB값을 제외하고 출력한다.
	TransparentBlt(
		hdc,
		x - image->pivotX, y - image->pivotY,
		image->width, image->height,
		image->memDC,
		0, 0, image->width, image->height,
		image->transColor);
}

/// <summary>
/// 스케일 이미지 출력
/// </summary>
void RenderClass::DrawScaleImage(int imageID, int x, int y, float size, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	StretchBlt(
		hdc,
		x - image->pivotX * size, y - image->pivotY * size,
		image->width * size, image->height * size,
		image->memDC,
		0, 0, image->width, image->height, SRCCOPY);
}

/// <summary>
/// 투명이 포함된 스케일 이미지 출력
/// </summary>
void RenderClass::DrawTransScaleImage(int imageID, int x, int y, float size, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// Scale -> Transparent 2중 작업을 한다.
	// 2중 작업을 위해 Scale 사이즈 만큼의 밑바탕을 만든다.
	HDC tempDc = CreateCompatibleDC(image->memDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(image->memDC, image->width * size, image->height * size);
	SelectObject(tempDc, hBitmap);

	// 밑바탕에 확장/축소시켜 그린다.
	StretchBlt(
		tempDc,
		0, 0,
		image->width * size, image->height * size,
		image->memDC,
		0, 0, image->width, image->height, SRCCOPY);

	// Back Buffer에 피봇 값을 뺀 위치에 지정된 RGB값을 제외하고 출력한다.
	TransparentBlt(
		hdc,
		x - image->pivotX * size, y - image->pivotY * size,
		image->width * size, image->height * size,
		tempDc,
		0, 0,
		image->width * size, image->height * size,
		image->transColor);

	DeleteObject(hBitmap);
	DeleteDC(tempDc);
}

/// <summary>
/// 이미지 스크롤링
/// </summary>
void RenderClass::DrawScrollImage(int imageID, int x, int y, ScrollLayer layer, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);
	int* pLocate;

	if (layer == ScrollLayer::GROUND)
	{
		pLocate = &scroll_location_ground;
	}
	else if (layer == ScrollLayer::BACK01)
	{
		pLocate = &scroll_location_back01;
	}
	else if (layer == ScrollLayer::BACK02)
	{
		pLocate = &scroll_location_back02;
	}
	else if (layer == ScrollLayer::BACK03)
	{
		pLocate = &scroll_location_back03;
	}
	else
	{
		pLocate = &scroll_location_back03;
	}

	// 만약, 맵이 이미지영역 바깥으로 이동했을 경우
	if (*pLocate < 0)
	{

		// 좌측영역
		BitBlt(
			hdc,										// 이미지를 출력할 위치의 핸들
			x - image->pivotX, y - image->pivotY,		// 이미지를 출력할 위치인 x,y 좌표
			-*pLocate, image->height,					// 가져올 이미지에서 잘라낼 너비, 높이
			image->memDC,								// 이미지의 핸들
			image->width + *pLocate, 0,					// 가져올 이미지의 시작지점 x,y 좌표
			SRCCOPY);


		// 우측영역
		BitBlt(
			hdc,
			x - image->pivotX - *pLocate, y - image->pivotY,
			static_cast<int>(WinInfo::Width) + *pLocate, image->height,
			image->memDC,
			0, 0, SRCCOPY);

	}
	else
	{
		BitBlt(
			hdc,
			x - image->pivotX - *pLocate, y - image->pivotY,
			image->width, image->height,
			image->memDC,
			0, 0, SRCCOPY);
	}

	if (*pLocate >= image->width - static_cast<int>(WinInfo::Width))
	{
		*pLocate -= image->width;
	}
}

/// <summary>
/// 투명이 포함된 이미지 스크롤링
/// </summary>
void RenderClass::DrawScrollTransImage(int imageID, int x, int y, ScrollLayer layer, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);
	int* pLocate;

	if (layer == ScrollLayer::GROUND)
	{
		pLocate = &scroll_location_ground;
	}
	else if (layer == ScrollLayer::BACK01)
	{
		pLocate = &scroll_location_back01;
	}
	else if (layer == ScrollLayer::BACK02)
	{
		pLocate = &scroll_location_back02;
	}
	else
	{
		pLocate = &scroll_location_back03;
	}

	// 만약, 맵이 이미지영역 바깥으로 이동했을 경우
	if (*pLocate < 0)
	{

		// 좌측영역
		TransparentBlt(
			hdc,										// 이미지를 출력할 위치의 핸들
			x - image->pivotX, y - image->pivotY,		// 이미지를 출력할 위치인 x,y 좌표
			-*pLocate, image->height,					// 출력할 이미지의 너비, 높이
			image->memDC,								// 이미지의 핸들
			image->width + *pLocate, 0,					// 가져올 이미지의 시작지점 x,y 좌표
			-*pLocate, image->height,					// 가져올 이미지에서 잘라낼 너비, 높이
			image->transColor);							// 투명색


		// 우측영역
		TransparentBlt(
			hdc,
			x - image->pivotX - *pLocate, y - image->pivotY,
			static_cast<int>(WinInfo::Width) + *pLocate, image->height,
			image->memDC,
			0, 0,
			static_cast<int>(WinInfo::Width) + *pLocate, image->height,
			image->transColor);

	}
	else
	{
		TransparentBlt(
			hdc,
			x - image->pivotX - *pLocate, y - image->pivotY,
			image->width, image->height,
			image->memDC,
			0, 0,
			image->width, image->height,
			image->transColor);
	}

	if (*pLocate >= image->width - static_cast<int>(WinInfo::Width))
	{
		*pLocate -= image->width;
	}

}

void RenderClass::DoScrollLayer(ScrollLayer layer, int scrollparam)
{
	if (layer == ScrollLayer::GROUND)
	{
		scroll_location_ground += scrollparam;
	}
	else if (layer == ScrollLayer::BACK01)
	{
		scroll_location_back01 += scrollparam;
	}
	else if (layer == ScrollLayer::BACK02)
	{
		scroll_location_back02 += scrollparam;
	}
	else
	{
		scroll_location_back03 += scrollparam;
	}
}

/// <summary>
/// 알파 이미지 출력
/// </summary>
void RenderClass::DrawAlphaImage(int imageID, int x, int y, int alpha, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	if (alpha == -1)
	{
		// 이미지를 피봇 값만큼 뺀 위치에서 지정된 알파값을 포함하여 출력한다.
		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			image->memDC,
			0, 0, image->width, image->height,
			image->blendf);
	}
	else
	{
		// 이미지를 피봇 값만큼 뺀 위치에 새로 입력된 알파값을 포함하여 출력한다.
		BLENDFUNCTION temp = image->blendf;
		temp.SourceConstantAlpha = alpha;

		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			image->memDC,
			0, 0, image->width, image->height,
			temp);
	}
}

/// <summary>
/// 투명이 포함된 알파 이미지 출력
/// </summary>
void RenderClass::DrawTransAlphaImage(int imageID, int x, int y, int alpha, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// Transparent -> Alpha Blend 2중 작업을 한다.
	// 2중 작업을 하기 위해 Back Buffer를 임시 Buffer에 옮겨 그린다.
	HDC tempDc = CreateCompatibleDC(memDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(memDC, windowSize.right, windowSize.bottom);
	SelectObject(tempDc, hBitmap);

	BitBlt(tempDc,
		0, 0, windowSize.right, windowSize.bottom,
		memDC,
		0, 0, SRCCOPY);

	// 옮겨진 임시 Buffer에 Transparent를 적용시킨 이미지를 그린다.
	DrawTransImage(imageID, x, y, tempDc);

	if (alpha == -1)
	{
		// 임시 Buffer에서 Transparent가 적용된 이미지 부분만 잘라서
		// Back Buffer에 지정된 알파 값을 포함하여 그린다.
		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			tempDc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			image->blendf);
	}
	else
	{
		// 임시 Buffer에서 Transparent가 적용된 이미지 부분만 잘라서
		// Back Buffer에 입력된 알파 값을 포함하여 그린다.
		BLENDFUNCTION temp = image->blendf;
		temp.SourceConstantAlpha = alpha;

		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			tempDc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			temp);
	}

	// 2중 작업을 위해 생성한 임시 핸들을 해제한다.
	DeleteObject(hBitmap);
	DeleteDC(tempDc);
}

/// <summary>
/// 숫자를 입력받아서 해당 위치에 숫자 이미지를 그리는 함수
/// 매개 변수로 입력되는 피봇을 중심으로 출력
/// </summary>
void RenderClass::DrawAlphaNumberImage(bool isDamage, int value, int x, int y, int alpha, HDC hdc)
{
	const ImageData* image;

	if (isDamage == true)
	{
		image = IMAGE_MANAGER->GetImage(IG_DAMAGE_NUMBER);
	}
	else
	{
		image = IMAGE_MANAGER->GetImage(IG_HEAL_NUMBER);
	}

	int temp = value;
	int digit = 0;
	int numWidth = image->width / 10;
	vector<char> num;

	do
	{
		num.push_back(temp % 10);
		digit++;
		temp /= 10;
	} while (temp != 0);

	if (alpha == 255)
	{
		for (int i = digit - 1; i >= 0; i--)
		{
			TransparentBlt(
				hdc,
				x - digit * (numWidth - 10) / 2 + (digit - i - 1) * (numWidth - 10), y - image->height / 2,
				numWidth, image->height,
				image->memDC,
				num[i] * numWidth, 0, numWidth, image->height,
				image->transColor);
		}
	}
	else
	{
		// Transparent -> Alpha Blend 2중 작업을 한다.
		// 2중 작업을 하기 위해 Back Buffer를 임시 Buffer에 옮겨 그린다.
		HDC tempDc = CreateCompatibleDC(memDC);
		HBITMAP hBitmap = CreateCompatibleBitmap(memDC, windowSize.right, windowSize.bottom);
		SelectObject(tempDc, hBitmap);

		BitBlt(tempDc,
			0, 0, windowSize.right, windowSize.bottom,
			memDC,
			0, 0, SRCCOPY);

		for (int i = digit - 1; i >= 0; i--)
		{
			TransparentBlt(
				tempDc,
				x - digit * (numWidth - 10) / 2 + (digit - i - 1) * (numWidth - 10), y - image->height / 2,
				numWidth, image->height,
				image->memDC,
				num[i] * numWidth, 0, numWidth, image->height,
				image->transColor);
		}

		// 임시 Buffer에서 Transparent가 적용된 이미지 부분만 잘라서
		// Back Buffer에 입력된 알파 값을 포함하여 그린다.
		BLENDFUNCTION tempBf = image->blendf;
		tempBf.SourceConstantAlpha = alpha;

		AlphaBlend(
			hdc,
			x - digit * (numWidth - 10) / 2, y - image->height / 2, (numWidth - 10) * digit + 10, image->height,
			tempDc,
			x - digit * (numWidth - 10) / 2, y - image->height / 2, (numWidth - 10) * digit + 10, image->height,
			tempBf);

		// 2중 작업을 위해 생성한 임시 핸들을 해제한다.
		DeleteObject(hBitmap);
		DeleteDC(tempDc);
	}
}

/// <summary>
/// 체력 UI용 흰 숫자 출력 함수, 우측 중앙이 피봇
/// </summary>
void RenderClass::DrawColorNumberImage(bool isWhite, int value, int x, int y, HDC hdc)
{
	const ImageData* image;

	if (isWhite == true)
	{
		image = IMAGE_MANAGER->GetImage(IG_WHITE_NUMBER);
	}
	else
	{
		image = IMAGE_MANAGER->GetImage(IG_GOLD_NUMBER);
	}

	int temp = value;
	int digit = 0;
	vector<char> num;

	do
	{
		num.push_back(temp % 10);
		digit++;
		temp /= 10;
	} while (temp != 0);

	for (int i = 0; i < digit; i++)
	{
		TransparentBlt(
			hdc,
			x - (i + 1) * image->width / 10, y,
			image->width / 10, image->height,
			image->memDC,
			num[i] * image->width / 10, 0, image->width / 10, image->height,
			image->transColor);
	}
}

/// <summary>
/// Bar 이미지를 출력한다. 100 percent = 1.0f
/// UnderBarID + 1 == FrontBarID가 성립되어야함
/// </summary>
void RenderClass::DrawBarImage(int underBarImageID, int x, int y, float percent, int alpha, HDC hdc)
{
	DrawTransImage(underBarImageID, x, y);

	const ImageData* const image = IMAGE_MANAGER->GetImage(underBarImageID + 1);

	// 이미지를 피봇 값만큼 뺀 위치에서 지정된 RGB값을 제외하고 출력한다.
	TransparentBlt(
		hdc,
		x - image->pivotX, y - image->pivotY,
		image->width * percent, image->height,
		image->memDC,
		0, 0, image->width * percent, image->height,
		image->transColor);
}