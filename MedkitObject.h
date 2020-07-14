#pragma once
#include "InteractionObject.h"

class MedkitObject : public InteractionObject
{
private:

public:
	virtual void Init();
	virtual void Udate();

	virtual void Interaction();
};

