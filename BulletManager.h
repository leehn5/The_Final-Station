#pragma once
#include "Object.h"

class PistolShell;
class Player;

class Bullet : public Object
{
private:
	Sprite* _sprite;
	PhysicsBody* _physics;
	float _speed;
	float _atk;
	Vector2 _startPos;
	bool _isFire;

public:
	virtual void Init();
	virtual void Update();
	void SetPhysicsBody();

	void SetSpeed(float speed) { _speed = speed; }
	float GetSpeed() { return _speed; }

	void SetStartPosition(Vector2 pos) { _startPos = pos; }
	Vector2 GetStartPosition() { return _startPos; }

	void SetAttackPoint(float atk) { _atk = atk; }
	float GetAttackPoint() { return _atk; }

	void SetIsFire(bool isFire) { _isFire = isFire; }
	bool GetIsFire() { return _isFire; }

	PhysicsBody* GetPhysics() { return _physics; }
};

class BulletManager
{
private:
	Player* _player;
	list<Bullet*> _vBullet;
	int _dir;
	float _atk;
	PistolShell* _pistolShell;

public:
	void Init();
	void Release();
	void Update();
	void Render();

	void Fire(Vector2 pos, float speed, float angle, float atk, int dir);
	void Move();
	void AddBullet(int value);
	void Reload(int& magazine, int& fireCount);
	void RemoveBullet(Bullet* b);

	float GetAttackPoint() { return _atk; }
};