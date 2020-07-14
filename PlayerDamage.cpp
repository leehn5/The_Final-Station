#include "stdafx.h"
#include "PlayerDamage.h"
#include "Player.h"
#include "PlayerIdle.h"

void PlayerDamage::Enter()
{
	_state = "Damage";

	_obj->GetBody()->GetComponent<Sprite>()->SetImgName("Hero_Damage");
	_obj->GetLegs()->GetComponent<Sprite>()->SetImgName("Legs_Idle");

	_obj->GetBody()->GetComponent<Sprite>()->SetFPS(1.5f);

	CAMERA->ShakingSetting(0.2f, 2.5f);
}

void PlayerDamage::Update()
{
	_obj->Move();

	_obj->GetBody()->GetTrans()->SetPos(_obj->GetTrans()->GetPos() + Vector2(0, -2.f));

	if (_obj->GetBody()->GetComponent<Sprite>()->IsFrameEnd())
	{
		_obj->ChangeState(make_shared<PlayerIdle>(_obj));
		return;
	}
}

void PlayerDamage::Exit()
{
	_obj->GetBody()->GetComponent<Sprite>()->SetImgName("Hero_Idle");
}
