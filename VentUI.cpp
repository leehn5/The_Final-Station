#include "stdafx.h"
#include "VentUI.h"

void VentUI::Init()
{
	GRAPHICMANAGER->AddImage("ventBase", L"Resource/UI/Vent/_vent base.png");
	GRAPHICMANAGER->AddImage("ok", L"Resource/UI/Vent/_ui ok.png");
	
	_base = GRAPHICMANAGER->FindImage("ventBase");

	_pos = Vector2(WINSIZEX / 2 + 20.f, WINSIZEY / 2);
	_size = Vector2(GRAPHICMANAGER->FindImage("ventBase")->GetFrameWidth(), GRAPHICMANAGER->FindImage("ventBase")->GetFrameHeight());

	for (int i = 0; i < 5; ++i)
	{
		_gauges[i] = new VentGraph;
		_gauges[i]->Init((WINSIZEX / 2 - 98) + i * 46, WINSIZEY / 2);
	}

	_increaseIndex = RND->getInt(5);
	_value = 0;
}

void VentUI::Release()
{
	for (int i = 0; i < 5; ++i)
		SAFE_OBJECT_RELEASE(_gauges[i]);
	SAFE_DELETE_ARRAY(*_gauges);
}

void VentUI::Update()
{
	for (int i = 0; i < 5; ++i)
		_gauges[i]->Update();
}

void VentUI::Render()
{
	_base->Render(_pos, 1.f, false, PIVOT::CENTER, false);

	wchar_t buffer[128];
	GRAPHICMANAGER->Text(Vector2(_pos.x - 143.f, _pos.y - 130), "VENT\nSYSTEM", 13, 60, 50, ColorF(0x859378, 1.f), TextPivot::LEFT_TOP, L"Pixel Cyr");

	for (int i = 0; i < 5; ++i)
		_gauges[i]->Render();

	if (_gauges[_increaseIndex]->GetFigure() - _gauges[_increaseIndex]->GetMidleFigure() > 0.f)
	{
		swprintf(buffer, 128, L"+%1.fV", _gauges[_increaseIndex]->GetFigure() - _gauges[_increaseIndex]->GetMidleFigure());
		GRAPHICMANAGER->Text(Vector2(_pos.x + 12.f, _pos.y - 128), buffer, 20, 60, 50, ColorF(0xb45d5e, 1.f), TextPivot::LEFT_TOP, L"Pixel Cyr");
	}
	else GRAPHICMANAGER->DrawImage("ok", Vector2(_pos.x + 35.f, _pos.y - 115), 1.0f, false, PIVOT::CENTER, false);
}

bool VentUI::IsMaxFigure()
{
	if (_gauges[_increaseIndex]->GetFigure() >= _gauges[_increaseIndex]->GetMaxFigure()) return true;
	return false;
}
