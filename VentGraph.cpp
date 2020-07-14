#include "stdafx.h"
#include "VentGraph.h"

void VentGraph::Init(float x, float y)
{
	GRAPHICMANAGER->AddImage("vent_button", L"Resource/UI/Vent/vent_button.png", 2, 1);

	_pos = Vector2(x, y);

	_btn = new Button;
	_btn->Init();
	_btn->Setting("vent_button", _pos + Vector2(0, 31), PointMake(0, 0), PointMake(1, 0), cbDown, this);

	_maxFigure = 46.f;
	_figure = _midFigure = 22.f;

	_color = 0xd88a8b;
}

void VentGraph::Release()
{
	SAFE_OBJECT_RELEASE(_btn);
	SAFE_DELETE(_btn);
}

void VentGraph::Update()
{
	_btn->Update();

	if (_figure >= _maxFigure * 0.75f) _color = 0xdf2426;
	else _color = 0xd88a8b;
}

void VentGraph::Render()
{
	GRAPHICMANAGER->DrawFillRect(_pos + Vector2(0, -2), Vector2(18.f, _figure), 0.f, ColorF(_color, 1.f), PIVOT::BOTTOM, false);
	_btn->Render();
}

void VentGraph::IncreaseFigure(float value)
{
	if (_figure >= _maxFigure)
	{
		_figure = _maxFigure;
		return;
	}
	_figure += value;
}

void VentGraph::ReduceFigure(float value)
{
	if (_figure <= 0.f)
	{
		_figure = 0.f;
		return;
	}
	_figure -= value;
}

void VentGraph::cbDown(void* obj)
{
	VentGraph* vent = (VentGraph*)obj;
	if (vent->GetFigure() <= vent->GetMidleFigure()) return;
	vent->ReduceFigure(1.f);
}
