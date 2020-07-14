#include "stdafx.h"
#include "ObjectManager.h"
#include"Object.h"
#include"Transform.h"

ObjectManager::ObjectManager()
{
}

ObjectManager* ObjectManager::GetInstace()
{
	static ObjectManager* instance = new ObjectManager();
	return instance;
}

ObjectManager::~ObjectManager()
{
}

HRESULT ObjectManager::Init()
{
	return S_OK;
}

void ObjectManager::Release()
{
	mObjMgrIter iter = objMgr.begin();
	if (iter == objMgr.end())return;

	for (; iter != objMgr.end(); ++iter)
	{
		if (!iter->second.size())continue;

		for (int i = iter->second.size() - 1; i <= 0; i--)
		{
			SAFE_OBJECT_RELEASE(iter->second[i]);
		}
		iter->second.clear();
	}
	objMgr.clear();
}

void ObjectManager::SceneRelease(string sceneName)
{
	mObjMgrIter iter = objMgr.find(sceneName);
	if (iter == objMgr.end())return;
	for (int i = iter->second.size() - 1; i <= 0; i--)
	{
		SAFE_OBJECT_RELEASE(iter->second[i]);
	}
	iter->second.clear();
}


void ObjectManager::AddObject(string sceneName, Object * obj)
{
	//objMgr 사이즈가 0이라면 새로 만들어서 넣어주기
	if (!objMgr.size())
	{
		vObjMgr a;
		a.push_back(obj);
		objMgr.insert(make_pair(sceneName, a));
		return;
	}

	mObjMgrIter iter = objMgr.find(sceneName);

	if (iter != objMgr.end())
	{
		for (Object* i : iter->second)
			if (i == obj)return;

		iter->second.push_back(obj);
		return;
	}
	else return;
	//해당 씬이 존재하지 않는다면 새로 만들어서 맵에 추가

	vObjMgr a;
	a.push_back(obj);
	objMgr.insert(make_pair(sceneName, a));
}

void ObjectManager::Update(string sceneName)
{
	vObjMgr vSceneObjec;
	mObjMgrIter iter = objMgr.find(sceneName);

	vSceneObjec = iter->second;

	if (iter == objMgr.end()) return;

	

	for (Object* i : vSceneObjec)
	{
		if(i->GetAllowsUpdate())
			i->Update();
	}

}

void ObjectManager::Render(string sceneName)
{
	vObjMgr vSceneObjec;
	mObjMgrIter iter = objMgr.find(sceneName);

	if (iter == objMgr.end()) return;

	vSceneObjec = iter->second;


	z_oderList z_List;

	for (Object* vIter : vSceneObjec)
	{
		z_List.insert(make_pair(vIter->GetTrans()->bottomPos.y, vIter));
	}

	for (auto i : z_List)
	{
		i.second->Render();
	}
}
