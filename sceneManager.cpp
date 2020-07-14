#include "stdafx.h"
#include "sceneManager.h"
#include "Scene.h"
#include"StartScene.h"

sceneManager::sceneManager()
{
}

sceneManager::~sceneManager()
{
}

Scene* sceneManager::_currentScene = NULL;

HRESULT sceneManager::init()
{
    if (!_isFirstInit) return S_OK;
    addScene("PG", new StartScene);
    changeScene("PG");
    return S_OK;
}

void sceneManager::release()
{
    mapSceneIter miSceneList = _mSceneList.begin();

    for (; miSceneList != _mSceneList.end();)
    {
        if (miSceneList->second != NULL)
        {
            if (miSceneList->second == _currentScene) miSceneList->second->Release();
            SAFE_DELETE(miSceneList->second);
            miSceneList = _mSceneList.erase(miSceneList);
        }
        else ++miSceneList;
    }

    _mSceneList.clear();
}

void sceneManager::update()
{
    if (_currentScene) _currentScene->Update();
}

void sceneManager::render()
{
    if (_currentScene) _currentScene->Render();
}

Scene* sceneManager::addScene(string sceneName, Scene* scene)
{
    if (!scene) return nullptr;

    auto iter = _mSceneList.find(sceneName);

    if (iter != _mSceneList.end())
    {
        //if (iter->second)
        //   iter->second->Release();
        _mSceneList.erase(iter);
    }

    _mSceneList.insert(make_pair(sceneName, scene));

    return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
    mapSceneIter find = _mSceneList.find(sceneName);

    if (find == _mSceneList.end()) return E_FAIL;
    if (find->second == _currentScene) return S_OK;


    //어떤 씬의 정보가 처음에 들어있기 때문에 릴리즈 시켜줘라
    if (_currentScene != nullptr)
    {
        _currentScene->Release();
    }

    //현재 씬에 바꾸려는 씬을 담는다
    _currentScene = find->second;
    _currentScene->Init();


    return S_OK;
}