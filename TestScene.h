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
	virtual void Init();			//�ʱ�ȭ ���� �Լ�
	virtual void Update();			//���� ���� �Լ�
	virtual void Render();
};

