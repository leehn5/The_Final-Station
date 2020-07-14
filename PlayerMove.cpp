#include "stdafx.h"
#include "PlayerMove.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "HandAttack.h"
#include "PistolAttack.h"
#include "UndergroundScene.h"
#include "PlayerDead.h"
#include "PlayerReload.h"
#include "UI.h"

void PlayerMove::Enter()
{
	_state = "Move";

	if (_obj->GetMode() == Mode::Combat)
	{
		_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Run");
		_obj->GetLegs()->GetComponent<Sprite>()->SetFPS(1.8f);
		_obj->GetPhysicsBody()->GetBody()->SetGravityScale(1);
	}
	else
	{
		if (_obj->GetIsHoldItem()) _obj->GetSprite()->SetImgName("train_walk_holdItem");
		else _obj->GetSprite()->SetImgName("train_walk");
		_obj->GetSprite()->SetFPS(1.9f);
	}
}

void PlayerMove::Update()
{
	if (_obj->GetMode() == Mode::Combat)
	{
		_obj->Move();

		if (!KEYMANAGER->isStayKeyDown('A') && !KEYMANAGER->isStayKeyDown('D'))
		{
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
			return;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			SOUNDMANAGER->play("Hit Swoosh");

			_obj->SetAttackType(AtkType::Hand);
			_obj->ChangeState(make_shared<HandAttack>(_obj));
			return;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (_obj->GetAmmo() > 0)
			{
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
	else
	{
		_obj->Non_CombatModeMove();

		if (!KEYMANAGER->isStayKeyDown('A') && !KEYMANAGER->isStayKeyDown('D'))
		{
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
			return;
		}
	}
}

void PlayerMove::Exit()
{
}
