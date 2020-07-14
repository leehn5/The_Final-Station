#pragma once

class Ability
{
private:
	float _curHP, _maxHP;
	float _atk, _speed;
	bool _isDead;

public:
	Ability() : _curHP(0.f), _maxHP(0.f), _atk(0.0f), _speed(0.f), _isDead(false) {};
	Ability(float curHP, float maxHP, float atk, float speed)
		: _curHP(curHP), _maxHP(maxHP), _atk(atk), _speed(speed), _isDead(false) {};

	float GetCurrentHP() { return _curHP; }
	float GetMaxHP() { return _maxHP; }
	float GetAttackPoint() { return _atk; }
	float GetSpeed() { return _speed; }

	void setHP(float value);
	void DamageHP(float value);
	void HealHP(float value);

	void SetSpeed(float speed) { _speed = speed; }

	bool IsDead();
};

