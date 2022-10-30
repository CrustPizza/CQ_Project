#pragma once
#include <map>
#include <vector>

using std::map;
using std::vector;

// image struct
struct ImageData
{
	HDC				memDC;
	HBITMAP			image;
	COLORREF		transColor;
	BLENDFUNCTION	blendf;

	// 가로, 세로 아마 2바이트면 충분할 것으로 생각됨
	int	width : 16;
	int	height : 16;
	int	pivotX : 16;
	int	pivotY : 16;
};

class Image
{
// Back Buffer Number
private:
	enum class BufferType {	BackBuffer = 0 };

// 변수
private:
	// Singleton
	static Image* SingletonObject;

	// 핸들
	HINSTANCE	hInstance;
	HWND		hWnd;
	HDC			hdc;

	// 이미지 저장 트리
	map<int, ImageData>	images;

// 함수
public:
	Image(const HINSTANCE& hInst, const HWND& hWnd);
	~Image();

	// Singleton
	static Image* const GetSingletonObject(const HINSTANCE& hInst = 0, const HWND& _hWnd = 0);
	static void	ReleaseSingleton();

	// BackBuffer DC
	const HDC GetMemDC();

private:
	// 기본 이미지 등록 함수
	void Init();

	// Back Buffer 비트맵 생성 함수
	void AddBackBuffer();

	// 이미지 데이터 저장하는 함수
	ImageData InputData(int imageID, int pivotX, int pivotY);
	ImageData InputDataFromJson(int imageID, int pivotX, int pivotY);

public:
	// 이미지 자료구조에 추가
	void AddImage(int imageID, int alpha = 255, bool alphaFormat = 0, int pivotX = 0, int pivotY = 0);
	void AddImageFromJson(int imageID, int alpha = 255, bool alphaFormat = 0, int pivotX = 0, int pivotY = 0);

	// 이미지 찾기
	const ImageData* const GetImage(int imageID);

	void Release();
};