#pragma once
#include "Scene.h"
#include"ParticleManager.h"
class TestScene :	public Scene
{
	//vector<Object*> test;
	//list<Object*>test2;
	//Object* bullet;
	//int _count=0;
	//float _timer=0;
	ParticleManager* mgr;
public:
	virtual void Init();			//초기화 전용 함수
	virtual void Update();			//연산 전용 함수
	virtual void Render();
};

