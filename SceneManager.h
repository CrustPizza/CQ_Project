#pragma once

class Scene
{
	// ����
protected:
	// Singleton
	static Scene* SingletonObject;

	// Scene
	static Scene* curScene;
	static map<string, Scene*> sceneList;

	// Camera
	int cameraX;

	// �Լ�
public:
	Scene();
	~Scene();

	// Singleton
	//static Scene* GetSingletonObject();
	//static void	ReleaseSingleton();

	// Scene �ʱ�ȭ
	void Init();

	// Scene ����
	void Release();

	// ȭ�� ��ȯ
	void SceneChange(string sceneName);
	void SceneBack();
	virtual void SetChange();

	// Camera
	int	GetCamera();
	void MoveCamera(int x);

	// Update & Render
	virtual void Update();
	virtual void Render();
};

// Singleton�� �ߺ� ȣ���� ���� ����� Ŭ����
class SceneSingleton : private Scene
{
public:
	// Singleton
	static Scene* GetSingletonObject();
	static void	ReleaseSingleton();
};