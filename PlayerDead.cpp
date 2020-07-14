#include "stdafx.h"
#include "PlayerDead.h"
#include "Player.h"
#include "PlayerIdle.h"

void PlayerDead::Enter()
{
	_state = "Dead";

	_obj->GetSprite()->SetImgName("Hero_Dead");
	_obj->GetSprite()->SetFPS(1.0f);
}

void PlayerDead::Update()
{
	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		_obj->ChangeState(make_shared<PlayerIdle>(_obj));
	}
}

void PlayerDead::Exit()
{
}
