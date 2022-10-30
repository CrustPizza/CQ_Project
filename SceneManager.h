#pragma once

class Scene
{
	// 변수
protected:
	// Singleton
	static Scene* SingletonObject;

	// Scene
	static Scene* curScene;
	static map<string, Scene*> sceneList;

	// Camera
	int cameraX;

	// 함수
public:
	Scene();
	~Scene();

	// Singleton
	//static Scene* GetSingletonObject();
	//static void	ReleaseSingleton();

	// Scene 초기화
	void Init();

	// Scene 해제
	void Release();

	// 화면 전환
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

// Singleton의 중복 호출을 막는 실험용 클래스
class SceneSingleton : private Scene
{
public:
	// Singleton
	static Scene* GetSingletonObject();
	static void	ReleaseSingleton();
};