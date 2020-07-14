#pragma once
#include "Object.h"

class PistolShell : public Object
{
private:
	Sprite* _sprite;
	PhysicsBody* _physics;
	float _speed;
	int _dir;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void SetDirection(int dir) { _dir = dir; }
};

