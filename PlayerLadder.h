#pragma once
#include "PlayerState.h"

class PlayerLadder : public PlayerState
{
private:
	float _distance;

public:
	PlayerLadder(Player* obj) : PlayerState(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

