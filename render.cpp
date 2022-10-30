#include "stdafx.h"
#include "render.h"

RenderClass* RenderClass::SingletonObject = nullptr;
HDC	RenderClass::memDC = nullptr;

/// <summary>
/// ������
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
/// �Ҹ���
/// </summary>
RenderClass::~RenderClass()
{

}

/// <summary>
/// �̱��� ���� �� ��ȯ
/// </summary>
RenderClass* const RenderClass::GetSingletonObject()
{
	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new RenderClass();
	}

	return SingletonObject;
}

/// <summary>
/// �̱��� ����
/// </summary>
void RenderClass::ReleaseSingleton()
{
	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
	if (SingletonObject != nullptr)
	{
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}

/// <summary>
/// Buffer ���� �Լ�
/// </summary>
void RenderClass::Render(HDC hdc)
{
	// Back Buffer�� ���� hdc�� �����Ѵ�.
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
/// Debug�� ��Ʈ�ڽ�
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
/// �⺻���� �̹��� ���
/// </summary>
void RenderClass::DrawImage(int imageID, int x, int y, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// �̹����� �Ǻ� ����ŭ �� ��ġ�� ��½�Ų��.
	BitBlt(
		hdc,
		x - image->pivotX, y - image->pivotY,
		image->width, image->height,
		image->memDC,
		0, 0, SRCCOPY);
}

/// <summary>
/// ����� �̹��� ���
/// </summary>
void RenderClass::DrawTransImage(int imageID, int x, int y, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// �̹����� �Ǻ� ����ŭ �� ��ġ���� ������ RGB���� �����ϰ� ����Ѵ�.
	TransparentBlt(
		hdc,
		x - image->pivotX, y - image->pivotY,
		image->width, image->height,
		image->memDC,
		0, 0, image->width, image->height,
		image->transColor);
}

/// <summary>
/// ������ �̹��� ���
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
/// ������ ���Ե� ������ �̹��� ���
/// </summary>
void RenderClass::DrawTransScaleImage(int imageID, int x, int y, float size, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// Scale -> Transparent 2�� �۾��� �Ѵ�.
	// 2�� �۾��� ���� Scale ������ ��ŭ�� �ع����� �����.
	HDC tempDc = CreateCompatibleDC(image->memDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(image->memDC, image->width * size, image->height * size);
	SelectObject(tempDc, hBitmap);

	// �ع����� Ȯ��/��ҽ��� �׸���.
	StretchBlt(
		tempDc,
		0, 0,
		image->width * size, image->height * size,
		image->memDC,
		0, 0, image->width, image->height, SRCCOPY);

	// Back Buffer�� �Ǻ� ���� �� ��ġ�� ������ RGB���� �����ϰ� ����Ѵ�.
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
/// �̹��� ��ũ�Ѹ�
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

	// ����, ���� �̹������� �ٱ����� �̵����� ���
	if (*pLocate < 0)
	{

		// ��������
		BitBlt(
			hdc,										// �̹����� ����� ��ġ�� �ڵ�
			x - image->pivotX, y - image->pivotY,		// �̹����� ����� ��ġ�� x,y ��ǥ
			-*pLocate, image->height,					// ������ �̹������� �߶� �ʺ�, ����
			image->memDC,								// �̹����� �ڵ�
			image->width + *pLocate, 0,					// ������ �̹����� �������� x,y ��ǥ
			SRCCOPY);


		// ��������
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
/// ������ ���Ե� �̹��� ��ũ�Ѹ�
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

	// ����, ���� �̹������� �ٱ����� �̵����� ���
	if (*pLocate < 0)
	{

		// ��������
		TransparentBlt(
			hdc,										// �̹����� ����� ��ġ�� �ڵ�
			x - image->pivotX, y - image->pivotY,		// �̹����� ����� ��ġ�� x,y ��ǥ
			-*pLocate, image->height,					// ����� �̹����� �ʺ�, ����
			image->memDC,								// �̹����� �ڵ�
			image->width + *pLocate, 0,					// ������ �̹����� �������� x,y ��ǥ
			-*pLocate, image->height,					// ������ �̹������� �߶� �ʺ�, ����
			image->transColor);							// �����


		// ��������
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
/// ���� �̹��� ���
/// </summary>
void RenderClass::DrawAlphaImage(int imageID, int x, int y, int alpha, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	if (alpha == -1)
	{
		// �̹����� �Ǻ� ����ŭ �� ��ġ���� ������ ���İ��� �����Ͽ� ����Ѵ�.
		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			image->memDC,
			0, 0, image->width, image->height,
			image->blendf);
	}
	else
	{
		// �̹����� �Ǻ� ����ŭ �� ��ġ�� ���� �Էµ� ���İ��� �����Ͽ� ����Ѵ�.
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
/// ������ ���Ե� ���� �̹��� ���
/// </summary>
void RenderClass::DrawTransAlphaImage(int imageID, int x, int y, int alpha, HDC hdc)
{
	const ImageData* const image = IMAGE_MANAGER->GetImage(imageID);

	// Transparent -> Alpha Blend 2�� �۾��� �Ѵ�.
	// 2�� �۾��� �ϱ� ���� Back Buffer�� �ӽ� Buffer�� �Ű� �׸���.
	HDC tempDc = CreateCompatibleDC(memDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(memDC, windowSize.right, windowSize.bottom);
	SelectObject(tempDc, hBitmap);

	BitBlt(tempDc,
		0, 0, windowSize.right, windowSize.bottom,
		memDC,
		0, 0, SRCCOPY);

	// �Ű��� �ӽ� Buffer�� Transparent�� �����Ų �̹����� �׸���.
	DrawTransImage(imageID, x, y, tempDc);

	if (alpha == -1)
	{
		// �ӽ� Buffer���� Transparent�� ����� �̹��� �κи� �߶�
		// Back Buffer�� ������ ���� ���� �����Ͽ� �׸���.
		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			tempDc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			image->blendf);
	}
	else
	{
		// �ӽ� Buffer���� Transparent�� ����� �̹��� �κи� �߶�
		// Back Buffer�� �Էµ� ���� ���� �����Ͽ� �׸���.
		BLENDFUNCTION temp = image->blendf;
		temp.SourceConstantAlpha = alpha;

		AlphaBlend(
			hdc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			tempDc,
			x - image->pivotX, y - image->pivotY, image->width, image->height,
			temp);
	}

	// 2�� �۾��� ���� ������ �ӽ� �ڵ��� �����Ѵ�.
	DeleteObject(hBitmap);
	DeleteDC(tempDc);
}

/// <summary>
/// ���ڸ� �Է¹޾Ƽ� �ش� ��ġ�� ���� �̹����� �׸��� �Լ�
/// �Ű� ������ �ԷµǴ� �Ǻ��� �߽����� ���
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
		// Transparent -> Alpha Blend 2�� �۾��� �Ѵ�.
		// 2�� �۾��� �ϱ� ���� Back Buffer�� �ӽ� Buffer�� �Ű� �׸���.
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

		// �ӽ� Buffer���� Transparent�� ����� �̹��� �κи� �߶�
		// Back Buffer�� �Էµ� ���� ���� �����Ͽ� �׸���.
		BLENDFUNCTION tempBf = image->blendf;
		tempBf.SourceConstantAlpha = alpha;

		AlphaBlend(
			hdc,
			x - digit * (numWidth - 10) / 2, y - image->height / 2, (numWidth - 10) * digit + 10, image->height,
			tempDc,
			x - digit * (numWidth - 10) / 2, y - image->height / 2, (numWidth - 10) * digit + 10, image->height,
			tempBf);

		// 2�� �۾��� ���� ������ �ӽ� �ڵ��� �����Ѵ�.
		DeleteObject(hBitmap);
		DeleteDC(tempDc);
	}
}

/// <summary>
/// ü�� UI�� �� ���� ��� �Լ�, ���� �߾��� �Ǻ�
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
/// Bar �̹����� ����Ѵ�. 100 percent = 1.0f
/// UnderBarID + 1 == FrontBarID�� �����Ǿ����
/// </summary>
void RenderClass::DrawBarImage(int underBarImageID, int x, int y, float percent, int alpha, HDC hdc)
{
	DrawTransImage(underBarImageID, x, y);

	const ImageData* const image = IMAGE_MANAGER->GetImage(underBarImageID + 1);

	// �̹����� �Ǻ� ����ŭ �� ��ġ���� ������ RGB���� �����ϰ� ����Ѵ�.
	TransparentBlt(
		hdc,
		x - image->pivotX, y - image->pivotY,
		image->width * percent, image->height,
		image->memDC,
		0, 0, image->width * percent, image->height,
		image->transColor);
}