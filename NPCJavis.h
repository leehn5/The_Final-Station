#pragma once
#include "NPC.h"

class NPCJavis : public NPC
{
private:

public:
	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

