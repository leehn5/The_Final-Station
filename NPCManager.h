#pragma once
#include "NPCAlex.h"
#include "NPCJavis.h"

enum class NPC_Type
{
	Alex,
	Javis,
	None,
};

class NPCManager
{
private:
	//vector<pair<string, NPC*>> _vNPC;
	map<string, NPC*> _mNPC;

public:
	void Init();
	void Release();
	void Update();
	void Render();

	void SetNPC(NPC_Type type, Vector2 pos);

	//vector<pair<string, NPC*>> GetNPC() { return _vNPC; }
	map<string, NPC*> GetNPC() { return _mNPC; }
	void DamageNPC_HP();
	NPC* FindNPC(string name);
};

