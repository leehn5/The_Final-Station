#include "stdafx.h"
#include "NPC.h"
#include "NPCCollider.h"

void NPC::Init()
{
	Object::Init();

	_tag = "NPC";

	_trans->SetScale(37, 62);

	_frame = _deadTime = _acceleration = 0.f;

	AddComponent<NPCCollider>();
}

void NPC::Release()
{
	Object::Release();
}

void NPC::Update()
{
	Object::Update();

	if (_curHP <= 0.f)
	{
		if (_deadTime < 8.f) _deadTime += 1.f * TIMEMANAGER->getElapsedTime();
		else
		{
			if (_state == NPC_State::Idle)
			{
				_state = NPC_State::Dead;
				_sprite->SetImgName(_name + "_dead");
				_sprite->SetPosition(_trans->GetPos() + Vector2::down * 33);
				_sprite->SetFPS(0.8f);
				_physics->SetBodyActive(false);
			}
		}
	}

	if (_curHunger <= 0.f) _acceleration = 0.02f;
}

void NPC::Render()
{
	Object::Render();
}

void NPC::DamageHP(float value)
{
	_curHP -= value;
	if (_curHP <= 0.f) _curHP = 0.f;
}

void NPC::HealHP(float value)
{
	_curHP += value;
	if (_curHP >= _maxHP) _curHP = _maxHP;
}

void NPC::SetHunger(float value)
{
	_curHunger -= value;
	if (_curHunger <= 0.f) _curHunger = 0.f;
}

void NPC::EatFood(float value)
{
	_curHunger += value;
	if (_curHunger >= _maxHunger) _curHunger = _maxHunger;
}
