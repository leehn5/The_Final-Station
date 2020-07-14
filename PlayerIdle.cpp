#include "stdafx.h"
#include "PlayerIdle.h"
#include "Player.h"
#include "PlayerMove.h"
#include "PlayerAttack.h"
#include "HandAttack.h"
#include "PlayerLadder.h"
#include "PlayerDead.h"
#include "UndergroundScene.h"
#include "PistolAttack.h"
#include "PlayerReload.h"
#include "UI.h"

void PlayerIdle::Enter()
{
	_state = "Idle";

	if (_obj->GetMode() == Mode::Combat)
	{
		_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Idle");
		_obj->GetLegs()->GetComponent<Sprite>()->SetFPS(1.0f);
	}
	else
	{
		if(_obj->GetIsHoldItem()) _obj->GetSprite()->SetImgName("train_idle_holdItem");
		else _obj->GetSprite()->SetImgName("train_idle");
		_obj->GetSprite()->SetFPS(0.7f);
		_obj->GetPhysicsBody()->GetBody()->SetLinearVelocity(Vector2::b2Zero);
	}
}

void PlayerIdle::Update()
{
	if (KEYMANAGER->isStayKeyDown('D') or KEYMANAGER->isStayKeyDown('A'))
	{
		if (_obj->GetIsMove())
		{
			_obj->ChangeState(make_shared<PlayerMove>(_obj));
			return;
		}
	}

	if (_obj->GetMode() == Mode::Combat)
	{
		if (_obj->GetIsLadderCollider())
		{
			_obj->SetLadderDistance(fabs(_obj->GetLadderPosition().x - _obj->GetTrans()->GetPos().x));

			if (_obj->GetLadderDistance() <= 10.0f)
			{
				UndergroundScene* scene = (UndergroundScene*)SCENEMANAGER->GetNowScene();

				int nowIndex = ((int)_obj->GetJudgingFloor()->GetTrans()->GetPos().x / TILE_WIDTH) + TILE_NUM_X * ((int)_obj->GetJudgingFloor()->GetTrans()->GetPos().y / TILE_HEIGHT);

				if (KEYMANAGER->isStayKeyDown('W'))
				{
					if (scene->GetTiles()[nowIndex - TILE_NUM_X]->GetTileObject()->GetAttribute() == TAttribute::LADDER)
					{
						_obj->ChangeState(make_shared<PlayerLadder>(_obj));
						return;
					}
				}
				if (KEYMANAGER->isStayKeyDown('S'))
				{
					if (scene->GetTiles()[nowIndex + TILE_NUM_X]->GetTileObject()->GetAttribute() == TAttribute::LADDER)
					{
						_obj->ChangeState(make_shared<PlayerLadder>(_obj));
						return;
					}
				}
			}
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
				UI::Text("Empty", _obj->GetTrans()->GetPos() + Vector2::up * 30, Vector2(100, 30), ColorF::Azure);
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

		if (_obj->GetAbility()->IsDead())
		{
			_obj->ChangeState(make_shared<PlayerDead>(_obj));
			return;
		}
	}
}

void PlayerIdle::Exit()
{
}
