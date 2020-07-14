#pragma once
#include <map>

class Scene;

class sceneManager
{
public:
	typedef map<string, Scene*>				mapSceneList;
	typedef map<string, Scene*>::iterator	mapSceneIter;

private:
	static Scene* _currentScene;
	mapSceneList _mSceneList;
	bool _isFirstInit = true;

	sceneManager();

public:
	static sceneManager* GetInstance()
	{
		static sceneManager* instance = new sceneManager();
		return instance;
	}

	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	Scene* addScene(string sceneName, Scene* scene);

	HRESULT changeScene(string sceneName);
	Scene* GetNowScene() { return _currentScene; }
};

