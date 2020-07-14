#include "stdafx.h"
#include "Ability.h"

void Ability::setHP(float value)
{
	_curHP = value;
}

void Ability::DamageHP(float value)
{
	_curHP -= value;

	if (_curHP <= 0) _curHP = 0;
}

void Ability::HealHP(float value)
{
	_curHP += value;

	if (_curHP >= _maxHP) _curHP = _maxHP;
}

bool Ability::IsDead()
{
	if (_curHP <= 0)
	{
		_curHP = 0;
		return true;
	}

	return false;
}
