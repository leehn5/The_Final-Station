#pragma once
#include "PlayerState.h"

class PlayerIdle : public PlayerState
{
private:

public:
	PlayerIdle(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

