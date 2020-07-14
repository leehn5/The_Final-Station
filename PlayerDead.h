#pragma once
#include "PlayerState.h"

class PlayerDead : public PlayerState
{
private:

public:
	PlayerDead(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

