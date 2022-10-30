/*
	Game institute of technology
	3rd Programming CQ Project Team
									21.11.15

		이원택, 이정호, 황상진, 김지훈, 오경민
											*/
#include "stdafx.h"
#include "sceneManager.h"
#include "option.h"

											// App 및 Window Class 이름
#define APP_NAME _T("CQ_PROJECT")

// 전역 핸들
HWND		g_hWnd;
HINSTANCE	g_hInstance;

// 윈도우 실제 크기
RECT		windowSize;

// 마우스 추적
TRACKMOUSEEVENT tme;

/// <summary>
/// 메인 윈도우 프로시저
/// </summary>
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC	hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
#ifdef DebugMode
	case WM_TIMER:
		RENDER_MANAGER->FPS();
		break;
#endif
	case WM_MOUSEMOVE:
		// 마우스가 윈도우 밖으로 나가면 이벤트를 발생시킨다.
		if (INPUT_MANAGER->isMouseInWindow == false)
		{
			INPUT_MANAGER->isMouseInWindow = true;
			tme.dwFlags = TME_LEAVE;
			TrackMouseEvent(&tme);
		}

		INPUT_MANAGER->SetMousePos(lParam);
		break;
	case WM_MOUSELEAVE:
		// 마우스가 윈도우 안으로 진입할 때 이벤트를 발생시킨다.
		INPUT_MANAGER->isMouseInWindow = false;
		tme.dwFlags = TME_HOVER;
		TrackMouseEvent(&tme);
		break;
	case WM_ACTIVATE:
		INPUT_MANAGER->isWindowActivated = LOWORD(wParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/// <summary>
/// 윈도우 클래스 생성 및 등록
/// </summary>
void WndClassRegister(WNDCLASS& wndClass)
{
	wndClass.lpszClassName = APP_NAME;
	wndClass.lpszMenuName = nullptr;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClass(&wndClass);
}

/// <summary>
/// 메인 윈도우 창 생성, 실패시 에러
/// </summary>
void CreateMainWnd(int nShowCmd)
{
	g_hWnd = CreateWindow(
		APP_NAME,
		APP_NAME,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		(int)WinInfo::X,
		(int)WinInfo::Y,
		(int)WinInfo::Width,
		(int)WinInfo::Height,
		nullptr,
		nullptr,
		g_hInstance,
		nullptr);

	// 생성에 실패했을 때 오류를 던진다.
	if (g_hWnd == nullptr)
	{
		throw "Error : Main Window not created";
	}

	// 윈도우에서 스크린 크기를 원하는 크기로 맞춘다.
	windowSize = { 0, 0, (int)WinInfo::Width, (int)WinInfo::Height };

	AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, false);

	SetWindowPos(g_hWnd, nullptr,
		(int)WinInfo::X, (int)WinInfo::Y,
		(windowSize.right - windowSize.left), (windowSize.bottom - windowSize.top),
		SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(g_hWnd, nShowCmd);
}

/// <summary>
/// 메인 함수
/// </summary>
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;
	WNDCLASS wndClass;
	g_hInstance = hInstance;

	WndClassRegister(wndClass);

	try
	{
		CreateMainWnd(nShowCmd);
	}
	catch (const TCHAR* message)
	{
		// 콘솔창에 에러 내용 출력
		cerr << message << endl;
	}

	// 초기화 작업
	srand(time(nullptr));
	HDC hdc = GetDC(g_hWnd);
	Image::GetSingletonObject(hInstance, g_hWnd);

#ifdef DebugMode
	SetTimer(g_hWnd, 1, 1000, nullptr);
#endif

	// 프레임 조절
	LARGE_INTEGER curTime;
	LARGE_INTEGER prevTime;
	LARGE_INTEGER frequency;
	double frameMs = 1000 / (double)WinInfo::Frame;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&prevTime);

	// 마우스 추적
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = g_hWnd;
	tme.dwHoverTime = 0.001f;
	TrackMouseEvent(&tme);

	while (TRUE)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			// 문자 입력이 필요할 때 사용
			//TranslateMessage(&msg);

			DispatchMessage(&msg);
			continue;
		}

		try
		{
			if (QueryPerformanceCounter(&curTime) == 0)
			{
				throw "Error : QueryPerformanceCounter is Failed";
			}

			if (((double)(curTime.QuadPart - prevTime.QuadPart) / (double)frequency.QuadPart) * 1000 >= frameMs)
			{
				prevTime = curTime;

				if (OPTION_MANAGER->Update() == false)
				{
					SCENE_MANAGER->Update();
				}

				SCENE_MANAGER->Render();
				OPTION_MANAGER->Render();
				RENDER_MANAGER->Render(hdc);
			}
		}
		catch (const TCHAR* message)
		{
			cerr << message << endl;
		}
	}

#ifdef DebugMode
	KillTimer(g_hWnd, 0);
#endif

	DeleteDC(hdc);

	// 싱글톤 해제
	Image::ReleaseSingleton();
	RenderClass::ReleaseSingleton();
	DataTable::ReleaseSingleton();
	Input::ReleaseSingleton();
	GameManager::ReleaseSingleton();
	SceneSingleton::ReleaseSingleton();
	Option::ReleaseSingleton();

	return (int)msg.wParam;
}