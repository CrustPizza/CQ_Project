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
///// �̱��� ���� �� ��ȯ
///// </summary>
//Scene* Scene::GetSingletonObject()
//{
//	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
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
///// �̱��� ����
///// </summary>
//void Scene::ReleaseSingleton()
//{
//	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
//	if (SingletonObject != nullptr)
//	{
//		SingletonObject->Release();
//		delete SingletonObject;
//		SingletonObject = nullptr;
//	}
//}

// �ʱ�ȭ�� ������ ������ �� �Ҹ��ڿ� �Ⱦ��� ����
// �ڽ� Ŭ������ ���� �� �Ҹ� �Ǹ鼭 �θ� Ŭ������ ������/�Ҹ��ڸ� ȣ���ϱ� ����

/// <summary>
/// �ʿ��� ��� Scene�� ����ϴ� �Լ�
/// </summary>
void Scene::Init()
{
	sceneList["Intro"] = (Scene*)new Intro;
	curScene = sceneList["Intro"];

	sceneList["Battle"] = (Scene*)new Battle;
}

/// <summary>
/// List�� ��ϵ� ��� Scene�� �����Ѵ�.
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
/// Scene ��ȯ
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
/// ī�޶� x��
/// </summary>
int Scene::GetCamera()
{
	return cameraX;
}

/// <summary>
/// ī�޶� �μ��� 5�踸ŭ �̵� ��Ų��
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
/// Update ȣ���� ���� ���� �Լ�
/// </summary>
void Scene::Update()
{

}

/// <summary>
/// Render ȣ���� ���� ���� �Լ�
/// </summary>
void Scene::Render()
{

}

/// <summary>
/// �̱��� ���� �� ��ȯ
/// </summary>
Scene* SceneSingleton::GetSingletonObject()
{
	// �̱��� ��ü�� ���� ���� �����ϰ� �����Ѵ�.
	if (SingletonObject == nullptr)
	{
		SingletonObject = new Scene();
		SingletonObject->Init();
	}

	return SingletonObject->curScene;
}

/// <summary>
/// �̱��� ����
/// </summary>
void SceneSingleton::ReleaseSingleton()
{
	// �̱��� ��ü�� �ִٸ� �����Ѵ�.
	if (SingletonObject != nullptr)
	{
		SingletonObject->Release();
		delete SingletonObject;
		SingletonObject = nullptr;
	}
}