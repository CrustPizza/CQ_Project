#include "stdafx.h"
#include "sceneManager.h"
#include "intro.h"
#include "battle.h"

Scene* Scene::SingletonObject = nullptr;
Scene* Scene::curScene = nullptr;
map<string, Scene*> Scene::sceneList;

Scene::Scene()
{
	cameraX = 0;
}

Scene::~Scene()
{
}

///// <summary>
///// 싱글톤 생성 및 반환
///// </summary>
//Scene* Scene::GetSingletonObject()
//{
//	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
//	if (SingletonObject == nullptr)
//	{
//		SingletonObject = new Scene();
//		SingletonObject->Init();
//	}
//
//	return SingletonObject->curScene;
//}
//
///// <summary>
///// 싱글톤 해제
///// </summary>
//void Scene::ReleaseSingleton()
//{
//	// 싱글톤 객체가 있다면 해제한다.
//	if (SingletonObject != nullptr)
//	{
//		SingletonObject->Release();
//		delete SingletonObject;
//		SingletonObject = nullptr;
//	}
//}

// 초기화와 해제를 생성자 및 소멸자에 안쓰는 이유
// 자식 클래스가 생성 및 소멸 되면서 부모 클래스의 생성자/소멸자를 호출하기 때문

/// <summary>
/// 필요한 모든 Scene을 등록하는 함수
/// </summary>
void Scene::Init()
{
	sceneList["Intro"] = (Scene*)new Intro;
	curScene = sceneList["Intro"];

	sceneList["Battle"] = (Scene*)new Battle;
}

/// <summary>
/// List에 등록된 모든 Scene을 해제한다.
/// </summary>
void Scene::Release()
{
	for (auto iter : sceneList)
	{
		if (iter.second != nullptr)
		{
			delete iter.second;
		}
	}
}

/// <summary>
/// Scene 전환
/// </summary>
void Scene::SceneChange(string sceneName)
{
	curScene = sceneList[sceneName];
	cameraX = 0;
}

void Scene::SceneBack()
{
	for (auto iter : sceneList)
	{
		if (curScene != iter.second)
		{
			continue;
		}

		if (iter.first == "Intro")
		{
			PostQuitMessage(0);
		}
		else if (iter.first == "Battle")
		{
			SCENE_MANAGER->SetChange();
		}

		break;
	}
}

void Scene::SetChange()
{
}

/// <summary>
/// 카메라 x값
/// </summary>
int Scene::GetCamera()
{
	return cameraX;
}

/// <summary>
/// 카메라를 인수의 5배만큼 이동 시킨다
/// </summary>
void Scene::MoveCamera(int x)
{
	cameraX += x;

	if (curScene == sceneList["Battle"])
	{
		RENDER_MANAGER->DoScrollLayer(ScrollLayer::GROUND, 5);
		RENDER_MANAGER->DoScrollLayer(ScrollLayer::BACK01, 3);
		RENDER_MANAGER->DoScrollLayer(ScrollLayer::BACK02, 4);
		RENDER_MANAGER->DoScrollLayer(ScrollLayer::BACK03, 5);
	}
}

/// <summary>
/// Update 호출을 위한 가상 함수
/// </summary>
void Scene::Update()
{

}

/// <summary>
/// Render 호출을 위한 가상 함수
/// </summary>
void Scene::Render()
{

}

/// <summary>
/// 싱글톤 생성 및 반환
/// </summary>
Scene* SceneSingleton::GetSingletonObject()
{
	// 싱글톤 객체가 없을 때만 생성하고 리턴한다.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Scene();
		SingletonObject->Init();
	}

	return SingletonObject->curScene;
}

/// <summary>
/// 싱글톤 해제
/// </summary>
void SceneSingleton::ReleaseSingleton()
{
	// 싱글톤 객체가 있다면 해제한다.
	if (SingletonObject != nullptr)
	{
		SingletonObject->Release();
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}