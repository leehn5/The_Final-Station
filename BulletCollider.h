#pragma once
#include "Collider.h"

class BulletCollider : public Collider
{
public:
	virtual void CollisionBegin(void* obj);
	virtual void CollisionPreSolve(void* obj);
	virtual void CollisionEnd(void* obj);
};

