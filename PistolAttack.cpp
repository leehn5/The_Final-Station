#include "stdafx.h"
#include "PistolAttack.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "BulletManager.h"
#include "PlayerLadder.h"

void PistolAttack::Enter()
{
	_state = "Weapon";

	_obj->GetArms()->GetComponent<Sprite>()->SetImgName("Pistol_Attack");
	_obj->GetArms()->GetComponent<Sprite>()->SetFPS(1.3f);

	_obj->GetBody()->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(-4.f * (int)_obj->GetCursorWhere(), -2.f));

	CAMERA->ShakingSetting( 0.3f, 3.5f);

	float angle = Vector2::GetAngle(_obj->GetTrans()->GetPos(), MOUSEPOINTER->GetMouseWorldPosition());

	if (_obj->GetCursorWhere() == CursorWhere::Right)
	{
		if (angle < -0.5f) angle = -0.5f;
		else if (angle > 0.6f) angle = 0.6f;
	}

	else if (_obj->GetCursorWhere() == CursorWhere::Left)
	{
		 if (angle <0  && angle > -2.5f) angle = -2.5f;
		else if (angle > 0 && angle < 2.5f) angle = 2.5f;
	}

	_obj->GetBullet()->Fire(_obj->GetTrans()->GetPos() + Vector2(-20 * (int)_obj->GetCursorWhere(), -2.f), 150.f, angle, 20.f, (int)_obj->GetCursorWhere());
	_obj->FireBullet(1);

	SOUNDMANAGER->play("Pistol Shoot", 0.6f);
}

void PistolAttack::Update()
{
	if (_obj->GetPrevState() != "Ladder") _obj->Move();

	_obj->GetBody()->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(-4.f * (int)_obj->GetCursorWhere(), -2.f));

	_obj->GetArms()->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(8.0f * (int)_obj->GetCursorWhere(), -2.5f));
	_obj->GetArms()->GetComponent<Sprite>()->SetPosition(_obj->GetArms()->GetTrans()->GetPos());

	if (_obj->GetArms()->GetComponent<Sprite>()->IsFrameEnd())
	{
		if (_obj->GetPrevState() == "Ladder")
		{
			_obj->SetPrevState(_state);
			_obj->ChangeState(make_shared<PlayerLadder>(_obj));
		}
		else
		{
			_obj->SetPrevState(_state);
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
		}
		return;
	}
}

void PistolAttack::Exit()
{
	_obj->GetArms()->GetComponent<Sprite>()->SetImgName("Hero_pistol_arm");
	_obj->GetArms()->GetComponent<Sprite>()->SetFPS(1.0f);
	_obj->GetArms()->GetComponent<Sprite>()->Start();

	_obj->GetBody()->GetComponent<Sprite>()->Start();
}
