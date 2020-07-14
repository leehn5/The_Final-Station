#pragma once
#include "Scene.h"
#include "BspNode.h"

class BSP_TestScene : public Scene
{
private:
	int count;

	vector<BspNode> _nodes;

public:
	virtual void Init();
	virtual void Update();
	virtual void Release();
	virtual void Render();
	
	void SpacePartitioning(int count);
};

