#include "stdafx.h"
#include "Sprite.h"

Sprite::Sprite()
{
	_name = "Sprite";

	_color = ColorF::Blue;
	_pivot = PIVOT::CENTER;
	_strokeWidth = 1.0f;

	_isFlipX = false;
	_isFillRect = false;
	_isShowRect = false;
	_isCameraEffect = true;

	_alpha = 1.0f;
	_scale = Vector2(1, 1);
	_angleAnchor = Vector2::zero;
}

void Sprite::Init(BOOL isFrame, BOOL isLoop)
{
	_imgKey.clear();
	_isFrame = isFrame;
	_isLoop = isLoop;
	_isPlay = false;

	_curFrameX = _curFrameY = 0;
	_count = 0.0f;
	_FPS = 1.0f;
}

void Sprite::Render()
{
	DrawComponent::Render();

	if (_isShowRect)
	{
		if (_isFillRect) GRAPHICMANAGER->DrawFillRect(_object->GetTrans()->GetPos(), _object->GetTrans()->GetScale(), _object->GetTrans()->GetRotateDegree(), _color, _pivot, _isCameraEffect);
		else GRAPHICMANAGER->DrawRect(_object->GetTrans()->GetPos(), _object->GetTrans()->GetScale(), _object->GetTrans()->GetRotateDegree(), _color, _pivot, _strokeWidth, _isCameraEffect);
	}

	if (_imgKey.empty()) return;

	if (_isFrame)
	{
		//GRAPHICMANAGER->DrawFrameImage(_imgKey, _object->GetTrans()->GetPos(), _curFrameX, _curFrameY, _object->GetTrans()->GetScale(), _object->GetTrans()->GetRotateRadian(), _isFlipX, _alpha, _pivot, _isCameraAffect);
		_graphic->FrameRender(_pos, _curFrameX, _curFrameY, _scale, _object->GetTrans()->GetRotateDegree(), _angleAnchor, _isFlipX, _alpha, _pivot, _isCameraEffect);
	}
	else
	{
		//GRAPHICMANAGER->DrawImage(_imgKey, _object->GetTrans()->GetPos(), _object->GetTrans()->GetScale(), _object->GetTrans()->GetRotateRadian(), _isFlipX, _alpha, _pivot);
		_graphic->Render(_pos, _scale, _object->GetTrans()->GetRotateDegree(), _angleAnchor, _isFlipX, _alpha, _pivot, _isCameraEffect);
	}
}

void Sprite::Update()
{
	PlayAnimation();
}

void Sprite::Start()
{
	_isPlay = true;
	_curFrameX = 0;
	_graphic->SetCurrentFrameX(_curFrameX);
}

void Sprite::Stop()
{
	_isPlay = false;
	_curFrameX = 0;
	_graphic->SetCurrentFrameX(_curFrameX);
}

void Sprite::Pause()
{
	_isPlay = false;
}

void Sprite::Resume()
{
	_isPlay = true;
}

void Sprite::PlayAnimation()
{
	if (_isFrame)
	{
		if (_isPlay)
		{
			_count += TIMEMANAGER->getElapsedTime() * 5;

			if (_count >= _FPS)
			{
				_count -= _FPS;
				_curFrameX++;
				if (_curFrameX > _maxFrameX)
				{
					if (_isLoop) _curFrameX = 0;
					else
					{
						_curFrameX--;
						_isPlay = false;
					}
				}
			}
		}
	}
}

void Sprite::SetImgName(string key)
{
	_imgKey = key;
	_graphic = GRAPHICMANAGER->FindImage(_imgKey);
	_maxFrameX = _graphic->GetMaxFrameX();
	_maxFrameY = _graphic->GetMaxFrameY();
	_curFrameX = 0;

	_pos = _object->GetTrans()->GetPos();

	if (_isFrame) _isPlay = true;
}

bool Sprite::IsFrameEnd()
{
	if (_curFrameX >= _maxFrameX)
	{
		_curFrameX = _maxFrameX;
		return true;
	}
	return false;
}

