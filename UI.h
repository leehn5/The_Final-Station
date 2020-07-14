#pragma once
#include "VentUI.h"
#include "Button.h"
#include "NPCManager.h"
#include "NPC_Info.h"

class ProgressBar;
class Player;

enum class TrainPart
{
	Vent,
	None,
};

class UI
{
private:
	static string text;
	static ColorF::Enum txtColor;
	static float timer;
	static float alpha;
	static Vector2 startPos;
	static Vector2 endPos;
	static Vector2 size;

	static string npcName;

private:
	Graphic* _statusbar;
	Graphic* _pistolBg;
	Graphic* _pistol;
	Graphic* _imgBattery;
	ProgressBar* _hpBar;
	Player* _player;

	VentUI* _ventUI;
	TrainPart _showPart;
	Button* _close;

	NPCManager* _npcMgr;
	NPC_Info* _npcInfo;

	float _timer;

public:
	void Init();
	void Release();
	void Update();
	void Render();

	void Disappear();
	void MoveDisappear();

	void SetShowTrainPart(TrainPart part) { _showPart = part; }
	TrainPart GetShowTrainPart() { return _showPart; }

	Button* GetCloseButton() { return _close; }

	void LinkedNPCManager(NPCManager* npc) { _npcMgr = npc; }

	string GetNpcName() { return npcName; }

	static void Text(string txt, Vector2 pos, Vector2 maxSize, ColorF::Enum color);
	static void cbClose(void* obj);

	static void GetNPCName(string name);
};

