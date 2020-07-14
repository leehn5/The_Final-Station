#pragma once
#include "Object.h"

enum class NPC_State
{
	Idle,
	Dead,
};

enum class RewardType
{
	Money,
	Ammo,
	None,
};

struct tagReward
{
	RewardType type;
	int count;
};

class NPC : public Object
{
protected:
	float _hpDamageSpeed;
	float _hungerSpeed;
	float _changeStateTime;
	float _frame;
	float _deadTime;
	float _acceleration;

	float _curHP;
	float _maxHP;
	float _curHunger;
	float _maxHunger;
	vector<tagReward> _vReward;

	bool _isShow;
	Sprite* _sprite;
	PhysicsBody* _physics;

	NPC_State _state;

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void SetIsShowInfo(bool isShow) { _isShow = isShow; }
	bool GetIsShowInfo() { return _isShow; }

	Sprite* GetSprite() { return _sprite; }
	PhysicsBody* GetPhysics() { return _physics; }

	void DamageHP(float value);
	void HealHP(float value);
	float GetCurrentHP() { return _curHP; }
	float GetMaxHP() { return _maxHP; }

	void SetHunger(float value);
	void EatFood(float value);
	float GetCurrentHunger() { return _curHunger; }
	float GetMaxHunger() { return _maxHunger; }

	void SetAcceleration(float value) { _acceleration = value; }
	float GetAcceleration() { return _acceleration; }

	float GetHpDamageSpeed() { return _hpDamageSpeed; }
	float GetHungerSpeed() { return _hungerSpeed; }

	vector<tagReward> GetReward() { return _vReward; }
};