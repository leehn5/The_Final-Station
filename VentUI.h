#pragma once
#include "Button.h"
#include "VentGraph.h"

class VentUI
{
private:
	Graphic* _base;
	Vector2 _size;
	Vector2 _pos;

	VentGraph* _gauges[5];

	int _increaseIndex;
	float _value;

public:
	void Init();
	void Release();
	void Update();
	void Render();
	
	Vector2 GetSize() { return _size; }
	Vector2 GetPosition() { return _pos; }

	void IncreaseFigure(float value) { _gauges[_increaseIndex]->IncreaseFigure(value); }
	VentGraph* GetGauge() { return _gauges[_increaseIndex]; }
	bool IsMaxFigure();
};

