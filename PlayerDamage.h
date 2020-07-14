#pragma once
#include "PlayerState.h"

class PlayerDamage : public PlayerState
{
private:

public:
	PlayerDamage(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

