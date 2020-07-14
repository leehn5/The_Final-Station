#pragma once
#include "InteractionObject.h"

class FoodObject : public InteractionObject
{
private:

public:
	virtual void Init();
	virtual void Update();

	virtual void Interaction();

};

