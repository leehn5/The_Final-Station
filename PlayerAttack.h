#pragma once
#include "PlayerState.h"

class PlayerAttack : public PlayerState
{
private:

public:
	PlayerAttack(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

