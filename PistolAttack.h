#pragma once
#include "PlayerAttack.h"

class PistolAttack : public PlayerAttack
{
private:

public:
	PistolAttack(Player* obj) : PlayerAttack(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

