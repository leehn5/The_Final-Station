#include "stdafx.h"
#include "PlayerReload.h"
#include "Player.h"
#include "PlayerIdle.h"
#include "PlayerLadder.h"

void PlayerReload::Enter()
{
	_state = "Reload";

	_obj->GetSprite()->SetImgName("Hero_Reload_Pistol");
	_obj->GetSprite()->SetFPS(2.4f);
	_obj->GetSprite()->SetPosition(_obj->GetTrans()->GetPos() + Vector2(0.0f, -1.8f));
	_obj->GetAbility()->SetSpeed(180.f);

	_obj->GetBody()->GetComponent<Sprite>()->SetImgName("None");
	_obj->GetArms()->GetComponent<Sprite>()->SetImgName("None");

	SOUNDMANAGER->play("Pistol Reload", 1.0f);
}

void PlayerReload::Update()
{
	if (_obj->GetPrevState() != "Ladder") _obj->Move();

	_obj->GetSprite()->SetPosition(_obj->GetTrans()->GetPos() + Vector2(0.0f, -1.8f));

	if (_obj->GetSprite()->IsFrameEnd())
	{
		if(_obj->GetPrevState() == "Ladder")
			_obj->ChangeState(make_shared<PlayerLadder>(_obj));
		else
			_obj->ChangeState(make_shared<PlayerIdle>(_obj));
		return;
	}
}

void PlayerReload::Exit()
{
	_obj->Reload();
	_obj->GetSprite()->SetFPS(1.f);
	_obj->GetAbility()->SetSpeed(SPEED);
	_obj->GetSprite()->SetImgName("None");
	_obj->GetArms()->GetComponent<Sprite>()->SetImgName("Hero_pistol_arm");
}
