#pragma once
#include "Object.h"

class InteractionObject : public Object
{
protected:
	Sprite* _sprite;
	PhysicsBody* _physics;

public:
	virtual void Init();
	virtual void Update();

	Sprite* GetSprite() { return _sprite; }
	PhysicsBody* GetPhysics() { return _physics; }

	virtual void Interaction();

};

