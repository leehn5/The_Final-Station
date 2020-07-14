#pragma once
#include "PlayerState.h"

class PlayerMove : public PlayerState
{
private:

public:
	PlayerMove(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

