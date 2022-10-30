#pragma once
#include "option.h"

#define IMAGE_MANAGER	Image::GetSingletonObject()
//#define DebugMode

enum class ScrollLayer 
{
	GROUND,
	BACK01,
	BACK02,
	BACK03
};

class RenderClass
{
	// 변수
private:
	// Singleton
	static RenderClass* SingletonObject;

	// Back Buffer
	static HDC	memDC;

	// Scroll
	int scroll_location_ground;
	int scroll_location_back01;
	int scroll_location_back02;
	int scroll_location_back03;

#ifdef DebugMode
	// frame
	char frame;
	double total;
	int time;
#endif

	// 함수
public:
	RenderClass();
	~RenderClass();

	// Singleton
	static RenderClass* const GetSingletonObject();
	static void	ReleaseSingleton();

	// Back Buffer -> hdc
	void Render(HDC hdc);

#ifdef DebugMode
	// Debug Function
	void HitBox(POINT dest, POINT src, COLORREF color, HDC hdc = memDC);
	void FPS();
#endif

	// 이미지 그리기 함수
	void DrawImage(int imageID, int x, int y, HDC hdc = memDC);
	void DrawTransImage(int imageID, int x, int y, HDC hdc = memDC);

	// 스케일 이미지
	void DrawScaleImage(int imageID, int x, int y, float size, HDC hdc = memDC);
	void DrawTransScaleImage(int imageID, int x, int y, float size, HDC hdc = memDC);
	
	// 알파 이미지
	void DrawAlphaImage(int imageID, int x, int y, int alpha = -1, HDC hdc = memDC);
	void DrawTransAlphaImage(int imageID, int x, int y, int alpha = -1, HDC hdc = memDC);

	// Number 이미지
	void DrawAlphaNumberImage(bool isDamage, int value, int x, int y, int alpha = 255, HDC hdc = memDC);
	void DrawColorNumberImage(bool isWhite, int value, int x, int y, HDC hdc = memDC);

	// Bar 이미지
	void DrawBarImage(int underBarImageID, int x, int y, float percent, int alpha = -1, HDC hdc = memDC);

	// 스크롤 되는 이미지 그리기 함수
	void DrawScrollImage(int imageID, int x, int y, ScrollLayer layer, HDC hdc = memDC);
	void DrawScrollTransImage(int imageID, int x, int y, ScrollLayer layer, HDC hdc = memDC);

	// 스크롤 시키는 함수
	void DoScrollLayer(ScrollLayer layer, int scrollparam);
};