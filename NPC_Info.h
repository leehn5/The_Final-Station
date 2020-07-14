#pragma once
#include "NPC.h"
#include "ProgressBar.h"

class NPC_Info
{
private:
	Graphic* _base;
	Graphic* _face;
	ProgressBar* _hp;
	ProgressBar* _hunger;
	string _name;

	float _curHP, _maxHP;
	float _curHunger, _maxHunger;

	Vector2 _basePos;

public:
	void Init();
	void Release();
	void Update();
	void Render();

	void SetPosition(Vector2 pos) { _basePos = pos; }
	void SetInfo(NPC* npc);
	//void SetInfo(pair<string, NPC*> npc);
};

