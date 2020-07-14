#include "stdafx.h"
#include "NPC_Info.h"

void NPC_Info::Init()
{
	GRAPHICMANAGER->AddImage("base", L"Resource/UI/NPC/npcUI.png");
	GRAPHICMANAGER->AddImage("Face_JOHN ALEX", L"Resource/UI/NPC/Face_JOHN ALEX.png");
	GRAPHICMANAGER->AddImage("Face_MARTIN JAVIS", L"Resource/UI/NPC/Face_MARTIN JAVIS.png");
	GRAPHICMANAGER->AddImage("npcHP", L"Resource/UI/NPC/npcHP.png");
	GRAPHICMANAGER->AddImage("npcHungerBar", L"Resource/UI/NPC/npcHungerBar.png");
	GRAPHICMANAGER->AddImage("npcHPback", L"Resource/UI/NPC/npcHPback.png");
	GRAPHICMANAGER->AddImage("hunger", L"Resource/UI/NPC/hunger.png");
	GRAPHICMANAGER->AddImage("health", L"Resource/UI/NPC/health.png");

	_basePos = Vector2::zero;

	_base = GRAPHICMANAGER->FindImage("base");
	_face = GRAPHICMANAGER->FindImage("Face_JOHN ALEX");

	_hp = new ProgressBar;
	_hp->Init("npcHP", "npcHPback", _basePos);

	_hunger = new ProgressBar;
	_hunger->Init("npcHungerBar", "npcHPback", _basePos);
}

void NPC_Info::Release()
{
}

void NPC_Info::Update()
{
	_hp->SetGauge(_curHP, _maxHP);
	_hunger->SetGauge(_curHunger, _maxHunger);
}

void NPC_Info::Render()
{
	_base->Render(_basePos + Vector2::down * 105);

	_hp->Render();
	_hunger->Render();

	_face->Render(_basePos + Vector2(-104, 114));

	GRAPHICMANAGER->Text(_basePos + Vector2(-80, 94), _name.c_str(), 20, 200, 30, ColorF::Black, TextPivot::LEFT_TOP, L"Pixel Cyr");

	if (_curHP <= 0.f)
		GRAPHICMANAGER->DrawImage("health", _basePos + Vector2(74, 71), 1.f, false, PIVOT::CENTER, false);

	if (_curHunger <= 0.f)
		GRAPHICMANAGER->DrawImage("hunger", _basePos + Vector2(44, 71), 1.f, false, PIVOT::CENTER, false);
}

void NPC_Info::SetInfo(NPC* npc)
{
	_name = npc->GetName();
	_face = GRAPHICMANAGER->FindImage("Face_" + _name);

	_curHP = npc->GetCurrentHP();
	_maxHP = npc->GetMaxHP();

	_curHunger = npc->GetCurrentHunger();
	_maxHunger = npc->GetMaxHunger();

	_hp->SetPosition(_basePos + Vector2(-54, 60));
	_hunger->SetPosition(_basePos + Vector2(-54, 75));
}
