#include "stdafx.h"
#include "PlayerLadder.h"
#include "Player.h"
#include "UndergroundScene.h"
#include "PlayerIdle.h"
#include "PlayerDead.h"
#include "PlayerReload.h"
#include "PistolAttack.h"
#include "HandAttack.h"
#include "UI.h"

void PlayerLadder::Enter()
{
	_state = "Ladder";

	_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Run");
	_obj->GetLegs()->GetComponent<Sprite>()->SetFPS(1.5f);

	_obj->GetPhysicsBody()->GetBody()->SetGravityScale(0);

	_obj->GetAbility()->SetSpeed(0.0f);
}

void PlayerLadder::Update()
{
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		if (_obj->GetLegs()->GetComponent<Sprite>()->GetImgKey().compare("Legs_Idle") == 0)
			_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Run");

		UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();

		int index = ((int)_obj->GetTrans()->GetPos().x / TILE_WIDTH) + TILE_NUM_X * ((int)_obj->GetTrans()->GetPos().y / TILE_HEIGHT);
		int nowIndex = ((int)_obj->GetJudgingFloor()->GetTrans()->GetPos().x / TILE_WIDTH) + TILE_NUM_X * ((int)_obj->GetJudgingFloor()->GetTrans()->GetPos().y / TILE_HEIGHT);
		int upIndex = nowIndex - TILE_NUM_X;
		int downIndex = nowIndex + TILE_NUM_X;

		_distance = fabs(_obj->GetLadderPosition().y - _obj->GetTrans()->GetPos().y);

		if (_distance >= (_obj->GetLadderSize().y / 2 - 15)) //16
		{
			if (scene->GetTiles()[downIndex]->GetAttribute() == TAttribute::WALL) scene->GetTiles()[downIndex]->GetPhysics()->SetSensor(false);

			_obj->GetTrans()->SetPos(_obj->GetLadderPosition().x, _obj->GetLadderPosition().y - _obj->GetLadderSize().y / 2 + 6);
			_obj->GetPhysicsBody()->SetBodyPosition();
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
			return;
		}
		else
		{
			if (SCENEMANAGER->GetNowScene()->GetName().compare("underground") == 0)
			{
				if (scene->GetTiles()[downIndex]->GetAttribute() == TAttribute::WALL)
					scene->GetTiles()[downIndex]->GetPhysics()->SetSensor(false);

				if (scene->GetTiles()[upIndex]->GetTileObject()->GetAttribute() == TAttribute::LADDER)
				{
					if (scene->GetTiles()[index - TILE_NUM_X]->GetAttribute() == TAttribute::WALL)
						scene->GetTiles()[index - TILE_NUM_X]->GetPhysics()->SetSensor(true);

					_obj->GetTrans()->pos.x = _obj->GetLadderPosition().x;
					_obj->SetDirection(Dir::Up);
					_obj->GetAbility()->SetSpeed(200.0f);
				}
			}
		}
	}

	else if (KEYMANAGER->isStayKeyDown('S'))
	{
		if (_obj->GetLegs()->GetComponent<Sprite>()->GetImgKey().compare("Legs_Idle") == 0)
			_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Run");

		_distance = fabs(_obj->GetLadderPosition().y - _obj->GetTrans()->GetPos().y);

		if (_obj->GetTrans()->GetPos().y > _obj->GetLadderPosition().y && _distance >= (_obj->GetLadderSize().y / 2 - TILE_HEIGHT) + 7) // 60
		{
			_obj->GetPhysicsBody()->SetBodyPosition();
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
			return;
		}
		else
		{
			if (SCENEMANAGER->GetNowScene()->GetName().compare("underground") == 0)
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();

				int index = ((int)_obj->GetTrans()->GetPos().x / TILE_WIDTH) + TILE_NUM_X * ((int)_obj->GetTrans()->GetPos().y / TILE_HEIGHT);
				int nowIndex = ((int)_obj->GetJudgingFloor()->GetTrans()->GetPos().x / TILE_WIDTH) + TILE_NUM_X * ((int)_obj->GetJudgingFloor()->GetTrans()->GetPos().y / TILE_HEIGHT);
				int downIndex = nowIndex + TILE_NUM_X;

				if (scene->GetTiles()[downIndex]->GetTileObject()->GetAttribute() == TAttribute::LADDER)
				{
					if (scene->GetTiles()[downIndex]->GetAttribute() == TAttribute::WALL)
						scene->GetTiles()[downIndex]->GetPhysics()->SetSensor(true);

					_obj->GetTrans()->pos.x = _obj->GetLadderPosition().x;
					_obj->SetDirection(Dir::Down);
					_obj->GetAbility()->SetSpeed(200.0f);
				}
				else
				{
					if (scene->GetTiles()[index - TILE_NUM_X]->GetAttribute() == TAttribute::WALL)
						scene->GetTiles()[index - TILE_NUM_X]->GetPhysics()->SetSensor(false);
				}
			}
		}
	}

	if (!KEYMANAGER->isStayKeyDown('W') && !KEYMANAGER->isStayKeyDown('S'))
	{
		_obj->GetAbility()->SetSpeed(0.f);

		if (_obj->GetLegs()->GetComponent<Sprite>()->GetImgKey().compare("Legs_Run") == 0)
			_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Idle");
	}

	_obj->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(cosf((int)_obj->GetDirection() * Deg2Rad), -sinf((int)_obj->GetDirection() * Deg2Rad)) * 
		_obj->GetAbility()->GetSpeed() * TIMEMANAGER->getElapsedTime());
	_obj->GetPhysicsBody()->SetBodyPosition();

	if (_obj->GetAbility()->IsDead())
	{
		_obj->GetPhysicsBody()->GetBody()->SetGravityScale(1);
		_obj->GetPhysicsBody()->ApplyForce(Vector2::b2Down);

		_obj->ChangeState(make_shared<PlayerDead>(_obj));
		return;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		SOUNDMANAGER->play("Hit Swoosh");

		_obj->SetPrevState(_state);
		_obj->SetAttackType(AtkType::Hand);
		_obj->ChangeState(make_shared<HandAttack>(_obj));
		return;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (_obj->GetAmmo() > 0)
		{
			_obj->SetPrevState(_state);
			_obj->SetAttackType(AtkType::Pistol);
			_obj->ChangeState(make_shared<PistolAttack>(_obj));
			return;
		}
		else
		{
			SOUNDMANAGER->play("Clip Empty");
			UI::Text("Empth", _obj->GetTrans()->GetPos() + Vector2::up * 30, Vector2(100, 30), ColorF::Azure);
		}
	}
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		if (_obj->GetAmmo() < _obj->GetMagazine() && _obj->GetReminingAmmo() > 0)
		{
			_obj->SetPrevState(_state);
			_obj->ChangeState(make_shared<PlayerReload>(_obj));
			return;
		}
	}
}

void PlayerLadder::Exit()
{
}
