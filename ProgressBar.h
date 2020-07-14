#pragma once
#include "Graphic.h"

class ProgressBar
{
private:
	Graphic* _gaugeFront;
	Graphic* _gaugeBack;

	Vector2 _pos;
	Vector2 _size;
	Vector2 _scale;

	string _frontImg;
	string _backImg;

public:
	void Init(string frontImgKey, string backImgKey, Vector2 pos);
	void Init(string frontImgKey, string backImgKey, wstring frontPath, wstring backPath, Vector2 pos);
	void Release();
	void Render();

	void SetPosition(Vector2 pos) { _pos = pos; }
	void SetGauge(float current, float max);
	void SetScale(Vector2 scale) { _scale = scale; }
};

