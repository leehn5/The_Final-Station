#pragma once
#include "Scene.h"

#define LOADINGMAX 1000


class loadingScene : public Scene
{
private:
	Graphic* _background;
	float count;
	int frameX;

public:
	loadingScene();
	~loadingScene();

	int _currentCount;

	HRESULT init();
	void release();
	void update();
	void render();
};

static DWORD CALLBACK threadFunction(LPVOID lpParameter);