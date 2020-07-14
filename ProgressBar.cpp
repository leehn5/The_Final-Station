#include "stdafx.h"
#include "ProgressBar.h"

void ProgressBar::Init(string upImgKey, string downImgKey, Vector2 pos)
{
	_frontImg = upImgKey;
	_backImg = downImgKey;

	_pos = pos;

	wstring path;

	path.assign(_frontImg.begin(), _frontImg.end());
	//_gaugeFront = GRAPHICMANAGER->AddImage(_frontImg, path + L".png");
	_gaugeFront = GRAPHICMANAGER->FindImage(upImgKey);

	path.assign(_backImg.begin(), _backImg.end());
	//_gaugeBack = GRAPHICMANAGER->AddImage(_backImg, path + L".png");
	_gaugeBack = GRAPHICMANAGER->FindImage(downImgKey);

	_size = Vector2(_gaugeFront->GetWidth(), _gaugeFront->GetHeight());
	_scale = Vector2(1, 1);
}

void ProgressBar::Init(string upImgKey, string downImgKey, wstring upPath, wstring downPath, Vector2 pos)
{
	_frontImg = upImgKey;
	_backImg = downImgKey;

	_pos = pos;

	wstring name;

	name.assign(_frontImg.begin(), _frontImg.end());
	_gaugeFront = GRAPHICMANAGER->AddImage(_frontImg, upPath + name + L".png");

	name.assign(_backImg.begin(), _backImg.end());
	_gaugeBack = GRAPHICMANAGER->AddImage(_backImg, downPath + name + L".png");

	_size = Vector2(_gaugeFront->GetWidth(), _gaugeFront->GetHeight());
	_scale = Vector2(1, 1);
}

void ProgressBar::Release()
{
	_gaugeFront->Release();
	_gaugeBack->Release();
}

void ProgressBar::Render()
{
	_gaugeBack->Render(_pos, _scale, 0.0f, Vector2::zero, false, 1.0f, PIVOT::LEFT_TOP, false);
	_gaugeFront->Render(_pos, Vector2(0, 0), _size, _scale, PIVOT::LEFT_TOP, 1.0f, 0.0f, false);
}

void ProgressBar::SetGauge(float current, float max)
{
	_size.x = (current / max) * _gaugeBack->GetWidth();
}
