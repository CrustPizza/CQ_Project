#pragma once

// 전역으로 사용할 헤더만 등록해주세요

// 버전
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN

// 윈도우
#include <Windows.h>

// 리소스
#include "resource.h"

// 싱글톤
#include "image.h"
#include "render.h"
#include "tableData.h"
#include "input.h"
#include "gamemanager.h"
#include "sceneManager.h"
#include "option.h"
#include "sound.h"

// 기타 헤더
#include <tchar.h>
#include <iostream>
#include "Character.h"
#include "Collider.h"
#include "skill.h"

// 추가 라이브러리
#pragma comment(lib, "Msimg32.lib")

// namespace
using std::cerr;
using std::endl;

// Singleton 객체
#ifndef SINGLETON_DEFINED
#define SINGLETON_DEFINED

#define IMAGE_MANAGER	Image::GetSingletonObject()
#define RENDER_MANAGER	RenderClass::GetSingletonObject()
#define TABLE_MANAGER	DataTable::GetSingletonObject()
#define INPUT_MANAGER	Input::GetSingletonObject()
#define GAME_MANAGER	GameManager::GetSingletonObject()
#define SCENE_MANAGER	SceneSingleton::GetSingletonObject()
#define OPTION_MANAGER	Option::GetSingletonObject()
#define SOUND_MANAGER	CSound::GetSingletonObject()

#endif

// 디버거용
#ifdef DebugMode

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#endif DebugMode

// 윈도우 시작 출력 위치와 가로 세로 크기
enum class WinInfo
{
	X = 50,
	Y = 50,
	Width = 1920,
	Height = 900,
	Frame = 60
};

// 실제 윈도우 사이즈
extern RECT windowSize;