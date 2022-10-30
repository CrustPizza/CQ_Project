#pragma once

// �������� ����� ����� ������ּ���

// ����
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN

// ������
#include <Windows.h>

// ���ҽ�
#include "resource.h"

// �̱���
#include "image.h"
#include "render.h"
#include "tableData.h"
#include "input.h"
#include "gamemanager.h"
#include "sceneManager.h"
#include "option.h"
#include "sound.h"

// ��Ÿ ���
#include <tchar.h>
#include <iostream>
#include "Character.h"
#include "Collider.h"
#include "skill.h"

// �߰� ���̺귯��
#pragma comment(lib, "Msimg32.lib")

// namespace
using std::cerr;
using std::endl;

// Singleton ��ü
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

// ����ſ�
#ifdef DebugMode

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#endif DebugMode

// ������ ���� ��� ��ġ�� ���� ���� ũ��
enum class WinInfo
{
	X = 50,
	Y = 50,
	Width = 1920,
	Height = 900,
	Frame = 60
};

// ���� ������ ������
extern RECT windowSize;