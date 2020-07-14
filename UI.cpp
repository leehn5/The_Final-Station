#include "stdafx.h"
#include "UI.h"
#include "ProgressBar.h"
#include "Player.h"
#include "Item.h"

float UI::alpha = 0.f;
float UI::timer = 0.f;
Vector2 UI::startPos = Vector2::zero;
Vector2 UI::endPos = Vector2::zero;
Vector2 UI::size = Vector2(1, 1);
ColorF::Enum UI::txtColor = ColorF::White;
string UI::text = "";

string UI::npcName = "";

void UI::Init()
{
	if (SCENEMANAGER->GetNowScene()->GetName() == "Train")
	{
		GRAPHICMANAGER->AddImage("ui_close", L"Resource/UI/ui_close.png", 2, 1);

		_ventUI = new VentUI;
		_ventUI->Init();

		_showPart = TrainPart::None;

		_close = new Button;
		_close->Init();
		_close->Setting("ui_close", _ventUI->GetPosition() + Vector2(_ventUI->GetSize().x / 3.4f, -(_ventUI->GetSize().y / 1.8f)), PointMake(0, 0), PointMake(1, 0), cbClose, this);

		_npcInfo = new NPC_Info;
		_npcInfo->Init();

		_timer = 0.0f;
	}
	else
	{
		GRAPHICMANAGER->AddImage("statusbar", L"Resource/UI/_sb statusbar.png");
		GRAPHICMANAGER->AddImage("pistol_bg", L"Resource/UI/pistol_bg.png");
		GRAPHICMANAGER->AddImage("a_pistol", L"Resource/UI/a_pistol.png");

		_statusbar = GRAPHICMANAGER->FindImage("statusbar");

		_hpBar = new ProgressBar;
		_hpBar->Init("health_line", "hp_bg", L"Resource/UI/", L"Resource/UI/", Vector2(WINSIZEX / 2, WINSIZEY - 24) + Vector2(-192.f, -6.0f));
		_hpBar->SetScale(Vector2(1.5f, 1.5f));

		_pistolBg = GRAPHICMANAGER->FindImage("pistol_bg");
		_pistol = GRAPHICMANAGER->FindImage("a_pistol");

		_imgBattery = GRAPHICMANAGER->FindImage("Battery");
	}

	_player = (Player*)SCENEMANAGER->GetNowScene()->GetChildFromName("Player");
}

void UI::Release()
{
	alpha = 0.f;
	timer = 0.f;
	text = "";
}

void UI::Update()
{
	if (SCENEMANAGER->GetNowScene()->GetName() == "Train")
	{
		_npcMgr->Update();
		_npcInfo->Update();

		if (_ventUI->IsMaxFigure() == false)
		{
			_timer += TIMEMANAGER->getElapsedTime();

			if (_timer > 5.f)
			{
				_timer = 0.0f;
				_ventUI->IncreaseFigure(1.f);
			}
		}
		if (_ventUI->GetGauge()->GetFigure() >= _ventUI->GetGauge()->GetMaxFigure() * 0.6f)
		{
			_npcMgr->DamageNPC_HP();
		}

		if (_showPart == TrainPart::Vent)
		{
			_close->Update();
			_ventUI->Update();
		}
		if (_player->GetCollisionType() == ColType::NPC)
		{
			_npcInfo->SetPosition(_npcMgr->FindNPC(npcName)->GetTrans()->GetPos());
			_npcInfo->SetInfo(_npcMgr->FindNPC(npcName));
		}

		MoveDisappear();
	}
	else
	{
		_hpBar->SetGauge(_player->GetAbility()->GetCurrentHP(), _player->GetAbility()->GetMaxHP());

		MoveDisappear();
	}
}

void UI::Render()
{
	if (SCENEMANAGER->GetNowScene()->GetName() == "Train")
	{
		if (_showPart == TrainPart::Vent)
		{
			_ventUI->Render();
			_close->Render();
		}

		if (_player->GetCollisionType() == ColType::NPC)
		{
			_npcInfo->Render();
		}
	}
	else
	{
		_statusbar->Render(Vector2(WINSIZEX / 2, WINSIZEY - 24), Vector2(1.5f, 1.5f), 0.0f, Vector2::zero, false, 1.0f, PIVOT::CENTER, false);
		_hpBar->Render();
		_pistolBg->Render(Vector2(WINSIZEX / 2 + 50, WINSIZEY - 22), 1.0f, false, PIVOT::CENTER, false);

		wchar_t buffer[128];
		swprintf(buffer, 128, L"%d", _player->GetReminingAmmo());
		GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2, WINSIZEY - 32), buffer, 15, 25, 15, ColorF::Azure, TextPivot::LEFT_TOP, L"Pixel Cyr");

		swprintf(buffer, 128, L"%d", _player->GetInventory().find("MEDKIT")->second->count);
		GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2 - 210, WINSIZEY - 32), buffer, 15, 25, 15, ColorF::Azure, TextPivot::LEFT_TOP, L"Pixel Cyr");

		swprintf(buffer, 128, L"%d", _player->GetInventory().find("FOOD")->second->count);
		GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2 + 154, WINSIZEY - 32), buffer, 15, 25, 15, ColorF::Azure, TextPivot::LEFT_TOP, L"Pixel Cyr");

		swprintf(buffer, 128, L"%d", _player->GetInventory().find("GOLD KEY")->second->count);
		GRAPHICMANAGER->Text(Vector2(WINSIZEX / 2 + 254, WINSIZEY - 32), buffer, 15, 25, 15, ColorF::Azure, TextPivot::LEFT_TOP, L"Pixel Cyr");

		for (int i = 0; i < _player->GetAmmo(); ++i)
		{
			_pistol->Render(Vector2((WINSIZEX / 2 + 25) + i * 8.2f, WINSIZEY - 29), 1.0f, false, PIVOT::LEFT_TOP, false);
		}

		if (_player->GetIsBattery())
			_imgBattery->Render(Vector2(340, WINSIZEY - 32), Vector2(1.5f, 1.5f), 0.0f, Vector2::zero, false, 1.0f, PIVOT::CENTER, false);
	}

	if (alpha > 0.f)
		GRAPHICMANAGER->Text(startPos, text.c_str(), 27, size.x, size.y, ColorF(txtColor, alpha), TextPivot::CENTER, L"Munro Small", true);
}

void UI::Disappear()
{
	if (alpha > 0.f)
	{
		if (timer < 3.f) timer += TIMEMANAGER->getElapsedTime();
		else alpha -= 3.0f * TIMEMANAGER->getElapsedTime();
	}
}

void UI::MoveDisappear()
{
	if (alpha > 0.f)
	{
		if (startPos.y > endPos.y) startPos += Vector2(0, -0.5f);
		else
		{
			if (timer < 3.f) timer += TIMEMANAGER->getElapsedTime();
			else alpha -= 3.0f * TIMEMANAGER->getElapsedTime();
		}
	}
}

void UI::Text(string txt, Vector2 pos, Vector2 maxSize, ColorF::Enum color)
{
	text = txt;
	txtColor = color;
	alpha = 1.f;
	timer = 0.f;
	startPos = pos + Vector2(-(maxSize.x * 0.5f), -50.f);
	endPos = startPos + Vector2(0, -15.f);
	size = maxSize;
}

void UI::cbClose(void* obj)
{
	UI* ui = (UI*)obj;
	ui->SetShowTrainPart(TrainPart::None);
	ui->_player->SetIsMove(true);
}

void UI::GetNPCName(string name)
{
	npcName = name;
}
