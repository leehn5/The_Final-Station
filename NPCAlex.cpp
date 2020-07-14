#include "stdafx.h"
#include "NPCAlex.h"

void NPCAlex::Init()
{
	NPC::Init();

	GRAPHICMANAGER->AddImage("JOHN ALEX", L"Resource/NPC/Train/JOHN ALEX.png", 9);
	GRAPHICMANAGER->AddImage("JOHN ALEX_dead", L"Resource/NPC/Train/JOHN ALEX_dead.png", 10);

	_name = "JOHN ALEX";

	_hpDamageSpeed = 0.4f;
	_hungerSpeed = 0.4f;

	_curHP = 100.f;
	_maxHP = 100.f;
	_curHunger = 100.f; 
	_maxHunger = 100.f;

	tagReward reward;
	reward.type = RewardType::Money;
	reward.count = 260;
	_vReward.push_back(reward);

	_isShow = false;

	_sprite = AddComponent<Sprite>();
	_sprite->Init(true);
	_sprite->SetImgName(_name);
	_sprite->SetPivot(PIVOT::BOTTOM);
	_sprite->SetDepth(3);
	_sprite->SetFPS(1.5f);
	_sprite->Stop();

	_physics = AddComponent<PhysicsBody>();
	_physics->Init(BodyType::STATIC, 1.f);
	_physics->SetSensor(true);

	_state = NPC_State::Idle;

	_changeStateTime = RND->getFromFloatTo(20, 50);
}

void NPCAlex::Release()
{
	NPC::Release();
}

void NPCAlex::Update()
{
	NPC::Update();

	if (_state == NPC_State::Idle)
	{
		_frame += 1.f * TIMEMANAGER->getElapsedTime();
		if (_frame > _changeStateTime)
		{
			_frame = 0.f;
			_changeStateTime = RND->getFromFloatTo(20, 50);
			_sprite->Start();
		}
	}
}

void NPCAlex::Render()
{
	NPC::Render();
}
