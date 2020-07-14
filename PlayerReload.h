#pragma once
#include "PlayerState.h"

class PlayerReload : public PlayerState
{
private:

public:
	PlayerReload(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

