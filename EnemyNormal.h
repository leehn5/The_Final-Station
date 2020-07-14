#pragma once
#include "Enemy.h"

class EnemyNormal : public Enemy
{
private:

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

