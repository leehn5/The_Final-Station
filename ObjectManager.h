#pragma once
#include <unordered_map>
#include <map>

class ObjectManager
{
	typedef unordered_map<string, vector<Object*>> mObjMgr; 
	typedef unordered_map<string, vector<Object*>>::iterator mObjMgrIter;
	typedef vector<Object*> vObjMgr;
	typedef multimap<int, Object*> z_oderList;
	mObjMgr objMgr;

	ObjectManager();

public:
	static ObjectManager* GetInstace();
	~ObjectManager();

	HRESULT Init();
	void Release();
	void SceneRelease(string sceneName);
	void AddObject(string sceneName, Object* obj);
	void Update(string sceneName);
	void Render(string sceneName);
	vObjMgr GetSceneObject(string sceneName) { return objMgr.find(sceneName)->second; }
};

