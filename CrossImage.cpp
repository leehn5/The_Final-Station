#include "stdafx.h"
#include "CrossImage.h"

void CrossImage::Init(string imgKey, Vector2 left, Vector2 right, float speed)
{
	_imgLeft = GRAPHICMANAGER->FindImage(imgKey);
	_imgRight = GRAPHICMANAGER->FindImage(imgKey);

	 _posLeft = left;
	 _posStart = _posRight = right;

	 _rcLeft = RectMake(0, 0, GRAPHICMANAGER->FindImage(imgKey)->GetFrameWidth(), GRAPHICMANAGER->FindImage(imgKey)->GetFrameHeight());

	_speed = speed;
}

void CrossImage::Release()
{
}

void CrossImage::Update()
{
	_posLeft.x -= _speed * TIMEMANAGER->getElapsedTime();
	_posRight.x -= _speed * TIMEMANAGER->getElapsedTime();

	if (_posLeft.x + _imgLeft->GetFrameWidth() <= 0) _posLeft.x = _posRight.x+ _imgRight->GetFrameWidth()-1;
	if (_posRight.x + _imgRight->GetFrameWidth() <= 0) _posRight.x = _posLeft.x + _imgLeft->GetFrameWidth()-1;
}

void CrossImage::Render()
{
	_imgLeft->Render(_posLeft, 1.0f, false, PIVOT::LEFT_TOP);
	_imgRight->Render(_posRight, 1.0f, true, PIVOT::RIGHT_TOP);
}
