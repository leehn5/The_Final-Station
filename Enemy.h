#pragma once
#include "Object.h"

class EnemyState;
class Ability;

typedef enum class EnemyDirection
{
	Left = -1,
	None,
	Right,
} E_Dir;

class Enemy : public Object
{
protected:

	Sprite* _sprite;
	PhysicsBody* _physics;
	shared_ptr<Ability> _ability;
	shared_ptr<EnemyState> _state;
	EnemyDirection _dir;
	float _fps;

	Object* _head;

public:
	Enemy() {}
	virtual ~Enemy() {}

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void SetDirection(E_Dir dir) { _dir = dir; }
	E_Dir GetDirection() { return _dir; }

	Sprite* GetSprite() { return _sprite; }
	PhysicsBody* GetPhysics() { return _physics; }
	shared_ptr<Ability> GetAbility() { return _ability; }
	
	void ChangeState(shared_ptr<EnemyState> state);
	
	shared_ptr<EnemyState> GetState() { return _state; }
	Object* GetHead() { return _head; }

	void SetFPS(float fps) { _fps = fps; }
	float GetFPS() { return _fps; }
};

