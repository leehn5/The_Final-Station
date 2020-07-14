#pragma once
#include "PlayerAttack.h"

class HandAttack : public PlayerAttack
{
private:
	Object* _atkObj;

public:
	HandAttack(Player* obj) : PlayerAttack(obj) {}

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

