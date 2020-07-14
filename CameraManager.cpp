#include "stdafx.h"
#include "CameraManager.h"


void CameraManager::Update()
{
	main->Update();
	for (Camera* c : cameras)
		c->Update();
}

void CameraManager::Release()
{
	for (int i = cameras.size() - 1; i >= 0; i--)
	{
		SafeRelease(cameras[i]);
		cameras.erase(cameras.begin() + i);
	}
	main = nullptr;
}

void CameraManager::AddCamera(Camera * camera)
{
	camera->init();
	cameras.push_back(camera);
}
