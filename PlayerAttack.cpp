#include "stdafx.h"
#include "PlayerAttack.h"
#include "Player.h"
#include "PlayerIdle.h"

void PlayerAttack::Enter()
{
	_state = "Attack";
}

void PlayerAttack::Update()
{
}

void PlayerAttack::Exit()
{
}
