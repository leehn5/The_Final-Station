#include "stdafx.h"
#include "Button.h"

void Button::Init()
{
	_scale = Vector2(1, 1);
}

void Button::Release()
{
}

void Button::Update()
{
	if (PtInRect(&RectMakeCenter(_pos.x, _pos.y, _size.x, _size.y), MOUSEPOINTER->GetMouseWorldPosition().Vector2ToPOINT()))
	{
		if (_leftBtnDown) _dir = BtnDir::Down;
		else if (not _leftBtnDown and _dir == BtnDir::Down)
		{
			_dir = BtnDir::Up;
			if (_callback != nullptr) _callback();
			else if (_obj != nullptr) _callbackParameter(_obj);
		}
		else _dir = BtnDir::PlaceOn;
	}
	else _dir = BtnDir::None;
}

void Button::Render()
{
	switch (_dir)
	{
	case BtnDir::Up : case BtnDir::None :
		_sprite->FrameRender(_pos, _upFrame.x, _upFrame.y, _scale, 0.0f, Vector2::zero, false, 1.0f, PIVOT::CENTER, false);
		break;
	case BtnDir::Down : case BtnDir::PlaceOn :
		_sprite->FrameRender(_pos, _downFrame.x, _downFrame.y, _scale, 0.0f, Vector2::zero, false, 1.0f, PIVOT::CENTER, false);
		break;
	//case BtnDir::PlaceOn:
	//	_sprite->GetGraphic()->FrameRender(_trans->GetPos(), _upFrame.x, _upFrame.y, _sprite->GetScale(), 0.0f, Vector2::zero, false, 1.0f, PIVOT::CENTER, false);
	//	GRAPHICMANAGER->DrawFillRect(_trans->GetPos(), _trans->GetScale(), 0.0f, _hangOnColor, PIVOT::CENTER, false);
	//	break;
	}
}

void Button::Setting(string imgKey, Vector2 pos, POINT up, POINT down, Callback_Function cb)
{
	_callback = static_cast<Callback_Function>(cb);
	_callbackParameter = nullptr;
	_dir = BtnDir::None;

	_obj = nullptr;

	_pos = pos;

	_upFrame = up;
	_downFrame = down;

	_sprite = GRAPHICMANAGER->FindImage(imgKey);

	_size = Vector2(_sprite->GetFrameWidth() * _scale.x, _sprite->GetFrameHeight()* _scale.y);
}

void Button::Setting(string imgKey, Vector2 pos, POINT up, POINT down, Callback_Function_Parameter cbp, void* obj)
{
	_callback = nullptr;
	_callbackParameter = static_cast<Callback_Function_Parameter>(cbp);
	_dir = BtnDir::None;

	_obj = obj;

	_pos = pos;

	_upFrame = up;
	_downFrame = down;

	_sprite = GRAPHICMANAGER->FindImage(imgKey);

	_size = Vector2(_sprite->GetFrameWidth() * _scale.x, _sprite->GetFrameHeight() * _scale.y);
}

void Button::SetScale(Vector2 scale)
{
	//_sprite->SetScale(scale);
	_scale = scale;
	_size = Vector2(_sprite->GetFrameWidth() * _scale.x, _sprite->GetFrameHeight() * _scale.y);
}
