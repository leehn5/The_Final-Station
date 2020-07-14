#pragma once
#include "Button.h"

class VentGraph
{
private:
	Button* _btn;
	Vector2 _pos;
	float _figure;
	float _midFigure;
	float _maxFigure;

	uint32 _color;

public:
	void Init(float x, float y);
	void Release();
	void Update();
	void Render();

	void IncreaseFigure(float value);
	void ReduceFigure(float value);

	float GetFigure() { return _figure; }
	float GetMaxFigure() { return _maxFigure; }
	float GetMidleFigure() { return _midFigure; }

	static void cbDown(void* obj);
};

