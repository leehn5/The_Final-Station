#pragma once

class Enemy;
class Player;

class EnemyState abstract
{
protected:
	Enemy* _enemy;
	string _state;
	Player* _player;
	float _timer;

public:
	EnemyState() {}
	EnemyState(Enemy* obj) : _enemy(obj) {}

	virtual void Enter() {}
	virtual void Update() {}
	virtual void Exit() {}

	string GetState() { return _state; }
};

// Enemy Idle
class EnemyIdle : public EnemyState
{
private:
	float _distance;

public:
	EnemyIdle(Enemy* obj) : EnemyState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

// Enemy Move
class EnemyMove : public EnemyState
{
private:
	Vector2 _v2Dis;

public:
	EnemyMove(Enemy* obj) : EnemyState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

// Enemy Attack
class EnemyAttack : public EnemyState
{
private:
	Object* _atkArea;

public:
	EnemyAttack(Enemy* obj) : EnemyState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

class EnemyDamage : public EnemyState
{
private:

public:
	EnemyDamage(Enemy* obj) : EnemyState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

// Enemy Dead
class EnemyDead : public EnemyState
{
private:

public:
	EnemyDead(Enemy* obj) : EnemyState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

// Enemy Dead Head shot
class EnemyDeadHeadshot : public EnemyState
{
private:

public:
	EnemyDeadHeadshot(Enemy* obj) : EnemyState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};