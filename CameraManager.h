#pragma once
#include "singletonBase.h"
#include "Camera.h"
class CameraManager : public singletonBase<CameraManager>
{
private:
	Camera* main;
	vector<Camera*> cameras;

public:

	void Update();
	void Release();
	Camera* GetMainCamera() { return main; }
	void SetMainCamera(Camera* main) { this->main = main; }
	void AddCamera(Camera* camera);


};

