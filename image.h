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

	// ����, ���� �Ƹ� 2����Ʈ�� ����� ������ ������
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

// ����
private:
	// Singleton
	static Image* SingletonObject;

	// �ڵ�
	HINSTANCE	hInstance;
	HWND		hWnd;
	HDC			hdc;

	// �̹��� ���� Ʈ��
	map<int, ImageData>	images;

// �Լ�
public:
	Image(const HINSTANCE& hInst, const HWND& hWnd);
	~Image();

	// Singleton
	static Image* const GetSingletonObject(const HINSTANCE& hInst = 0, const HWND& _hWnd = 0);
	static void	ReleaseSingleton();

	// BackBuffer DC
	const HDC GetMemDC();

private:
	// �⺻ �̹��� ��� �Լ�
	void Init();

	// Back Buffer ��Ʈ�� ���� �Լ�
	void AddBackBuffer();

	// �̹��� ������ �����ϴ� �Լ�
	ImageData InputData(int imageID, int pivotX, int pivotY);
	ImageData InputDataFromJson(int imageID, int pivotX, int pivotY);

public:
	// �̹��� �ڷᱸ���� �߰�
	void AddImage(int imageID, int alpha = 255, bool alphaFormat = 0, int pivotX = 0, int pivotY = 0);
	void AddImageFromJson(int imageID, int alpha = 255, bool alphaFormat = 0, int pivotX = 0, int pivotY = 0);

	// �̹��� ã��
	const ImageData* const GetImage(int imageID);

	void Release();
};