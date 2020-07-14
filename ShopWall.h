#pragma once
#include "Object.h"
#include "PhysicsBody.h"

class ShopWall :public Object
{
private:
	PhysicsBody* _physics;

public:

	virtual void Init(Vector2 pos, Vector2 scale);

};

